#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#include "alsa_audio.h"

#define AUDIO_HW_OUT_PERIOD_MULT 8 // (8 * 128 = 1024 frames)
#define AUDIO_HW_OUT_PERIOD_CNT 4

pthread_t codec_tid;  
int codec_err = -1;

void* rec_play_test(void *argv)
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

int codec_test(void *argv)
{
	sleep(3);    
	codec_err = pthread_create(&codec_tid, NULL, rec_play_test,NULL); //
	if(codec_err != 0)
	{  
	   printf("create camera test thread error: %s/n",strerror(codec_err));  
	   
	} 
	
    return 0;
}
