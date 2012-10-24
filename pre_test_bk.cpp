
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
	#include "common.h"
	#include "gui/gui.h"
	#include "extra-functions.h"
	#include "data.h"
	
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
#if 0
	ui_print_xy_rgba(0,0,255,0,0,255,"Rockchip Pcba test v1.0\n");
	ui_print_xy_rgba(0,1,255,0,0,255,"%s %s\n",__DATE__,__TIME__);
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
		ui_print_xy_rgba(0,cur_p_y++,0,0,255,100,"rtc test success:%s\n",rtc_msg->date);
	}
#endif
	//while(1);
	gui_start();
	prompt_and_wait();
	return 0;
}
#endif
