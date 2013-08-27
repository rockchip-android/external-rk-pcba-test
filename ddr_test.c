#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "extra-functions.h"
#include "language.h"

#define MAX_TEST 255
#define MAX_USE_RATE 0.850000
#define DDR_TXT "/data/get_ddr_msg.txt"

#define GET_TOTAL_SIZE  0x01
#define GET_USE_RATE	0x02
#define GET_FREE_RATE	0x03

int get_data(char *buf,char *data)
{
	char *p1 =NULL,* p2=NULL;
	int num =0;
	
        p1 = buf;
        p2 = strstr(p1,data);
        if(p2 == NULL)
        {
                printf("%s no find  %s --- %s\r\n",__FUNCTION__ ,buf,data);
                return 0;
        }

        p1 = p2+strlen(data);
        while(1)
        {
                if(p1[0] != 0x20)
                        break;

                p1++;
        }

        num = atoi(p1);

	return num; 
}

float get_ddr_msg(int state)
{
	FILE *fp = NULL;
        char command[1024],buf[1024];
        int num=0,len=0,ddr_free = 0,ddr_used = 0;
	float result;

        memset(command,0,sizeof(command));
        sprintf(command,"busybox top -n1 | busybox grep Mem: > %s" , DDR_TXT);        
	__system(command);

        fp = fopen(DDR_TXT,"r");
        if(fp == NULL)
        {
                printf("%s open err\r\n",__FUNCTION__);
                return -1;
        }

        memset(buf,0,sizeof(buf));
        len = fread(buf, 1,sizeof(buf), fp);
        if(len<=0)
        {
                printf("%s open err\r\n",__FUNCTION__);
                return -1;
        }
	
	ddr_used = get_data(buf,"Mem:");
	ddr_free = get_data(buf,"used,");
	
	if(ddr_free == 0 || ddr_used ==0)
		return -1;
  
        fclose(fp);

	switch(state)
	{
		case GET_TOTAL_SIZE:
			return((float)ddr_used + (float)ddr_free); 
		
		case GET_USE_RATE:
        		return ((float)ddr_used/((float)ddr_used+(float)ddr_free));

		case GET_FREE_RATE:
			break;

		default:
			break;
	}

        return -1;
}


int get_block_size(int total_size)
{
	if(total_size < 256*1024)
		return -1;
	
	return ((total_size/256)*5);
}


void *ddr_test(void *argv)
{
	char *ddr_data1[MAX_TEST];
	char *ddr_data2[MAX_TEST];
	int val = 0, num =0,result = 0,i = 0,block_size = 0;
	float use_rate = 0,total_size = 0;
	
	/*Get ddr total size*/
	total_size = get_ddr_msg(GET_TOTAL_SIZE);

	/*According to the different size calculation for each application space*/
	block_size = get_block_size((int)total_size); 
	if(block_size <= 0)
	{	
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"%s:[%s] \n",PCBA_DDR,PCBA_FAILED);
		return -1;
	}
	
	while(1)
	{	
		if(num >= MAX_TEST)
			break;

		use_rate = get_ddr_msg(GET_USE_RATE);
		if(use_rate >= MAX_USE_RATE)
		{
			printf("%s : test ok :%f\r\n",__FUNCTION__,use_rate);
			break;
		}

		ddr_data1[num] = calloc(1,block_size*1024);
		if(ddr_data1[num] == NULL)
			break;
		
		ddr_data2[num] = calloc(1,block_size*1024);
		if(ddr_data2[num] == NULL)
			break;

		memset(ddr_data1[num],num,block_size*1024);
		memset(ddr_data2[num],num,block_size*1024);

		if(strcmp(ddr_data1[num],ddr_data2[num]) != 0)
		{	
			result = -1;
			break;
		}	
		num++;	
	}

	for(i = 0; i< num; i++)
	{	
		if(ddr_data1[i])
			free(ddr_data1[i]);

		if(ddr_data2[i])
			free(ddr_data2[i]);
	}

	if(result < 0)	
		ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"%s:[%s] \n",PCBA_DDR,PCBA_FAILED);
	else
		ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"%s:[%s] \n",PCBA_DDR,PCBA_SECCESS);
		
	return 0;

}

