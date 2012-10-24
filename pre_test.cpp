
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <inttypes.h>
#include <errno.h>
#include <pthread.h>


extern "C" {
	#include "./minuitwrp/minui.h"
	#include "recovery_ui.h"
	#include "rtc_test.h"
	#include "camera_test.h"
	#include "screen_test.h"
	#include "key_test.h"
	#include "codec_test.h"
	#include "wlan_test.h"
	#include "gsensor_test.h"
	#include "sdcard_test.h"
	#include "common.h"
	#include "gui/gui.h"
	#include "extra-functions.h"
	#include "data.h"
	
}


struct manual_item m_item[] = {
		/*name,  	x, 	y, 	w,	h,	argc, 	func*/
		{"Codec",	0,	3,	40,	3,	NULL,	NULL},
		{"KEY",		0,	6,	40,	3,	NULL,	NULL},
		{NULL,		0,	0,	0,	0,	NULL,	NULL}, //end of item
	};

int init_manual_test_item(void)
{
	struct manual_item *item = m_item;
	while(item->name != NULL)
	{
		if(!strcmp(item->name, "Codec"))
		{
			item->func = codec_test;
		}
		else if(!strcmp(item->name, "KEY"))
		{
			struct key_msg *key_msg = (struct key_msg *)malloc(sizeof(struct key_msg));
			if(!key_msg)
			{
				printf("malloc for key_msg fail!\n");
			}
			else
			{
				key_msg->result = -1;
				key_msg->x = item->x + strlen(item->name) + 4;
				key_msg->y = item->y;
				key_msg->w = item->w;
				key_msg->h = item->h;
			}
			item->func = key_test;
			item->argc = key_msg;
		}

		//printf("%s>>x:%d>>y:%d>>w:%d>>h:%d\n",
		//		item->name,item->x,item->y,item->w,item->h);
		ui_print_xy_rgba(item->x,item->y,0,0,255,255,"%s\n",item->name);
		item++;
	}
	return 0;
}

extern "C" int start_manual_test_item(int x,int y)
{
	struct manual_item *item = m_item;
	int x_start,x_end;
	int y_start,y_end;
	while(item->name != NULL)
	{
		x_start = (item->x)*CHAR_WIDTH;
		x_end = x_start + (item->w)*CHAR_WIDTH;
		y_start = (item->y - 1)*CHAR_HEIGHT;
		y_end = y_start + (item->h)*CHAR_HEIGHT;
		//printf("%s>>x_start:%d>>x_end:%d>>y_start:%d>>y_end:%d\n",
		//	item->name,x_start,x_end,y_start,y_end);
		if( (x >= x_start) && (x <= x_end) && (y >= y_start) && (y <= y_end))
		{
			item->func(item->argc);
			break;
		}
		item++;
	}
	return 0;
	
}
static const char *TEMPORARY_LOG_FILE = "/tmp/recovery.log";

const char* man_title[] = { "manual test",
                        "",
                        NULL};
const char* man_items[]= {
							"Codec",
							"Backlight",
							"tp",
                        	NULL};

	//RecoveryUI* ui = NULL;

int cur_p_y;		//current position for auto test tiem in y direction
pthread_t rtc_tid;  
char *rtc_res;
char dt[30]={"20120927.143045"};
struct rtc_msg *rtc_msg;
int err_rtc;
int rtc_p_y; //rtc position in y direction

pthread_t screen_tid;  
char *screen_res;
struct screen_msg *screen_msg;
int screen_err = -1;


pthread_t camera_tid;  
char *camera_res;
struct camera_msg *camera_msg;
int camera_err = -1;

pthread_t wlan_tid;  
char *wlan_res;
struct wlan_msg *wlan_msg;
int wlan_err = -1;

pthread_t gsensor_tid;  
char *gsensor_res;
struct gsensor_msg *gsensor_msg;
int gsensor_err = -1;


pthread_t sd_tid;  
char *sd_res;
struct sd_msg *sd_msg;
int sd_err = -1;

static pthread_mutex_t gCur_p_y = PTHREAD_MUTEX_INITIALIZER;

int get_cur_print_y(void)
{
	int tmp;
	pthread_mutex_lock(&gCur_p_y);
	 tmp = cur_p_y++;
	pthread_mutex_unlock(&gCur_p_y);
	printf("cur_print_y:%d\n",tmp);
	return tmp;
	
}

int ensure_path_mounted(const char* path) {return 0;}

int 
get_menu_selection(char** headers, char** items, int menu_only,
                   int initial_selection) {
    // throw away keys pressed previously, so user doesn't
    // accidentally trigger menu items.
    ui_clear_key_queue();

    ui_start_menu(headers, items, initial_selection);
    int selected = initial_selection;
    int chosen_item = -1;
    
    while (chosen_item < 0) {
        int key = ui_wait_key();
        int visible = ui_text_visible();

        int action = device_handle_key(key, visible);
        if (action < 0) {
            switch (action) {
                case HIGHLIGHT_UP:
                    --selected;
                    selected = ui_menu_select(selected);
                    break;
                case HIGHLIGHT_DOWN:
                    ++selected;
                    selected = ui_menu_select(selected);
                    break;
                case KEY_POWER:
                case SELECT_ITEM:
                    chosen_item = selected;
                    break;
                case UP_A_LEVEL:
                	if (menu_loc_idx != 0)
                	{
                		chosen_item = menu_loc[menu_loc_idx];
                	}
                    break;
                case HOME_MENU:
                	if (menu_loc_idx != 0)
                	{
                		go_home = 1;
                		chosen_item = menu_loc[menu_loc_idx];
                	}
                    break;
                case MENU_MENU:
                	if (menu_loc_idx == 0)
                	{
                	    return 3;
                	} else
                	{
                    	go_home = 1;
                    	go_menu = 1;
                    	chosen_item = menu_loc[menu_loc_idx];
                	}
                    break;
                case NO_ACTION:
                    break;
            }
        } else if (!menu_only) {
            chosen_item = action;
        }
    }

    ui_end_menu();
    return chosen_item;
}


char**
prepend_title(const char** headers) {
    char* title1 = (char*)malloc(40);
    strcpy(title1, "Team Win Recovery Project (twrp) v");
    char* header1 = strcat(title1, DataManager_GetStrValue(TW_VERSION_VAR));
    char* title[] = { header1,
                      "Based on Android System Recovery <"
                      EXPAND(RECOVERY_API_VERSION) "e>",
                      "", //
                      print_batt_cap(),
                      "", //
                      NULL };

    // count the number of lines in our title, plus the
    // caller-provided headers.
    int count = 0;
    char** p;
    for (p = title; *p; ++p, ++count);
    for (p = (char**) headers; *p; ++p, ++count);

    char** new_headers = (char**)malloc((count+1) * sizeof(char*));
    char** h = new_headers;
    for (p = title; *p; ++p, ++h) *h = *p;
    for (p = (char**) headers; *p; ++p, ++h) *h = *p;
    *h = NULL;

    return new_headers;
}

void prompt_and_wait()
{

	// Main Menu
	#define START_FAKE_MAIN          0
	#define REALMENU_REBOOT     	 1

	go_reboot = 0;
    //ui_reset_progress();

	char** headers = prepend_title((const char**)MENU_HEADERS);
    char* MENU_ITEMS[] = {  "Start Recovery",
                            "Reboot",
                            NULL };
	
    for (;;)
	{

        go_home = 0;
        go_menu = 0;
        menu_loc_idx = 0;
		

        if (go_reboot)
		{
			return;
		}
		show_fake_main_menu();
    }
}


#if 0
int main(int argc, char **argv)
{	
	
	
	time_t start = time(NULL);
	
	printf("Starting pcba test on %s", ctime(&start));
	printf("Build time:%s %s\n",__DATE__,__TIME__);

    Device* device = make_device();
    ui = device->GetUI();

    ui->Init();
	ui->ShowText(true);
    ui->SetBackground(RecoveryUI::NONE);
    ui->Print(2,2,100,5,5,100,"Pcba test system v1.0 \n\n");
	cur_p_y = (gr_fb_height()/CHAR_HEIGHT)>> 1;
	rtc_msg = (struct rtc_msg *)malloc(sizeof(struct rtc_msg));
	if(!rtc_msg)
	{
		printf("malloc for rtc_msg fail!\n");
		err_rtc = -1;
	}
	else
	{
		rtc_msg->result = -1;
		rtc_msg->date = dt;
		err_rtc = pthread_create(&rtc_tid, NULL, rtc_test,rtc_msg); //
		if(err_rtc != 0)
		{  
		   printf("create rtc test thread error: %s/n",strerror(err_rtc));  
		   
		}  
	}

	screen_msg = (struct screen_msg *)malloc(sizeof(struct screen_msg));
	if(!screen_msg)
	{
		printf("malloc for screen_msg fail!\n");
		screen_err = -1;
	}
	else
	{
		screen_msg->result = -1;
		screen_msg->x = gr_fb_width() >> 1;
		screen_msg->y = (gr_fb_height()*2)/3;
		screen_msg->w = gr_fb_width() >> 1;
		screen_msg->h = gr_fb_height()/3;
		screen_msg->ui = ui;
		screen_err = pthread_create(&screen_tid, NULL, screen_test,screen_msg); //
		if(screen_err != 0)
		{  
		   printf("create screen test thread error: %s/n",strerror(screen_err));  
		   
		}  
	}


	camera_msg = (struct camera_msg *)malloc(sizeof(struct camera_msg));
	if(!camera_msg)
	{
		printf("malloc for camera_msg fail!\n");
		camera_err = -1;
	}
	else
	{
		camera_msg->result = -1;
		camera_msg->id = 0;
		camera_msg->x = gr_fb_width() >> 1;
		camera_msg->y = 0;
		camera_msg->w = gr_fb_width() >> 1;
		camera_msg->h = gr_fb_height()*2/3;
		//ui->FillColor(255,255,255,255,camera_msg->x,0,camera_msg->w,camera_msg->h); 
		camera_err = pthread_create(&camera_tid, NULL, camera_test,camera_msg); //
		if(camera_err != 0)
		{  
		   printf("create camera test thread error: %s/n",strerror(camera_err));  
		   
		}  
	}
	
	
	if(!err_rtc)
	{
		pthread_join(rtc_tid,(void**)&rtc_res); 
		rtc_msg = (struct rtc_msg *)(rtc_res);
		if(rtc_msg->result >= 0)
			ui->Print(2,cur_p_y++,0,0,255,100,"rtc test success:%s\n",rtc_msg->date);
	}

	if(!screen_err)
	{
		pthread_join(screen_tid,(void**)&screen_res); 
		screen_msg = (struct screen_msg *)(screen_res);
		printf("screen test %s\n",(screen_msg->result >= 0)? "success":"fail");
	}

	prompt_and_wait(device,man_title, man_items, 0,2,4);
	
	return 0;
}
#else
int
main(int argc, char **argv)
{
    
	if (gui_init())
	{
		ui_init();
		ui_set_background(BACKGROUND_ICON_INSTALLING);
	}
	ui_print_init();
	gui_loadResources();
#if 1
	ui_print_xy_rgba(0,0,255,0,0,255,"Rockchip Pcba test v1.0\n");
	ui_print_xy_rgba(0,1,255,0,0,255,"%s %s\n",__DATE__,__TIME__);
	init_manual_test_item();
	//FillColor(255,0,0,255,400,240,400,240);
	cur_p_y = (gr_fb_height()/CHAR_HEIGHT)>> 1;

	screen_msg = (struct screen_msg *)malloc(sizeof(struct screen_msg));
	if(!screen_msg)
	{
		printf("malloc for screen_msg fail!\n");
		screen_err = -1;
	}
	else
	{
		screen_msg->result = -1;
		screen_msg->x = gr_fb_width() >> 1;
		screen_msg->y = (gr_fb_height()*2)/3;
		screen_msg->w = gr_fb_width() >> 1;
		screen_msg->h = gr_fb_height()/3;
		screen_err = pthread_create(&screen_tid, NULL, screen_test,screen_msg); //
		if(screen_err != 0)
		{  
		   printf("create screen test thread error: %s/n",strerror(screen_err));  
		   
		}  
	}


	rtc_msg = (struct rtc_msg *)malloc(sizeof(struct rtc_msg));
	if(!rtc_msg)
	{
		printf("malloc for rtc_msg fail!\n");
		err_rtc = -1;
	}
	else
	{
		rtc_msg->result = -1;
		rtc_msg->date = dt;
		err_rtc = pthread_create(&rtc_tid, NULL, rtc_test,rtc_msg); //
		if(err_rtc != 0)
		{  
		   printf("create rtc test thread error: %s/n",strerror(err_rtc));  
		   
		}  
	}


	camera_msg = (struct camera_msg *)malloc(sizeof(struct camera_msg));
	if(!camera_msg)
	{
		printf("malloc for camera_msg fail!\n");
		camera_err = -1;
	}
	else
	{
		camera_msg->result = -1;
		camera_msg->id = 0;
		camera_msg->x = gr_fb_width() >> 1;
		camera_msg->y = 0;
		camera_msg->w = gr_fb_width() >> 1;
		camera_msg->h = gr_fb_height()*2/3;
		//ui->FillColor(255,255,255,255,camera_msg->x,0,camera_msg->w,camera_msg->h); 
		camera_err = pthread_create(&camera_tid, NULL, camera_test,camera_msg); //
		if(camera_err != 0)
		{  
		   printf("create camera test thread error: %s/n",strerror(camera_err));  
		   
		}  
	}

	wlan_msg = (struct wlan_msg *)malloc(sizeof(struct wlan_msg));
	if(!wlan_msg)
	{
		printf("malloc for wlan_msg fail!\n");
		wlan_err = -1;
	}
	else
	{
		wlan_msg->result = -1;
		wlan_msg->ssid = (char *)malloc(sizeof(char));
		wlan_msg->y = cur_p_y++;
		wlan_err = pthread_create(&wlan_tid, NULL, wlan_test,wlan_msg); //
		if(wlan_err != 0)
		{  
		   printf("create camera test thread error: %s/n",strerror(wlan_err));  
		   
		}  
	}

	gsensor_msg = (struct gsensor_msg *)malloc(sizeof(struct gsensor_msg));
	if(!gsensor_msg)
	{
		printf("malloc for wlan_msg fail!\n");
		gsensor_err = -1;
	}
	else
	{
		gsensor_msg->result = -1;
		gsensor_msg->y = get_cur_print_y();
		gsensor_err = pthread_create(&gsensor_tid, NULL, gsensor_test,gsensor_msg); //
		if(gsensor_err != 0)
		{  
		   printf("create camera test thread error: %s/n",strerror(gsensor_err));  
		   
		}  
	}

	sd_msg = (struct sd_msg *)malloc(sizeof(struct sd_msg));
	if(!sd_msg)
	{
		printf("malloc for sd_msg fail!\n");
		sd_err = -1;
	}
	else
	{
		sd_msg->result = -1;
		sd_err = pthread_create(&sd_tid, NULL, sdcard_test,sd_msg); //
		if(sd_err != 0)
		{  
		   printf("create sdcard test thread error: %s/n",strerror(sd_err));  
		   
		}  
	}

	if(!screen_err)
	{
		pthread_join(screen_tid,(void**)&screen_res); 
		screen_msg = (struct screen_msg *)(screen_res);
		printf("screen test %s\n",(screen_msg->result >= 0)? "success":"fail");
	}

	if(!err_rtc)
	{
		pthread_join(rtc_tid,(void**)&rtc_res); 
		rtc_msg = (struct rtc_msg *)(rtc_res);
		if(rtc_msg->result >= 0)
		ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"rtc test success:%s\n",rtc_msg->date);
	}
#endif
	//while(1);
	gui_start();
	//prompt_and_wait();
	printf("pcba test over!\n");
	return 0;
}
#endif
