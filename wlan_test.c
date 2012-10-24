/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "wltest"

#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/time.h>  // for utimes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>

#include <cutils/log.h>
#include "common.h"
#include "extra-functions.h"
#include "wlan_test.h"

#define LOG(x...) printf(x)

#define SCAN_RESULT_LENGTH 512
#define SCAN_RESULT_FILE "/data/scan_result.txt"

void strcpy_without_space_ahead(char *dst,char *src)
{
	char *tmp = dst;
	while( *src != '\0')
	{
		//printf("%d ",*src);
		if(*src!= ' ')
		{
			*dst++ = *src;
		}
		src++;
	}
	*dst = '\0';
	
}
// ---------------------------------------------------------------------------

void* wlan_test(void* argv)
{
	
	int ret;
	FILE *fp;
	char results[SCAN_RESULT_LENGTH];
	struct wlan_msg *wlan_msg = (struct wlan_msg *)argv;
	
	ret =  __system("busybox chmod 777 /sbin/wifi.sh");
	if(ret)
		LOG("chmod wifi.sh failed :%d\n",ret);
		
	ret = __system("/sbin/wifi.sh");
	if(ret <= 0) {
		LOG("wifi test failed.\n");
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"wlan test fail\n");
		wlan_msg->result = -1;
		return argv;
	}
	
	
	fp = fopen(SCAN_RESULT_FILE, "r");
	if(fp == NULL) {
		LOG("can not open %s.\n", SCAN_RESULT_FILE);
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"wlan test fail\n");
		wlan_msg->result = -1;
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
	
	//LOG("end.\n");
	strcpy_without_space_ahead(wlan_msg->ssid,results);
	ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"wlan test success:%s\n",wlan_msg->ssid);
	
		
    return 0;
}
