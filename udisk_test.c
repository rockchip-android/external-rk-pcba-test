#include<stdio.h>
#include <stdlib.h>

#include"common.h"
#include"extra-functions.h"

#include"udisk_test.h"
#include"test_case.h"

#define SCAN_RESULT_LENGTH 128
#define SCAN_RESULT_FILE "/data/udisk_capacity.txt"

void * udisk_test(void * argv)
{
	
	struct testcase_info *tc_info = (struct testcase_info*)argv;
	int ret;
	double cap;
	FILE *fp;
	char results[SCAN_RESULT_LENGTH];
	
	ret =  __system("busybox chmod 777 /res/udisktester.sh");
	if(ret)
		printf("chmod udisktester.sh failed :%d\n",ret);
		
	ret = __system("/res/udisktester.sh");
	if(ret < 0) {
		printf("udisk test failed.\n");
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"Udisk : [FAIL]\n");
		tc_info->result = -1;
		return argv;
	}
	
	
	fp = fopen(SCAN_RESULT_FILE, "r");
	if(fp == NULL) {
		printf("can not open %s.\n", SCAN_RESULT_FILE);
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"Udisk : [FAIL]\n");
		tc_info->result = -1;
		return argv;
	}

	
  	memset(results, 0, SCAN_RESULT_LENGTH);
	fgets(results,50,fp);
	
	cap = strtod(results,NULL);
    printf("capacity : %s\n", results);
	if(cap > 0)
		ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"Udisk : [OK] {%2fG}\n",cap*1.0/1024/1024);
    else
        ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"Udisk : [FAIL]");

	return argv;
	
}
