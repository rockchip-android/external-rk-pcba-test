#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>

#include "fb_test.h"


#include "../../bootable/recovery/common.h"
#include "cutils/properties.h"
#include "../../bootable/recovery/minui/minui.h"
#include "../../bootable/recovery/minzip/DirUtil.h"


struct touch_point
{
     int x;
	 int y;
		     };



#define TP_CAL_PATH "sys/devices/tp_calibration/tp_calibration"
 int touchpanel_test(void)
{
	struct input_event touch_ev;
  	struct point point;
	struct touch_point point_buf[3];
	char tcp_cmd[7] = {"tp_cal"};
    char tcp_sta[11]= {"fail"};
	int fd =0;
	int ret = -1;
 	int i = 0;
 	int touch_conter = 0;
	int pressure = 0;
	unsigned short * fb_map;
	int touch_max_x = get_fb_width();
	int touch_max_y = get_fb_height();

 	point.x = 0;
 	point.y = 0;
	
	ui_clear_screen();
	ui_print("Touch Test, please press touch\n");
#if 0
	fd = open(TP_CAL_PATH,O_RDWR);
  	if(fd < 0)
  	{
  		INFO("open TP_CAL err\n");
		return -1;
  	}
  	else
  	{
  		ui_print("calibrating the touch panel ,do not touch it!\n");
  	}
  	INFO("%d\n",sizeof(tcp_cmd));
  	ret = write(fd,tcp_cmd,sizeof(tcp_cmd));
  	if(ret < sizeof(tcp_cmd))
  	{
  		INFO("write tcp_cmd err\n");
  	}
  	sleep(5);
 	 do
  	{
  		lseek(fd,0,SEEK_SET);
  		ret = read(fd,tcp_sta,sizeof(tcp_sta)-1);
  		if(ret < (sizeof(tcp_sta)-1))
  		{
  	 	INFO("read tcp_sta error:%s>>>%d\n",tcp_sta,ret);
	 	i++;
	 	sleep(1);
  		}
		else
		{
	 	 break;
		}
  	}while(i<5);
  	if(!strcmp(tcp_sta,"successful"))
  	{
  		ui_print("touch calibrating successful\nplease move you finger on the screen\n");
  	}
#endif
	#if 0
	ret = open_fb(rk_fb);
	 fb_graphic_init();
	fb_map = get_fb_mem();
	
	point.x = 400;
	point.y = 240;
	set_back_ground(fb_map,0x00,0x00,0x00);
 	draw_fill_circle(fb_map,point);
	#endif
	
	ui_print("Please key the power key to close the touch test!\n");
	 while(1)
	 {
	 	ev_get(&touch_ev,10);
	 	if(touch_ev.type==EV_ABS)  //one touch happen
     	{
			INFO("cdy === type:0x%x>>>code:0x%x>>>value:0x%x\n",touch_ev.type,touch_ev.code,touch_ev.value);
  	    	if(touch_ev.code==ABS_MT_POSITION_X)
	  	    	point.x=touch_ev.value;
	    	else if(touch_ev.code==ABS_MT_POSITION_Y)
	  	    	point.y=touch_ev.value;
			else if(touch_ev.code==ABS_MT_TOUCH_MAJOR)
				pressure = touch_ev.value ;

			if((point.x>0)&&(point.x<touch_max_x)&&(point.y>0)&&(point.y<touch_max_y)&&pressure) //press down
       		{
			    INFO("cdy === >>>>>>(%d,%d)\n",point.x,point.y);
			   #if 1
				//set_fb_background();
				draw_square(point.x,point.y);
				ret = 1;
				#else
				point.x -= 7; 
			   set_back_ground(fb_map,0x00,0x00,0x00);
			   draw_fill_circle(fb_map,point);
				ret = 1;
				printf("%d,%d\n",point.x,point.y);
			   #endif
				 
       		}
			else if(!pressure) //up
			{
				point.x = 0;
				point.y = 0;
				//set_fb_background();
				//set_back_ground(fb_map,0x00,0x00,0x00);
			}

     	}
	    else if((touch_ev.type == EV_KEY)&&(!touch_ev.value))
		{
			//printf("key code = %d \n",touch_ev.code);
			if(touch_ev.code ==KEY_POWER)
			{
			//	ev_exit();
				break;
			}
		 }
     

	 }
	printf("touch ret = %d \n",ret);
	 return ret;
}
