#include "camera_test.h"
#include "./minuitwrp/minui.h"
#include "test_case.h"
#include "common.h"
#include "script.h"
#include "test_case.h"
#include "language.h"

#define LOG(x...)   printf("[Camera_TEST] "x)

static int fd = -1;
static int rc = -2;
static int mDevFp = -1;
static int y = 0;
int *pBuf;
struct ion_fd_data fd_data;
struct ion_allocation_data alloc_data;
struct v4l2_format v4l2Fmt;
struct v4l2_requestbuffers req;
struct v4l2_buffer v4l2Buf;
struct v4l2_input input;
pthread_t camera_tid;

int pollDataFromCamera(int index)
{
	int ret = -1;
	struct pollfd mEvents;
	input.index = index;
	ret = ioctl(mDevFp, VIDIOC_S_INPUT, &input);
	if (ret < 0) {
		LOG("pollDataFromCamera::channel[%d] set input error,ret=%d\n",input.index,ret);
		return ret;
	}
	LOG("pollDataFromCamera::set input ok\n");
	
	//设置format
	memset(&v4l2Fmt, 0, sizeof(v4l2Fmt));
	v4l2Fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2Fmt.fmt.pix.width = 854;
	v4l2Fmt.fmt.pix.height = 480;
	v4l2Fmt.fmt.pix.bytesperline = 0;//854*4;//0
	v4l2Fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV422P;//V4L2_PIX_FMT_RGB32;//V4L2_PIX_FMT_YUV422P;//v4l2PixFmt;
	v4l2Fmt.fmt.pix.sizeimage = (854 * 480 * 16) / 8;
	
	ret = -1;
	ret = ioctl(mDevFp, VIDIOC_S_FMT, &v4l2Fmt);
	if(ret){
		LOG("pollDataFromCamera::set format error.");
		return ret;
	}
   	
   	LOG("pollDataFromCamera::set format ok\n");
	
	//申请帧缓冲空间
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;
	
	ret = -1;
	ret = ioctl(mDevFp, VIDIOC_REQBUFS, &req);
	if(ret){
		LOG("pollDataFromCamera::request buffers error,ret=%d\n",ret);
		return ret;
	}
	
	LOG("pollDataFromCamera::request buffers ok\n");
	
	//把申请到的帧缓冲全部入队列，以便存放采集到的数据
	v4l2Buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2Buf.memory = V4L2_MEMORY_USERPTR;
	v4l2Buf.index = 0;//buffer.getIndex();
	v4l2Buf.m.userptr = pBuf;
	v4l2Buf.length = (854*480*16/8);//buffer.getCapacity();
	
	ret = -1;
	ret = ioctl(mDevFp, VIDIOC_QBUF, &v4l2Buf);
	if(ret){
		LOG("pollDataFromCamera::camera qbuf error,ret=%d\n",ret);
		return ret;
	}
	
	LOG("pollDataFromCamera::queue buffer ok\n");
	
	//开始视频的采集
	ret = -1;
	ret = ioctl(mDevFp, VIDIOC_STREAMON, &v4l2Buf.type);
	if(ret){
		LOG("pollDataFromCamera::camera streamon error,ret=%d\n",ret);
		return ret;
	}
	
	LOG("pollDataFromCamera::turn on camera ok\n");

	ioctl(mDevFp, VIDIOC_STREAMOFF, &v4l2Buf.type);

	
	//出队列以取得已采集数据的帧缓冲，取得原始采集数据
	ret = -1;
	mEvents.fd = mDevFp;
   	mEvents.revents = 0;
   	mEvents.events = POLLIN | POLLERR;
	ret = poll(&mEvents, 1, 1000);
	if (ret < 0) {
		LOG("pollDataFromCamera::camera dqbuf poll error,ret=%d\n",ret);
		return ret;
	}
	else if (!ret) {
		LOG("pollDataFromCamera::camera dqbuf poll timeout,ret=%d\n",ret);
		return ret;
   	}
   	LOG("pollDataFromCamera::camera poll ok,ret=%d\n",ret);
	return 1;
}

int startCameraTest()
{
	int ret = -1;
	fd = open("/dev/ion", O_RDWR);
	if (fd < 0) {
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_FAILED);
	    LOG("startCameraTest::opening ion device failed with fd = %d\n", fd);
	    return fd;
	}
	alloc_data.flags = 0;
	alloc_data.heap_id_mask = (1<<4);//camera_id = 4,mask=1<<4
	alloc_data.len = 4194304;//1048576;//128*4096;//854*480;//must page align
	alloc_data.align = 0;//1024*1024;//clip2(alignment);
	rc = ioctl(fd,ION_IOC_ALLOC,&alloc_data);

	if(rc){
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_FAILED);
        LOG("startCameraTest::ION ALLOC memory failed,rc=%d\n",rc);
        close(fd);
        fd = -1;
        return fd;
	}

	fd_data.handle = alloc_data.handle;
    rc = -2;
	rc = ioctl(fd,ION_IOC_MAP,&fd_data);
	
	if (rc) {
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_FAILED);
		LOG("startCameraTest::ION MAP failed,rc=%d\n",rc);
		fd_data.fd =-1;
		close(fd);
		fd = -1;
		return fd;
	}
	
    printf("startCameraTest::pBuf=0x%08x,fd_data.fd=%d\n",pBuf,fd_data.fd);
	pBuf = mmap(NULL, 4194304, /*PROT_READ | PROT_WRITE*/3, 1, fd_data.fd, 0);
	if (pBuf == MAP_FAILED) {
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_FAILED);
        LOG("startCameraTest::mmap failed,pBuf=0x%08x\n",pBuf);
        return -1;
	}
	memset(pBuf,0x55,1024);
	memset(pBuf+854*480/4,0x55,1024);
	
	mDevFp = open("/dev/video2", O_RDWR);
	if (mDevFp < 0) {
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_FAILED);
		LOG("startCameraTest::Cannot open camera.\n");
		return mDevFp;
	}

	printf("startCameraTest::open camera ok\n");

	ret = -1;
	int index = 0;
	while(1){
		input.index = index;
		ret = ioctl(mDevFp, VIDIOC_ENUMINPUT, &input);
		if(ret != 0){
			//printf("startCameraTest::channel[%d] enum input error,ret=%d\n",input.index,ret);
			break;//return ret;
		}
		//printf("startCameraTest::Name of input channel[%d] is %s\n", input.index, input.name);
		index++;
	}
	
	if(index == 0){
		ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] \n",PCBA_CAMERA,PCBA_FAILED);
		return ret;
	}

	if(index == 1)
	{
		if(pollDataFromCamera(0) > 1)
		{
			ui_print_xy_rgba(0,y,0,255,0,255,"%s:[%s]\n",PCBA_BACK_CAMERA,PCBA_SECCESS);
		}
		else
		{
			ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED);
		}
	}

	if(index == 2)
	{
		int result_back = pollDataFromCamera(0);
		int result_front = pollDataFromCamera(1);
		LOG("startCameraTest::result_back=%d,result_front=%d\n", result_back, result_front);
		if(result_back == 1 && result_front <= 0)
		{
			ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_SECCESS,PCBA_FRONT_CAMERA,PCBA_FAILED);
		}

		if(result_back <= 0 && result_front == 1)
		{
			ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_SECCESS);
		}

		if(result_back <= 0 && result_front <= 0)
		{
			ui_print_xy_rgba(0,y,255,0,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_FAILED,PCBA_FRONT_CAMERA,PCBA_FAILED);
		}

		if(result_back == 1 && result_front == 1)
		{
			ui_print_xy_rgba(0,y,0,255,0,255,"%s:[%s] %s:[%s]\n",PCBA_BACK_CAMERA,PCBA_SECCESS,PCBA_FRONT_CAMERA,PCBA_SECCESS);
		}
	}

	close(mDevFp);
	return 0;
}

void * camera_test(void *argc)
{
	int ret,num;
	struct testcase_info *tc_info = (struct testcase_info*)argc;
	int arg = 1; 

	if(tc_info->y <= 0)
		tc_info->y  = get_cur_print_y();	

	y = tc_info->y;
	
	ui_print_xy_rgba(0,y,255,255,0,255,"%s:[%s..] \n",PCBA_CAMERA,PCBA_TESTING);

	pthread_create(&camera_tid, NULL, startCameraTest, NULL); 
		
	return argc;
}

