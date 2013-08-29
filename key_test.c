#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <linux/input.h>
#include <fcntl.h>
#include <dirent.h>
#include "common.h"
#include "key_test.h"
#include "test_case.h"
#include "language.h"
#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))

uint8_t keyBitmask[(KEY_MAX + 1) / 8];
struct key key_code[KEY_MAX];
uint8_t key_cnt = 0; //key counter
unsigned int gKey = 0;
static pthread_mutex_t gKeyMutex = PTHREAD_MUTEX_INITIALIZER;

int g_key_test = 0;
static struct testcase_info  *tc_info = NULL;
extern int manual_p_y;

int set_gKey(unsigned int code)
{
	int i;
	static n = 0;
	 for(i=0; i < key_cnt;i++)
	{
		if(code == key_code[i].code)
		{
//			ui_print_xy_rgba(tc_info->x,tc_info->y + 1,0,0,255,255,"    [ %s ]\n",key_code[i].name);
//                        ui_print_xy_rgba(tc_info->x,tc_info->y + 1,0,255,0,255,"%s\n",key_code[i].name);
                        ui_print_xy_rgba(0,tc_info->y,0,255,0,255,"%s:[%s]\n",PCBA_KEY,key_code[i].name);
			break;
		}
	}
//	 if(code == KEY_POWER)
//	 	 g_key_test = 0;
	 
	return 0;
}

int scan_key_code()
{
	unsigned temp = 0;
	pthread_mutex_lock(&gKeyMutex);
	if(gKey)
	{
		temp = gKey;
		gKey = 0;
	}
	pthread_mutex_unlock(&gKeyMutex);

	return temp;
}
 int key_code_probe(void)
 {
	 DIR *dir;
	 struct dirent *de;
	 int fd;
	 char name[80];
	 int i;
	 dir = opendir("/dev/input");
	 if(dir != 0)
	 {
		 while((de = readdir(dir))) {
 // 		   fprintf(stderr,"/dev/input/%s\n", de->d_name);
			 if(strncmp(de->d_name,"event",5)) 
				 continue;
			 fd = openat(dirfd(dir), de->d_name, O_RDONLY);
			 if(fd < 0) 
			 	continue;
			 else
			 {
				 if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1)
				 {
					 name[0] = '\0';
				 }
				 if (!strcmp(name, "gsensor")) //do not open gsensor here
					 continue;
				 ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBitmask)),keyBitmask);
				 for(i =0 ;i< KEY_MAX;i++)
				 {
				 	if(test_bit(i, keyBitmask))
				 	{
						if(i != KEY_WAKEUP)
						{
							key_code[key_cnt].code = i;
							switch (i)
							{
								case KEY_VOLUMEDOWN:
									key_code[key_cnt].name = "vol-";
									break;
								case KEY_VOLUMEUP:
									 key_code[key_cnt].name = "vol+";
									 break;
								case KEY_POWER:
									 key_code[key_cnt].name = "power";
									 break;
								case KEY_F1:
									 key_code[key_cnt].name = "menu";
									 break;
								case KEY_HOME:
									 key_code[key_cnt].name = "home";
									 break;
								case KEY_BACK:
									 key_code[key_cnt].name = "ESC";
									 break;
								default:
									 printf("un supported key:%d\n",i);
									 break;
									 
							}
							//printf("support key code:%d\n",i);
							key_cnt++;
						}
						
					}
						
				 }
			 }
			 
 
			
		 }
	 }
 
	 return 0;
 }

void* key_test(void *argc)
{
	int i = 0;
	int code;
	int run = 1;
	//printf("start key test\n");
	tc_info  = (struct testcase_info *)argc;

	if(tc_info->y <= 0)
		tc_info->y  = get_cur_print_y();	

        ui_print_xy_rgba(0,tc_info->y,255,255,0,255,"%s \n",PCBA_KEY);
	key_code_probe();
	g_key_test = 1;
		
	return NULL;
}
