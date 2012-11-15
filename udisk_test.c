#include<stdio.h>
#include <stdlib.h>

#include"common.h"
#include"extra-functions.h"

#include"udisk_test.h"

#define SCAN_RESULT_LENGTH 128
#define SCAN_RESULT_FILE "/data/udisk_capacity.txt"

void * udisk_test(void * argv)
{
	
	struct udisk_msg *udisk_msg = (struct udisk_msg *)malloc(sizeof(struct udisk_msg));
	int ret;
	double cap;
	FILE *fp;
	char results[SCAN_RESULT_LENGTH];
	
	ret =  __system("busybox chmod 777 /sbin/udisktester.sh");
	if(ret)
		printf("chmod udisktester.sh failed :%d\n",ret);
		
	ret = __system("/sbin/udisktester.sh");
	if(ret < 0) {
		printf("udisk test failed.\n");
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"udisk test script fail\n");
		udisk_msg->result = -1;
		return argv;
	}
	
	
	fp = fopen(SCAN_RESULT_FILE, "r");
	if(fp == NULL) {
		printf("can not open %s.\n", SCAN_RESULT_FILE);
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"udisk test fail read result\n");
		udisk_msg->result = -1;
		return argv;
	}

	
  	memset(results, 0, SCAN_RESULT_LENGTH);
	fgets(results,50,fp);
	
	cap = strtod(results,NULL);
    	printf("capacity : %s\n", results);
	if(cap > 0)
		ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"udisk test success:%2fG\n",cap*1.0/1024/1024);
    	else
        	ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"udisk test failed!!!");

	return argv;
	
}
