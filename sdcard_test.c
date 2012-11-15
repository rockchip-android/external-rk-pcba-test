#include<stdio.h>
#include <stdlib.h>

#include"extra-functions.h"
#include"common.h"
#include"sdcard_test.h"

#define SCAN_RESULT_LENGTH 128
#define SCAN_RESULT_FILE "/data/sd_capacity"

void * sdcard_test(void * argv)
{
	
	struct sd_msg *sd_msg = (struct sd_msg *)malloc(sizeof(struct sd_msg));
	int ret;
	double cap;
	FILE *fp;
	char results[SCAN_RESULT_LENGTH];
	
	ret =  __system("busybox chmod 777 /sbin/mmctester.sh");
	if(ret)
		printf("chmod mmctester.sh failed :%d\n",ret);
		
	ret = __system("/sbin/mmctester.sh");
	if(ret < 0) {
		printf("mmc test failed.\n");
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"sdcard test fail\n");
		sd_msg->result = -1;
		return argv;
	}
	
	
	fp = fopen(SCAN_RESULT_FILE, "r");
	if(fp == NULL) {
		printf("can not open %s.\n", SCAN_RESULT_FILE);
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"sdcad test fail\n");
		sd_msg->result = -1;
		return argv;
	}

	
	/*if(stat("/res/images/wifi_scan.txt", &stat_ret) < 0) {
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
		ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"sdcard test success:%2fG\n",cap*1.0/1024/1024);

	return argv;
	
}
