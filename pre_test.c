
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

#include "./minuitwrp/minui.h"
#include "recovery_ui.h"
#include "rtc_test.h"
#include "camera_test.h"
#include "screen_test.h"
#include "key_test.h"
#include "codec_test.h"
#include "wlan_test.h"
#include "bt_test.h"
#include "any_test.h" //zhangwei
#include "gsensor_test.h"
#include "sdcard_test.h"
#include "udisk_test.h"
#include "common.h"
#include "gui/gui.h"
#include "extra-functions.h"
#include "data.h"
#include "script.h"
#include "test_case.h"
#include "script_parser.h"
#include "debug.h"
#include "hdmi_test.h"
#include "sim_test.h"
#include "battery_test.h"
#include "ddr_test.h"
#include "cpu_test.h"

#include <signal.h>
#include "language.h"

#define SCRIPT_NAME                     "/res/test_config.cfg"
#define ITEM_H				2			//height of test item
#define ITEM_X				0			//x positon of test item

#define LOG(x...) printf(x)

pid_t g_codec_pid = -1;

static const char *TEMPORARY_LOG_FILE = "/tmp/recovery.log";

const char* man_title[] = { "manual test",
                        "",
                        NULL};
const char* man_items[]= {
				"Codec",
				"Backlight",
				"tp",
                        	NULL};

struct manual_item m_item[] = {
		/*name,  	x, 	y, 	w,	h,	argc, 	func*/
		{"Codec",	0,	3,	40,	5,	NULL,	NULL},
		{"KEY",		0,	9,	40,	5,	NULL,	NULL},
		{NULL,		0,	0,	0,	0,	NULL,	NULL}, //end of item
	};


int manual_p_y = 1;


int cur_p_y;		//current position for auto test tiem in y direction
pthread_t rtc_tid;  
char *rtc_res;
char dt[30]={"20120927.143045"};
struct rtc_msg *rtc_msg;
int err_rtc;
int rtc_p_y; //rtc position in y direction
pthread_t battery_tid;  

pthread_t screen_tid;  
char *screen_res;
struct screen_msg *screen_msg;
int screen_err = -1;


pthread_t codec_tid;
char *codec_res;
struct codec_msg *codec_msg;


pthread_t key_tid;
char *key_res;
struct key_msg *key_msg;


pthread_t camera_tid;  
char *camera_res;
struct camera_msg *camera_msg;
int camera_err = -1;

pthread_t camera1_tid;  


pthread_t wlan_tid;  
char *wlan_res;
struct wlan_msg *wlan_msg;
int wlan_err = -1;


pthread_t bt_tid;  
char *bt_res;
struct bt_msg *bt_msg;
int bt_err = -1;

//zhangwei
pthread_t any_tid;  
char *any_res;
struct any_msg *any_msg;
int any_err = -1;


pthread_t gsensor_tid;  
char *gsensor_res;
struct gsensor_msg *gsensor_msg;
int gsensor_err = -1;


pthread_t sd_tid;  
char *sd_res;
struct sd_msg *sd_msg;
int sd_err = -1;

pthread_t udisk_tid;  
char *udisk_res;
struct udisk_msg *udisk_msg;
int udisk_err = -1;

pthread_t hdmi_tid;  
char *hdmi_res;
struct sd_msg *hdmi_msg;
int hdmi_err = -1;

pthread_t sim_tid;

pthread_t ddr_tid;  
int ddr_err = -1;

pthread_t cpu_tid;
int cpu_err = -1;

static pthread_mutex_t gCur_p_y = PTHREAD_MUTEX_INITIALIZER;

int get_cur_print_y(void)
{
	int tmp;
	pthread_mutex_lock(&gCur_p_y);
	if (gr_fb_height() >600)
		cur_p_y--;
	 tmp = cur_p_y--;
	pthread_mutex_unlock(&gCur_p_y);
	printf("cur_print_y:%d\n",tmp);
	return tmp;
	
}

static int total_testcases = 0;
static struct testcase_base_info *base_info = NULL;
static struct list_head auto_test_list_head;
static struct list_head manual_test_list_head;


static int parse_testcase()
{
    int i, j, mainkey_cnt;
    struct testcase_base_info *info;
    char mainkey_name[32], display_name[64], binary[16];
    int activated, category, run_type;
    int len;

    mainkey_cnt = script_mainkey_cnt();
    info = (struct testcase_base_info*)malloc(sizeof(struct testcase_base_info) * mainkey_cnt);
    if (info == NULL) {
        db_error("core: allocate memory for temporary test case basic "
                "information failed(%s)\n", strerror(errno));
        return -1;
    }
    memset(info, 0, sizeof(struct testcase_base_info) * mainkey_cnt);

    for (i = 0, j = 0; i < mainkey_cnt; i++) {
		struct testcase_info *tc_info;
        memset(mainkey_name, 0, 32);
        script_mainkey_name(i, mainkey_name);

        if (script_fetch(mainkey_name, "display_name", (int *)display_name, 16))
            continue;

        if (script_fetch(mainkey_name, "activated", &activated, 1))
            continue;

        if (display_name[0] && activated == 1) {
            strncpy(info[j].name, mainkey_name, 32);
            strncpy(info[j].display_name, display_name, 64);
            info[j].activated = activated;

            if (script_fetch(mainkey_name, "program", (int *)binary, 4) == 0) {
                strncpy(info[j].binary, binary, 16);
            }

            info[j].id = j;

            if (script_fetch(mainkey_name, "category", &category, 1) == 0) {
                info[j].category = category;
            }

            if (script_fetch(mainkey_name, "run_type", &run_type, 1) == 0) {
                info[j].run_type = run_type;
            }
			tc_info = (struct testcase_info*) malloc(sizeof(struct testcase_info));
			if(tc_info == NULL)
			{
				printf("malloc for tc_info[%d] fail\n",j);
				return -1;
			}
			tc_info->x = 0;
			tc_info->y =0;
			tc_info->base_info = &info[j];
			if(tc_info->base_info->category)   
				 list_add(&tc_info->list, &manual_test_list_head);
			else
				 list_add(&tc_info->list, &auto_test_list_head);
			//printf("name:%s>>disp_name:%s>>activated:%d>>binary:%s>>id:%d>>category:%d>>run_type:%d\n",
			//	info[j].name,info[j].display_name, info[j].activated,info[j].binary,info[j].id, info[j].category,
			//	 info[j].run_type);
            j++;
        }
    }
    total_testcases = j;

    db_msg("core: total test cases #%d\n", total_testcases);
    if (total_testcases == 0) {
        return 0;
    }

    len = sizeof(struct testcase_base_info) * total_testcases;
	#if 0
    base_info_shmid = shmget(IPC_PRIVATE, len, IPC_CREAT | 0666);
    if (base_info_shmid == -1) {
        db_error("core: allocate share memory segment for test case basic "
                "information failed(%s)\n", strerror(errno));
        return -1;
    }

    base_info = shmat(base_info_shmid, 0, 0);
    if (base_info == (void *)-1) {
        db_error("core: attach the share memory for test case basic "
                "information failed(%s)\n", strerror(errno));
        shmctl(base_info_shmid, IPC_RMID, 0);
        return -1;
    }
    memcpy(base_info, info, sizeof(struct testcase_base_info) * 
            total_testcases);
	#endif

    return total_testcases;
}


int start_test_pthread(struct testcase_info *tc_info)
{
	int err;
	printf("%s\n",tc_info->base_info->name);

	if(!strcmp(tc_info->base_info->name, "Lcd"))
	{
		err = pthread_create(&screen_tid, NULL, screen_test,tc_info); //
		if(err != 0)
		{  
			   printf("create screen test thread error: %s/n",strerror(err));
			   return -1;
			   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "rtc"))
	{
		err = pthread_create(&rtc_tid, NULL, rtc_test,tc_info); //
		if(err != 0)
		{  
		   printf("create rtc test thread error: %s/n",strerror(err));
		   return -1;
		   
		}  
	}else if(!strcmp(tc_info->base_info->name, "battery"))
	{
		err = pthread_create(&battery_tid, NULL, battery_test,tc_info); //
		if(err != 0)
		{  
		   printf("create battery_test test thread error: %s/n",strerror(err));
		   return -1;
		   
		}  
	}else if(!strcmp(tc_info->base_info->name, "Codec"))
        {
                err = pthread_create(&codec_tid, NULL, codec_test,tc_info); //
                if(err != 0)
                {
                   printf("create codec test thread error: %s/n",strerror(err));
                   return -1;

                }
        }else if(!strcmp(tc_info->base_info->name, "Key"))
	{
		err = pthread_create(&key_tid, NULL, key_test,tc_info); //
		if(err != 0)
		{  
		   printf("create key test thread error: %s/n",strerror(err));
		   return -1;
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "camera"))
	{
		tc_info->dev_id = 0;
		err = pthread_create(&camera_tid, NULL, camera_test,tc_info); //
		if(err != 0)
		{  
		   printf("create camera test thread error: %s/n",strerror(err)); 
		   return -1;
		   
		}  
		
	}
	else if(!strcmp(tc_info->base_info->name, "wifi"))
	{
		//ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"iiiiiiiiii wifi 1\n");
		
		err = pthread_create(&wlan_tid, NULL, wlan_test,tc_info); //
		if(err != 0)
		{  
		   printf("create wifi test thread error: %s/n",strerror(err));	
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "bluetooth"))
	{
		printf("bluetooth_test thread created\n");

		err = pthread_create(&bt_tid, NULL, bt_test,tc_info); //
		if(err != 0)
		{  
		   printf("create bt(bluetooth) test thread error: %s/n",strerror(err));	
		   
		}  
	}
	//zhangwei
	else if(!strcmp(tc_info->base_info->name, "any"))
	{
		printf("any_test thread created\n");

		err = pthread_create(&any_tid, NULL, any_test,tc_info); //
		if(err != 0)
		{  
		   printf("create any test thread error: %s/n",strerror(err));	
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "gsensor"))
	{
		err = pthread_create(&gsensor_tid, NULL, gsensor_test,tc_info); //
		if(err != 0)
		{  
		   printf("create camera test thread error: %s/n",strerror(err)); 
		   return -1;
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "udisk"))
	{
		err = pthread_create(&udisk_tid, NULL, udisk_test,tc_info); //
		if(err != 0)
		{  
		   printf("create sdcard test thread error: %s/n",strerror(err)); 
		   return -1;
		   
		}
	}
	else if(!strcmp(tc_info->base_info->name, "sdcard"))
	{
		sd_err = pthread_create(&sd_tid, NULL, sdcard_test,tc_info); //
		if(sd_err != 0)
		{  
		   printf("create sdcard test thread error: %s/n",strerror(sd_err));
		   return -1;
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "hdmi"))
	{
		hdmi_err = pthread_create(&hdmi_tid, NULL, hdmi_test, tc_info); //
		if(hdmi_err != 0)
		{  
		   printf("create hdmi test thread error: %s/n",strerror(hdmi_err));
		   return -1;
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "sim"))
	{
		err = pthread_create(&sim_tid, NULL, sim_test,tc_info); //
		if(err != 0)
		{  
		   printf("create sim test thread error: %s/n",strerror(err));
		   return -1;
		   
		}  
	}	
	else if(!strcmp(tc_info->base_info->name, "ddr"))
	{
		ddr_err = pthread_create(&ddr_tid, NULL, ddr_test,tc_info); //
		if(ddr_err != 0)
		{  
		   printf("create ddr test thread error: %s/n",strerror(ddr_err));
		   return -1;
		   
		}  
	}
	else if(!strcmp(tc_info->base_info->name, "cpu")){
		cpu_err = pthread_create(&cpu_tid, NULL, cpu_test, tc_info);
		if(cpu_err != 0){
		   printf("create cpu test thread error: %s/n",strerror(cpu_err));
		   return -1;
		}
	}
	else
	{
		printf("unsupport test item:%s\n",tc_info->base_info->name);
		return -1;
	}

	return 0;
			
}

int init_manual_test_item(struct testcase_info *tc_info)
{
	int err = 0;

	manual_p_y += 1;
	tc_info->y=manual_p_y;

	start_test_pthread(tc_info);
	
	return 0;
}




int start_manual_test_item(int x,int y)
{
	return Camera_Click_Event(x,y); 
}

int start_auto_test_item(struct testcase_info *tc_info)
{
	printf("start_auto_test_item : %d, %s \r\n",tc_info->y,tc_info->base_info->name);
	start_test_pthread(tc_info);
	
	return 0;
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



int main(int argc, char **argv)
{
	int ret,w;
	char *script_buf;
	struct list_head *pos;
	int success = 0;
	
	freopen("/dev/ttyFIQ0", "a", stdout); setbuf(stdout, NULL);
	freopen("/dev/ttyFIQ0", "a", stderr); setbuf(stderr, NULL);

    	
	if (gui_init())
	{
		ui_init();
		ui_set_background(BACKGROUND_ICON_INSTALLING);
	}
	ui_print_init();
	gui_loadResources();
#if 1
	w =  gr_fb_width() >> 1;
	ui_print_xy_rgba(((w>>1)/CHAR_WIDTH-9),0,0,255,0,255,"%s\n",PCBA_VERSION_NAME);
//	ui_print_xy_rgba(0,1,255,0,0,255,"%s %s\n",__DATE__,__TIME__);
	ui_print_xy_rgba(((w>>1)/CHAR_WIDTH-3),1,255,255,0,255,"%s\n",PCBA_MANUAL_TEST);
    drawline_4(255,255,0,255,0,(1*CHAR_HEIGHT-CHAR_HEIGHT/4),w,CHAR_HEIGHT,3);
	cur_p_y = (gr_fb_height()/CHAR_HEIGHT) - 1;
	INIT_LIST_HEAD(&manual_test_list_head);
	INIT_LIST_HEAD(&auto_test_list_head);
	script_buf = parse_script(SCRIPT_NAME);
	if (!script_buf)
	{
		   printf("parse script failed\n");
		   return -1;
	}
	
	ret = init_script(script_buf);
	if (ret) {
		   db_error("core: init script failed(%d)\n", ret);
		   return -1;
	}
	
	ret = parse_testcase();
	if (ret < 0) {
		db_error("core: parse all test case from script failed(%d)\n", ret);
		return -1;
	}
	else if (ret == 0) {
		db_warn("core: NO TEST CASE to be run\n");
		return -1;
	}

	printf("manual testcase:\n");
	list_for_each(pos, &manual_test_list_head) {
		struct testcase_info *tc_info = list_entry(pos, struct testcase_info, list);
		init_manual_test_item(tc_info);
		
	}
	ui_print_xy_rgba(((w>>1)/CHAR_WIDTH-3),manual_p_y+1,255,255,0,255,"%s\n",PCBA_AUTO_TEST);
        drawline_4(255,255,0,255,0,(CHAR_HEIGHT*(manual_p_y+1)-CHAR_HEIGHT/4),w,CHAR_HEIGHT,3); 

	printf("\n\nauto testcase:\n");
	list_for_each(pos, &auto_test_list_head) {
		struct testcase_info *tc_info = list_entry(pos, struct testcase_info, list);
		start_auto_test_item(tc_info);
	}
	
#endif
	//while(1);
	gui_start();
	start_input_thread();

	//prompt_and_wait();
	stopCameraTest();
	list_for_each(pos, &auto_test_list_head) {
		struct testcase_info *tc_info = list_entry(pos, struct testcase_info, list);
		if(tc_info->result == -1){
			success = -1;
			printf("%s test fail!\n",tc_info->base_info->name);
			
		}
	}
	finishCameraTest();

	/*if(g_codec_pid > 0){
		kill(g_codec_pid,SIGKILL);
		printf("pcba-test-over\n");
		sleep(3);
	}*/

	printf("pcba test over!\n");
	return success;
}

