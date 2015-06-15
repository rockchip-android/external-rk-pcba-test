#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/android_alarm.h>
#include <linux/rtc.h>
#include <termios.h>

#include <stdlib.h>
#include "common.h"
#include "at_util.h"
#include "script.h"
#include "test_case.h"


#define LOG(x...) printf("[AT_UTIL] "x)

static pthread_mutex_t s_commandmutex = PTHREAD_MUTEX_INITIALIZER;
extern int gFd;
static pthread_t s_tid_reader;
#define MAX_AT_RESPONSE 1024
static char s_ATBuffer[MAX_AT_RESPONSE+1];
static char *s_ATBufferCur = s_ATBuffer;
static int s_readCount = 0;

static char * findNextEOL(char *cur)
{
    if (cur[0] == '>' && cur[1] == ' ' && cur[2] == '\0') {
        /* SMS prompt character...not \r terminated */
        return cur+2;
    }

    // Find next newline
    while (*cur != '\0' && *cur != '\r' && *cur != '\n') cur++;

    return *cur == '\0' ? NULL : cur;
}

static const char *readline()
{
    int count;

    char *p_read = NULL;
    char *p_eol = NULL;
    char *ret;

    if (*s_ATBufferCur == '\0') {

        s_ATBufferCur = s_ATBuffer;
        *s_ATBufferCur = '\0';
        p_read = s_ATBuffer;
    } else { 
        while (*s_ATBufferCur == '\r' || *s_ATBufferCur == '\n')
            s_ATBufferCur++;

        p_eol = findNextEOL(s_ATBufferCur);

        if (p_eol == NULL) {
            size_t len;

            len = strlen(s_ATBufferCur);

            memmove(s_ATBuffer, s_ATBufferCur, len + 1);
            p_read = s_ATBuffer + len;
            s_ATBufferCur = s_ATBuffer;
        }
    }

    while (p_eol == NULL) {
        if (0 == MAX_AT_RESPONSE - (p_read - s_ATBuffer)) {
            //LOGE("ERROR: Input line exceeded buffer\n");
            s_ATBufferCur = s_ATBuffer;
            *s_ATBufferCur = '\0';
            p_read = s_ATBuffer;
        }

        do {
        	//ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"start to read\n");
            count = read(gFd, p_read,
                            MAX_AT_RESPONSE - (p_read - s_ATBuffer));
        } while (count < 0 && errno == EINTR);

        if (count > 0) {
            //AT_DUMP( "<< ", p_read, count );
            s_readCount += count;
//ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"count:%d\n",count);
            p_read[count] = '\0';

            while (*s_ATBufferCur == '\r' || *s_ATBufferCur == '\n')
                s_ATBufferCur++;

            p_eol = findNextEOL(s_ATBufferCur);
            p_read += count;
        } else if (count <= 0) {
            if(count == 0) {
                //LOGD("atchannel: EOF reached");
            } else {
                //LOGD("atchannel: read error %s", strerror(errno));
            }
            return NULL;
        }
    }


    ret = s_ATBufferCur;
    *p_eol = '\0';
    s_ATBufferCur = p_eol + 1; 

    //LOGD("AT< %s\n", ret);
    return ret;
}

static int isAtResponse = 0;
static int isResponseSuc = 0;
static int isAtSended = 0;
static char gAtAck_command[100]={"ERROR"};
static char ISMI[30]={"\0"};


static void *readerLoops(void *arg){
	 const char  *line;
	int count;
	char sread;
	int cur=0;
	while(1){
		line = readline();
		pthread_mutex_lock(&s_commandmutex);
		
		if(isAtSended){
			if(line != NULL){
				
				//ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"line:%s\n",line);
				
				if(strstr(line,gAtAck_command)!=NULL){
					isResponseSuc = 1;
					isAtResponse = 1;
					isAtSended =0;
				}else if(strstr(line,"ERROR")!=NULL){
					isResponseSuc = 0;
					isAtResponse = 1;
					isAtSended =0;
				}
	  		
	  		}
	 	 //else
	  	//	ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"line:err\n");
		}else{
				/*if(line != NULL){
					if(strlen(line)<20)				
					ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"line1:%s\n",line);
					else
						ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"line1:>20\n");
				}
			 	else
	  			ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"line1:err\n");*/
		}
		
		pthread_mutex_unlock(&s_commandmutex);


	}
	return NULL;
}

int atcmd_send(int fd,char *send_command,char *ack_command)
{
	LOG("atcmd_send::send_command=%s\n", send_command);
	int cur = 0;
	int len = strlen(send_command);
	int written;
	int i = 3;
	int j=0;
for(j=0;j<3;j++){
	
	pthread_mutex_lock(&s_commandmutex);
	
	isResponseSuc = 0;
	isAtResponse = 0;
	isAtSended =0;	
	strcpy(gAtAck_command,ack_command);
	cur = 0;
	while (cur < len) {
		written = write (fd, send_command + cur, len - cur);
		if (written < 0) {
			//ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,100,"at_send_error\n");
			pthread_mutex_unlock(&s_commandmutex);
			return -1;
		}
		cur += written;
	}
	isAtSended = 1;
	
	pthread_mutex_unlock(&s_commandmutex);
	
	i = 3;
  while(1){
  	sleep(1);
  	if(isAtResponse && isResponseSuc ){
  		isResponseSuc = 0;
		isAtResponse = 0;
		isAtSended =0;
  		return 1;
  	}else  if(isAtResponse && isResponseSuc==0){
  		isResponseSuc = 0;
			isAtResponse = 0;
			isAtSended =0;
  		break;
  	}
  	
  	i--;
  	if(i<=0){
  		isResponseSuc = 0;
			isAtResponse = 0;
			isAtSended =0;
  		break;
  	}
  }
}
	return -1;
}

int send_at_cmd(char *send_command,char *ack_command)
{
	pthread_t tid;
	pthread_attr_t attr;
	int serial_fd;
	int err;
	int biID =-1;
	int loop = 3;

	if(gFd <= 0) {
	    do{
			serial_fd = open("/dev/mvpipe-atc",O_RDWR );
			if(serial_fd < 0)
			{
				sleep(1);
			}
			loop --;
			if(loop<0)break;
		}while(serial_fd < 0);
	} else {
		serial_fd = gFd;
	}


	if(serial_fd <0)
	{
		LOG("%s line=%d open at port fail!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	if ( serial_fd >= 0 ) {
		struct termios  ios;
		tcgetattr( serial_fd, &ios );
		ios.c_lflag = 0;
		if(biID == 13){ 
			ios.c_cflag |= CRTSCTS;
			cfsetispeed(&ios, B460800);
			cfsetospeed(&ios, B460800);
		}else if(biID == 14){
			ios.c_cflag |= CRTSCTS;
			cfsetispeed(&ios, B115200);
                        cfsetospeed(&ios, B115200);
		}else{
			cfsetispeed(&ios, B115200);
			cfsetospeed(&ios, B115200);
		}
		cfmakeraw(&ios);		 		
		tcsetattr( serial_fd, TCSANOW, &ios );
		
        gFd = serial_fd;     
    
        pthread_attr_init (&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	    if(pthread_create(&s_tid_reader, &attr, readerLoops, &attr)<0)
	    {
	     	LOG("%s line=%d pthread_create err\n", __FUNCTION__, __LINE__);
			close(serial_fd);
			return -1;
	    }
     	//while(1){sleep(5);write (gFd, "AT+CPIN?\r\n", 10);}
	}
	if(atcmd_send(serial_fd,send_command,ack_command) < 0){
		LOG("%s line=%d send_at_cmd failed !\n", __FUNCTION__, __LINE__);
		close(serial_fd);
		return -1;
	}

	return 1;

}

