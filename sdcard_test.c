#include<stdio.h>
#include <stdlib.h>

#include"extra-functions.h"
#include"common.h"
#include"sdcard_test.h"
#include "test_case.h"
#include "language.h"

#define SCAN_RESULT_LENGTH 128
#define SCAN_RESULT_FILE "/data/sd_capacity"

void * sdcard_test(void * argv)
{
	
	struct testcase_info *tc_info = (struct testcase_info*)argv;
	int ret,y;
	double cap;
	FILE *fp;
	char results[SCAN_RESULT_LENGTH];
	
	/*remind ddr test*/
	if(tc_info->y <= 0)
		tc_info->y  = get_cur_print_y();	

	y = tc_info->y;
	ui_print_xy_rgba(0,y,255,255,0,255,"%s \n",PCBA_SDCARD);

	#ifdef RK3288_PCBA
	ret =  __system("busybox chmod 777 /res/emmctester.sh");
	#else
		#ifdef SOFIA3GR_PCBA
		//ret =  __system("busybox chmod 777 /system/bin/mmctester.sh");
		#else
		ret =  __system("busybox chmod 777 /res/mmctester.sh");
		#endif
	#endif
	if(ret)
		printf("chmod mmctester.sh failed :%d\n",ret);
	
    #ifdef RK3288_PCBA      
    ret = __system("/res/emmctester.sh");
    #else
		#ifdef SOFIA3GR_PCBA
			int testCounts =0;
			while(1)
			{
				fp = NULL;
				if(__system("/system/bin/mmctester.sh") > 0)
				{
					fp = fopen(SCAN_RESULT_FILE, "r");
				}
				
				if(fp != NULL)
				{
					break;
				}
				
				if(testCounts++ > 5)
				{
					printf("can not open %s.\n", SCAN_RESULT_FILE);
					ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s]\n",PCBA_SDCARD,PCBA_FAILED);
					tc_info->result = -1;
					return argv;
				}
				sleep(1);
			}

			memset(results, 0, SCAN_RESULT_LENGTH);
			//fread(results, 1, SCAN_RESULT_LENGTH, fp);
			fgets(results,50,fp);
			//fgets(wlan_msg->ssid,50,fp); //we assume tha a AP's name is less of 50 charactes
			
			//LOG("%s.\n", results);
			
			cap = strtod(results,NULL);
			if(cap)
				ui_print_xy_rgba(0,y,0,255,0,255,"%s:[%s] { %2fG } \n",PCBA_SDCARD,PCBA_SECCESS,cap*1.0);
		    fclose(fp);

			return argv;
		#else
		ret = __system("/res/mmctester.sh");
		#endif
    #endif
		
	if(ret < 0) {
		printf("mmc test failed.\n");
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s]\n",PCBA_SDCARD,PCBA_FAILED);
		tc_info->result = -1;
		return argv;
	}
	
	
	fp = fopen(SCAN_RESULT_FILE, "r");
	if(fp == NULL) {
		printf("can not open %s.\n", SCAN_RESULT_FILE);
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s]\n",PCBA_SDCARD,PCBA_FAILED);
		tc_info->result = -1;
		return argv;
	}

	
	/*
		#ifdef SOFIA3GR_PCBA
		if(stat("/system/etc/images/wifi_scan.txt", &stat_ret) < 0) {
		#else
		if(stat("/res/images/wifi_scan.txt", &stat_ret) < 0) {
		#endif
		LOG( "error getting file stat.\n");				 
		return -1;
	}*/

  	memset(results, 0, SCAN_RESULT_LENGTH);
	//fread(results, 1, SCAN_RESULT_LENGTH, fp);
	fgets(results,50,fp);
	//fgets(wlan_msg->ssid,50,fp); //we assume tha a AP's name is less of 50 charactes
	
	//LOG("%s.\n", results);
	
	cap = strtod(results,NULL);
	if(cap)
	{
		ui_print_xy_rgba(0,y,0,255,0,255,"%s:[%s] { %2fG } \n",PCBA_SDCARD,PCBA_SECCESS,cap*1.0/1024/1024);
	}
    fclose(fp);

	return argv;
	
}
