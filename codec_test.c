#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>

#include "alsa_audio.h"

#define AUDIO_HW_OUT_PERIOD_MULT 8 // (8 * 128 = 1024 frames)
#define AUDIO_HW_OUT_PERIOD_CNT 4
#define FILE_PATH "/res/codectest.pcm"
#define REC_DUR 3 //the unit is second
pthread_t codec_tid;  
int codec_err = -1;
extern pid_t g_codec_pid;

// 先放后录模式，测试效率相对低，使用喇叭时不会有啸叫，可在使用喇叭时选择此模式
void* rec_play_test_1(void *argv)
{		
	struct pcm* pcmIn = NULL;
	struct pcm* pcmOut = NULL;
	FILE *fp = NULL;
	unsigned bufsize;
	char *data;
	char recData[4*REC_DUR*44100];

	unsigned inFlags = PCM_IN;
	unsigned outFlags = PCM_OUT;
	unsigned flags =  PCM_STEREO;

	flags |= (AUDIO_HW_OUT_PERIOD_MULT - 1) << PCM_PERIOD_SZ_SHIFT;
	flags |= (AUDIO_HW_OUT_PERIOD_CNT - PCM_PERIOD_CNT_MIN)<< PCM_PERIOD_CNT_SHIFT;

	inFlags |= flags;
	outFlags |= flags;
	
	fp = fopen(FILE_PATH,"rb");
	if(NULL == fp)
	{
		fprintf(stderr,"could not open %s file, will go to fail\n", FILE_PATH);
		goto fail;
	}
				
	
	pcmOut = pcm_open(outFlags);
	if (!pcm_ready(pcmOut)) {
		fprintf(stderr,"the first time pcmOut open but the not ready,will go to fail\n");
		pcm_close(pcmOut);
		pcmOut = NULL;
		goto fail;
	}
	
	bufsize = pcm_buffer_size(pcmOut);
	usleep(10000);
repeat:	
	fseek(fp,0,SEEK_SET);
	
	while(bufsize == fread(recData,1,bufsize,fp)){
		if (pcm_write(pcmOut, recData, bufsize)) {
			fprintf(stderr,"the pcmOut could not write %d bytes file data,will go to fail\n", bufsize);
			goto fail;
		}
	}
	
	pcm_close(pcmOut);
	pcmOut = NULL;

	pcmIn = pcm_open(inFlags);
	if (!pcm_ready(pcmIn)) {
		fprintf(stderr,"the pcmIn open but the not ready,will go to fail\n");
		pcm_close(pcmIn);
		pcmIn = NULL;
		goto fail;
	}
	
	bufsize = pcm_buffer_size(pcmIn);
	usleep(10000);
	data = recData;
	while (!pcm_read(pcmIn, data, bufsize)) {
		data += bufsize;
		if(data + bufsize - recData >= 4*REC_DUR*44100)
		{
			pcm_close(pcmIn);
			pcmIn = NULL;
			break;
		}		
	}
	
	if(data + bufsize - recData < 4*REC_DUR*44100)
	{
			fprintf(stderr,"the pcmIn could not be read\n");
			goto fail;
	}

	pcmOut = pcm_open(outFlags);
	if (!pcm_ready(pcmOut)) {
		fprintf(stderr,"the second time pcmOut open but the not ready,will go to fail\n");
		pcm_close(pcmOut);
		pcmOut = NULL;
		goto fail;
	}
	
	bufsize = pcm_buffer_size(pcmOut);
	usleep(10000);
	data = recData;
	while (!pcm_write(pcmOut, data, bufsize)) {
		data += bufsize;
		if(data + bufsize - recData >= 4*REC_DUR*44100)
		{
			fprintf(stderr,"now goto repeat\n");
			goto repeat;
		}		
	}
	fprintf(stderr,"the pcmOut could not write record data ,will go to fail\n");
	
fail:
	if(pcmIn)
		pcm_close(pcmIn);
	if(pcmOut)
		pcm_close(pcmOut);
	if(fp)
		fclose(fp);
	return 0;
}

// 边录边放模式，测试效率高，使用喇叭时会有啸叫，可在使用耳机时选择此模式
void* rec_play_test_2(void *argv)
{		
	struct pcm* pcmIn;
	struct pcm* pcmOut;
	unsigned bufsize;
	char *data;

	unsigned inFlags = PCM_IN;
	unsigned outFlags = PCM_OUT;
	unsigned flags =  PCM_STEREO;

	flags |= (AUDIO_HW_OUT_PERIOD_MULT - 1) << PCM_PERIOD_SZ_SHIFT;
	flags |= (AUDIO_HW_OUT_PERIOD_CNT - PCM_PERIOD_CNT_MIN)<< PCM_PERIOD_CNT_SHIFT;

	inFlags |= flags;
	outFlags |= flags;

	pcmIn = pcm_open(inFlags);
	if (!pcm_ready(pcmIn)) {
	pcm_close(pcmIn);
	goto fail;
	}

	pcmOut = pcm_open(outFlags);
	if (!pcm_ready(pcmOut)) {
	pcm_close(pcmOut);
	goto fail;
	}

	bufsize = pcm_buffer_size(pcmIn);
	data = malloc(bufsize);
	if (!data) {
	fprintf(stderr,"could not allocate %d bytes\n", bufsize);
	return -1;
	}

	while (!pcm_read(pcmIn, data, bufsize)) {
	if (pcm_write(pcmOut, data, bufsize)) {
	fprintf(stderr,"could not write %d bytes\n", bufsize);
	return -1;
	}
	}

	fail:
	pcm_close(pcmIn);
	pcm_close(pcmOut);
	return 0;
}

void* codec_test(void *argv)
{
    int ret = -1;
    char dt[32] = {0};
    
	sleep(3);    
	
	if(script_fetch("Codec", "program", (int *)dt, 8) == 0) {
		printf("script_fetch program = %s.\n", dt);
	}	
	
/*
	codec_err = pthread_create(&codec_tid, NULL, rec_play_test,NULL); //
	if(codec_err != 0)
	{  
	   printf("create camera test thread error: %s/n",strerror(codec_err));  
	   
	} 
*/
/*
    printf ("+++++++++++++++++++ executing codec_test\r\n");
    ret = __system("/sbin/codec_test");	
    printf ("+++++++++++++++++++ ret %d , executing codec_test\r\n", ret);
*/
	pid_t codec_pid = vfork();
	if(codec_pid > 0){
		g_codec_pid = codec_pid;
	}else if(codec_pid == 0){
		execl("/sbin/codec_test", dt);
		exit(0);
	}
	printf("pcba-test codec pid %d\n",g_codec_pid);
    return NULL;
}
