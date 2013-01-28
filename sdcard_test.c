#include<stdio.h>
#include <stdlib.h>

#include"extra-functions.h"
#include"common.h"
#include"sdcard_test.h"
#include "test_case.h"

#define SCAN_RESULT_LENGTH 128
#define SCAN_RESULT_FILE "/data/sd_capacity"

void * sdcard_test(void * argv)
{
	
	struct testcase_info *tc_info = (struct testcase_info*)argv;
	int ret;
	double cap;
	FILE *fp;
	char results[SCAN_RESULT_LENGTH];
	
	ret =  __system("busybox chmod 777 /res/mmctester.sh");
	if(ret)
		printf("chmod mmctester.sh failed :%d\n",ret);
		
	ret = __system("/res/mmctester.sh");
	if(ret < 0) {
		printf("mmc test failed.\n");
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"SDCard: [FAIL]\n");
		tc_info->result = -1;
		return argv;
	}
	
	
	fp = fopen(SCAN_RESULT_FILE, "r");
	if(fp == NULL) {
		printf("can not open %s.\n", SCAN_RESULT_FILE);
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"SDCard: [FAIL]\n");
		tc_info->result = -1;
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
		ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"SDCard: [OK] {%2fG}\n",cap*1.0/1024/1024);

	return argv;
	
}
