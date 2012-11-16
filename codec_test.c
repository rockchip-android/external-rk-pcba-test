#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#include "alsa_audio.h"

#define AUDIO_HW_OUT_PERIOD_MULT 8 // (8 * 128 = 1024 frames)
#define AUDIO_HW_OUT_PERIOD_CNT 4
#define FILE_PATH "/sbin/codectest.pcm"
#define REC_DUR 3 //the unit is second
pthread_t codec_tid;  
int codec_err = -1;

void* rec_play_test(void *argv)
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
			goto fail;
		
	
	
	pcmOut = pcm_open(outFlags);
	if (!pcm_ready(pcmOut)) {
		pcm_close(pcmOut);
		pcmOut = NULL;
		goto fail;
	}
	
	bufsize = pcm_buffer_size(pcmOut);
	
repeat:	
	fseek(fp,0,SEEK_SET);
	
	while(bufsize == fread(recData,1,bufsize,fp)){
		if (pcm_write(pcmOut, recData, bufsize)) {
		fprintf(stderr,"could not write %d bytes\n", bufsize);
		goto fail;
		}
	}
	
	pcm_close(pcmOut);
	pcmOut = NULL;

	pcmIn = pcm_open(inFlags);
	if (!pcm_ready(pcmIn)) {
		pcm_close(pcmIn);
		pcmIn = NULL;
		goto fail;
	}
	
	bufsize = pcm_buffer_size(pcmIn);
	
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

	pcmOut = pcm_open(outFlags);
	if (!pcm_ready(pcmOut)) {
		pcm_close(pcmOut);
		pcmOut = NULL;
		goto fail;
	}
	
	bufsize = pcm_buffer_size(pcmOut);

	data = recData;
	while (!pcm_write(pcmOut, data, bufsize)) {
		data += bufsize;
		if(data + bufsize - recData >= 4*REC_DUR*44100)
		{
			fprintf(stderr,"now goto repeat\n");
			goto repeat;
		}		
	}
	
fail:
	if(pcmIn)
		pcm_close(pcmIn);
	if(pcmOut)
		pcm_close(pcmOut);
	if(fp)
		fclose(fp);
	return 0;
}

void* codec_test(void *argv)
{
	sleep(3);    
	codec_err = pthread_create(&codec_tid, NULL, rec_play_test,NULL); //
	if(codec_err != 0)
	{  
	   printf("create camera test thread error: %s/n",strerror(codec_err));  
	   
	} 
	
    return NULL;
}
