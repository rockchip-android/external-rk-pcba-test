#include "camera_test.h"
#include "./minuitwrp/minui.h"
#include "test_case.h"
#define VIDEO_DEV_NAME   "/dev/video0"
#define PMEM_DEV_NAME    "/dev/pmem_cam"
#define DISP_DEV_NAME    "/dev/graphics/fb1"
#define ION_DEVICE          "/dev/ion"

#define FBIOSET_ENABLE			0x5019	


#define CAM_OVERLAY_BUF_NEW  1
#define RK29_CAM_VERSION_CODE_1 KERNEL_VERSION(0, 0, 1)
#define RK29_CAM_VERSION_CODE_2 KERNEL_VERSION(0, 0, 2)

static void *m_v4l2Buffer[4];
static int v4l2Buffer_phy_addr;
static int iCamFd, iPmemFd, iDispFd;
static int preview_w,preview_h;

static char videodevice[20] ={0};
static struct v4l2_capability mCamDriverCapability;
static unsigned int pix_format;

static void* vaddr = NULL;
static volatile int isstoped = 0;
static int hasstoped = 1;
enum {
	FD_INIT = -1,
};

static int iIonFd = -1;
struct ion_allocation_data ionAllocData;
struct ion_fd_data fd_data;
struct ion_handle_data handle_data;
#define RK30_PLAT 1
#define RK29_PLAT 0
static int is_rk30_plat = RK30_PLAT;
#define  FB_NONSTAND ((is_rk30_plat == RK29_PLAT)?0x2:0x20)




int CameraCreate(void)
{
    int err,size;
	struct pmem_region sub;
	struct v4l2_format format;

    if (iCamFd == 0) {
        iCamFd = open(videodevice, O_RDWR);
       
        if (iCamFd < 0) {
            printf(" Could not open the camera  device:%s\n",videodevice);
    		err = -1;
            goto exit;
        }

        memset(&mCamDriverCapability, 0, sizeof(struct v4l2_capability));
        err = ioctl(iCamFd, VIDIOC_QUERYCAP, &mCamDriverCapability);
        if (err < 0) {
        	printf("Error opening device unable to query device.\n");
    	    goto exit;
        } 
				if(strstr((char*)&mCamDriverCapability, "front") != NULL){
					printf("it is a front camera \n!");
					}
				else if(strstr((char*)&mCamDriverCapability, "back") != NULL){
					printf("it is a back camera \n!"); 
				}
				else{
					printf("it is a usb camera \n!");
					}
        if (mCamDriverCapability.version == RK29_CAM_VERSION_CODE_1) {
            pix_format = V4L2_PIX_FMT_YUV420;
            printf("Current camera driver version: 0.0.1 \n");    
        } else 
        { 
            pix_format = V4L2_PIX_FMT_NV12;
            printf("Current camera driver version: %d.%d.%d \n",                
                (mCamDriverCapability.version>>16) & 0xff,(mCamDriverCapability.version>>8) & 0xff,
                mCamDriverCapability.version & 0xff); 
        }
        
    }
    if(access("/sys/module/rk29_camera_oneframe", O_RDWR) >=0 ){
        is_rk30_plat =  RK29_PLAT;
        printf("it is rk29 platform!\n");
    }else if(access("/sys/module/rk30_camera_oneframe", O_RDWR) >=0){
        printf("it is rk30 platform!\n");
    }else{
        printf("default as rk30 platform\n");
    }
    if(access(PMEM_DEV_NAME, O_RDWR) < 0) {
            iIonFd = open(ION_DEVICE, O_RDONLY);

            if(iIonFd < 0 ) {
                printf("%s: Failed to open ion device - %s",
                        __FUNCTION__, strerror(errno));
                iIonFd = -1;
        		err = -1;
                goto exit1;
            }
            ionAllocData.len = 0x200000;
            ionAllocData.align = 4*1024;
            ionAllocData.flags = 1 << 0;
              err = ioctl(iIonFd, ION_IOC_ALLOC, &ionAllocData);
            if(err) {
                printf("%s: ION_IOC_ALLOC failed to alloc 0x%x bytes with error - %s", 
        			__FUNCTION__, ionAllocData.len, strerror(errno));
                
        		err = -errno;
                goto exit2;
            }

            fd_data.handle = ionAllocData.handle;
            handle_data.handle = ionAllocData.handle;

            err = ioctl(iIonFd, ION_IOC_MAP, &fd_data);
            if(err) {
                printf("%s: ION_IOC_MAP failed with error - %s",
                        __FUNCTION__, strerror(errno));
                ioctl(iIonFd, ION_IOC_FREE, &handle_data);
        		err = -errno;
               goto exit2;
            }
            m_v4l2Buffer[0] = mmap(0, ionAllocData.len, PROT_READ|PROT_WRITE,
                    MAP_SHARED, fd_data.fd, 0);
            if(m_v4l2Buffer[0] == MAP_FAILED) {
                printf("%s: Failed to map the allocated memory: %s",
                        __FUNCTION__, strerror(errno));
                err = -errno;
                ioctl(iIonFd, ION_IOC_FREE, &handle_data);
                goto exit2;
            }
        	err = ioctl(fd_data.fd, PMEM_GET_PHYS, &sub);
        	if (err < 0) {
            	printf(" PMEM_GET_PHY_ADDR failed, limp mode\n");
                ioctl(iIonFd, ION_IOC_FREE, &handle_data);
                goto exit2;
        	}
              err = ioctl(iIonFd, ION_IOC_FREE, &handle_data);
        	if(err){
        		printf("%s: ION_IOC_FREE failed with error - %s",
                        __FUNCTION__, strerror(errno));
        		err = -errno;
        	}else
            	printf("%s: Successfully allocated 0x%x bytes, mIonFd=%d, SharedFd=%d",
            			__FUNCTION__,ionAllocData.len, iIonFd, fd_data.fd);
        }else{
            iPmemFd = open(PMEM_DEV_NAME, O_RDWR, 0);
            if (iPmemFd < 0) {
            	printf(" Could not open pmem device(%s)\n",PMEM_DEV_NAME);
        		err = -1;
                goto exit1;
            }

        	size = sub.len = 0x200000; 
        	m_v4l2Buffer[0] =(unsigned char *) mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, iPmemFd, 0);
        	if (m_v4l2Buffer[0] == MAP_FAILED) {
            	printf(" m_v4l2Buffer[0] mmap failed\n");
        		err = -1;
                goto exit2;
        	}
        	err = ioctl(iPmemFd, PMEM_GET_PHYS, &sub);
        	if (err < 0) {
            	printf(" PMEM_GET_PHY_ADDR failed, limp mode\n");
                goto exit3;
        	}
        }
    memset(m_v4l2Buffer[0], 0x00, size);
	v4l2Buffer_phy_addr = sub.offset;
   
          err = ioctl(iCamFd, VIDIOC_QUERYCAP, &mCamDriverCapability);
        if (err < 0) {
        	printf("Error opening device unable to query device.\n");
    	    goto exit;
        }  
    return 0;

exit3:
	munmap(m_v4l2Buffer[0], size);
exit2:
    if(iPmemFd > 0){
    	close(iPmemFd);
    	iPmemFd = -1;
        }
    if(iIonFd > 0){
    	close(iIonFd);
    	iIonFd = -1;
        }
exit1:
exit:
    return err;
}

int CameraStart(int phy_addr, int buffer_count, int w, int h)
{
    int err,i;
    int nSizeBytes;
    struct v4l2_format format;
    enum v4l2_buf_type type;
    struct v4l2_requestbuffers creqbuf;
		
		isstoped = 0;
		hasstoped = 0;
	//buffer_count = 2;
	if( phy_addr == 0 || buffer_count == 0  ) {
    	printf(" Video Buf is NULL\n");
		goto  fail_bufalloc;
    }

	/* Set preview format */
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.width = w;
	format.fmt.pix.height = h;
	format.fmt.pix.pixelformat = pix_format;
	format.fmt.pix.field = V4L2_FIELD_NONE;	
	err = ioctl(iCamFd, VIDIOC_S_FMT, &format);
	if ( err < 0 ){
		printf(" Failed to set VIDIOC_S_FMT\n");
		goto exit1;
	}

	preview_w = format.fmt.pix.width;
	preview_h = format.fmt.pix.height;	
	creqbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    creqbuf.memory = V4L2_MEMORY_OVERLAY;
    creqbuf.count  =  buffer_count /*- 1*/ ; //We will use the last buffer for snapshots.
    if (ioctl(iCamFd, VIDIOC_REQBUFS, &creqbuf) < 0) {
        printf("%s VIDIOC_REQBUFS Failed\n",__FUNCTION__);
        goto fail_reqbufs;
    }
	printf("creqbuf.count = %d\n",creqbuf.count);
    for (i = 0; i < (int)creqbuf.count; i++) {

        struct v4l2_buffer buffer;
        buffer.type = creqbuf.type;
        buffer.memory = creqbuf.memory;
        buffer.index = i;

        if (ioctl(iCamFd, VIDIOC_QUERYBUF, &buffer) < 0) {
            printf("%s VIDIOC_QUERYBUF Failed\n",__FUNCTION__);
            goto fail_loop;
        }

        #if CAM_OVERLAY_BUF_NEW
        buffer.m.offset = phy_addr + i*buffer.length;
        #else
        buffer.m.offset = phy_addr;
        #endif

        m_v4l2Buffer[i] =(void*)((int)m_v4l2Buffer[0] + i*buffer.length);
	memset(m_v4l2Buffer[i],0x0,buffer.length);
        err = ioctl(iCamFd, VIDIOC_QBUF, &buffer);
        if (err < 0) {
            printf("%s CameraStart VIDIOC_QBUF Failed\n",__FUNCTION__);
            goto fail_loop;
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    err = ioctl(iCamFd, VIDIOC_STREAMON, &type);
    if ( err < 0) {
        printf("%s VIDIOC_STREAMON Failed\n",__FUNCTION__);
        goto fail_loop;
    }

    return 0;

fail_bufalloc:
fail_loop:
fail_reqbufs:

exit1:
    close(iCamFd);
	iCamFd = -1;
exit:
    return -1;
}

int CameraStop()
{
	struct v4l2_requestbuffers creqbuf;

	creqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(iCamFd, VIDIOC_STREAMOFF, &creqbuf.type) == -1) {
        printf("%s VIDIOC_STREAMOFF Failed", __FUNCTION__);
        return -1;
    }

	return 0;

}



int DispCreate(int corx ,int cory,int preview_w,int preview_h )
{
	int err;
	struct fb_var_screeninfo var;
	unsigned int panelsize[2];
	int x_phy,y_phy,w_phy,h_phy;
	int x_visual,y_visual,w_visual,h_visual;
	struct fb_fix_screeninfo finfo;
	int data[2];
	iDispFd = open(DISP_DEV_NAME,O_RDWR, 0);
	if (iDispFd < 0) {
		printf("%s Could not open display device\n",__FUNCTION__);
		err = -1;
		goto exit;
	}
	if(ioctl(iDispFd, 0x5001, panelsize) < 0)
	{
		printf("%s Failed to get panel size\n",__FUNCTION__);
		err = -1;
		goto exit1;
	}
	if(panelsize[0] == 0 || panelsize[1] ==0)
	{
		panelsize[0] = preview_w;
		panelsize[1] = preview_h;
	}
	data[0] = v4l2Buffer_phy_addr;
	data[1] = (int)(data[0] + preview_w *preview_h);
	if (ioctl(iDispFd, 0x5002, data) == -1) 
	{
		printf("%s ioctl fb1 queuebuf fail!\n",__FUNCTION__);
		err = -1;
		goto exit;
	}
	if (ioctl(iDispFd, FBIOGET_VSCREENINFO, &var) == -1) {
		printf("%s ioctl fb1 FBIOPUT_VSCREENINFO fail!\n",__FUNCTION__);
		err = -1;
		goto exit;
	}
	//printf("preview_w = %d,preview_h =%d,panelsize[1] = %d,panelsize[0] = %d\n",preview_w,preview_h,panelsize[1],panelsize[0]);
	//var.xres_virtual = preview_w;	//win0 memery x size
	//var.yres_virtual = preview_h;	 //win0 memery y size
	var.xoffset = 0;   //win0 start x in memery
	var.yoffset = 0;   //win0 start y in memery
	var.nonstd = ((cory<<20)&0xfff00000) + ((corx<<8)&0xfff00) +FB_NONSTAND; //win0 ypos & xpos & format (ypos<<20 + xpos<<8 + format)
	var.grayscale = ((preview_h<<20)&0xfff00000) + (( preview_w<<8)&0xfff00) + 0;	//win0 xsize & ysize
	var.xres = preview_w;	 //win0 show x size
	var.yres = preview_h;	 //win0 show y size
	var.bits_per_pixel = 16;
	var.activate = FB_ACTIVATE_FORCE;
	if (ioctl(iDispFd, FBIOPUT_VSCREENINFO, &var) == -1) {
		printf("%s ioctl fb1 FBIOPUT_VSCREENINFO fail!\n",__FUNCTION__);
		err = -1;
		goto exit;
	}

	return 0;
exit1:
	if (iDispFd > 0)
	{
		close(iDispFd);
		iDispFd = -1;
	}
exit:
	return err;
}
int TaskStop(void)
{
	struct v4l2_requestbuffers creqbuf;
    creqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    isstoped =1;
    while(!hasstoped){
    	sleep(1);
    	}
    if (ioctl(iCamFd, VIDIOC_STREAMOFF, &creqbuf.type) == -1) {
        printf("%s VIDIOC_STREAMOFF Failed\n", __FUNCTION__);
        return -1;
    }
	
	if (iCamFd > 0) {
		close(iCamFd);
		iCamFd = 0;
	}

	if (iPmemFd > 0) {
		close(iPmemFd);
		iPmemFd = -1;
	}

    if(iIonFd > 0){
        munmap(m_v4l2Buffer[0], ionAllocData.len);

    	close(iIonFd);
    	iIonFd = -1;
        }

	if (iDispFd > 0) {
        int disable = 0;
		printf("Close disp\n");
        ioctl(iDispFd, FBIOSET_ENABLE,&disable);
		close(iDispFd);
		iDispFd = -1;
	}
	return 0;
}

int TaskRuning(int fps_total,int corx,int cory)
{
	int err,fps;
	int data[2];
	struct v4l2_buffer cfilledbuffer1;
	int i ;
	struct fb_var_screeninfo var ;
	int fb_offset = 0;
	cfilledbuffer1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	cfilledbuffer1.memory = V4L2_MEMORY_OVERLAY;
	cfilledbuffer1.reserved = 0;
	fps = 0;
	while (!isstoped)
	{
		if (ioctl(iCamFd, VIDIOC_DQBUF, &cfilledbuffer1) < 0)
		{
			printf("%s VIDIOC_DQBUF Failed!!! \n",__FUNCTION__);
			err = -1;
			goto exit;
		}		
		if (iDispFd > 0) 
		{	
#if CAM_OVERLAY_BUF_NEW
			data[0] = (int)cfilledbuffer1.m.offset;  
#else
			data[0] = (int)cfilledbuffer1.m.offset + cfilledbuffer1.index * cfilledbuffer1.length;
#endif
			data[1] = (int)(data[0] + preview_w *preview_h);
			//  		for(i = 0;i < 100;i++){
			// 			printf("0x%x ",*((char*)(m_v4l2Buffer[cfilledbuffer1.index])+i));
			//  			}
			//printf("y_addr = 0x%x,length = %d\n",data[0],cfilledbuffer1.length);
			if (ioctl(iDispFd, 0x5002, data) == -1) {
			       printf("%s ioctl fb1 queuebuf fail!\n",__FUNCTION__);
			       err = -1;
				goto exit;
			}
			if (ioctl(iDispFd, FBIOGET_VSCREENINFO, &var) == -1) {
				printf("%s ioctl fb1 FBIOPUT_VSCREENINFO fail!\n",__FUNCTION__);
				err = -1;
				goto exit;
			}
			//printf("preview_w = %d,preview_h =%d,panelsize[1] = %d,panelsize[0] = %d\n",preview_w,preview_h,panelsize[1],panelsize[0]);
			var.xres_virtual = preview_w;	//win0 memery x size
			var.yres_virtual = preview_h;	 //win0 memery y size
			var.xoffset = 0;   //win0 start x in memery
			var.yoffset = 0;   //win0 start y in memery
			var.nonstd = ((cory<<20)&0xfff00000) + (( corx<<8)&0xfff00) +FB_NONSTAND;
			var.grayscale = ((preview_h<<20)&0xfff00000) + (( preview_w<<8)&0xfff00) + 0;   //win0 xsize & ysize
			var.xres = preview_w;	 //win0 show x size
			var.yres = preview_h;	 //win0 show y size
			var.bits_per_pixel = 16;
			var.activate = FB_ACTIVATE_FORCE;
			if (ioctl(iDispFd, FBIOPUT_VSCREENINFO, &var) == -1) {
				printf("%s ioctl fb1 FBIOPUT_VSCREENINFO fail!\n",__FUNCTION__);
				err = -1;
				goto exit;
			}

		}
	if (ioctl(iCamFd, VIDIOC_QBUF, &cfilledbuffer1) < 0) {
		printf("%s VIDIOC_QBUF Failed!!!\n",__FUNCTION__);
		err = -1;
		goto exit;
	}

	    fps++;
	}
	hasstoped = 1;

exit:
	return err;
}
// the func is a while loop func , MUST  run in a single thread.
void * startCameraTest(void *argv){
	int ret = 0;
	int cameraId;
	int preWidth;
	int preHeight;
	int corx ;
	int cory;
	struct camera_msg *camera_msg = (struct camera_msg*)argv;

	if(!camera_msg)
	{
		printf("malloc camera_msg fail\n");
		return NULL;
	}
	cameraId = camera_msg->id;
	preWidth = camera_msg->w;
	preHeight = camera_msg->h;
	corx = camera_msg->x;
	cory = camera_msg->y;
	sprintf(videodevice,"/dev/video%d",cameraId);
	preview_w = preWidth;
	preview_h = preHeight;
	if (CameraCreate() == 0)
	{
		if (CameraStart(v4l2Buffer_phy_addr, 4, preview_w,preview_h) == 0)
		{
			if (DispCreate(corx ,cory,preWidth,preHeight) == 0)
			{
				TaskRuning(1,corx,cory);
			}
			else
			{
				camera_msg->tc_info->result = -1;
				printf("%s display create wrong!\n",__FUNCTION__);
			}
		}
		else
		{
			camera_msg->tc_info->result = -1;
			printf("%s camera start erro\n",__FUNCTION__);
		}
	}
	else
	{
		camera_msg->tc_info->result = -1;
		printf("%s camera create erro\n",__FUNCTION__);
	}
	isstoped = 1;
	hasstoped = 1;
	printf("camrea%d test over\n",cameraId);
	return argv ;
}
int stopCameraTest(){
	return TaskStop();
}

void * camera_test(void *argc)
{
	struct testcase_info *tc_info = (struct testcase_info *)argc;
	struct camera_msg *camera_msg = NULL;
	pthread_t tid;
	int err;
	int id = tc_info->dev_id;
	int x =  gr_fb_width() >> 1;//camera_msg->x;
	int y= 0;//camera_msg->y;
	int w = gr_fb_width() >> 1;//camera_msg->w;
	int h = gr_fb_height()*2/3;// camera_msg->h;
	//printf("%s:video%d x:%d y:%d w:%d h:%d\n",__func__,id,x,y,w,h);
	camera_msg = (struct camera_msg*)malloc(sizeof(struct camera_msg));
	if(!camera_msg)
	{
		printf(" malloc for camera_msg failed\n");
		tc_info->result = -1;
		return  argc;
	}
	camera_msg->id = id;
	camera_msg->x = x;
	camera_msg->y = y;
	camera_msg->w = w;
	camera_msg->h = h;
	camera_msg->tc_info = tc_info;
	err = pthread_create(&tid, NULL,startCameraTest,camera_msg); //
	if(err != 0)
	{  
		printf("start camera test thread error: %s/n",strerror(err)); 
		tc_info->result = -1;
		return  argc;
	   
	}  
	
	return argc;
}

