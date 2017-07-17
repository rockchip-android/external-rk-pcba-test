#include "camera_test.h"
#include "../minui_pcba/minui.h"
#include "../test_case.h"
#include "sys_core_ion.h"
#define VIDEO_DEV_NAME   "/dev/video0"
#define PMEM_DEV_NAME    "/dev/pmem_cam"
#define DISP_DEV_NAME    "/dev/graphics/fb1"
#define ION_DEVICE          "/dev/ion"

#define CAMSYS_DEPATH_MARVIN	"/dev/camsys_marvin"
#define CAMSYS_DEPATH_MARVIN1	"/dev/camsys_marvin1"

#define FBIOSET_ENABLE			0x5019

/********add by cx start****/
#define MIPI_LANE_NUM					2
#define MIPI_PHY_INDEX					0

#define ION_LEN							0x1600000
#define SENSOR_MCLK						24000000

#define MRV_ISP_BASE                      0x400 
#define MRV_ISP_MIS                       (MRV_ISP_BASE+0x1c4)
#define MRV_ISP_ICR                       (MRV_ISP_BASE+0x1c8)

#define MRV_MI_BASE                       (0x1400)
#define MRV_MI_MIS                        (MRV_MI_BASE+0x100)
#define MRV_MI_ICR                        (MRV_MI_BASE+0x104)

//GRF_SOC_CON14
//bit 0     dphy_rx0_testclr
//bit 1     dphy_rx0_testclk
//bit 2     dphy_rx0_testen
//bit 3:10 dphy_rx0_testdin
#define GRF_SOC_CON14_OFFSET    (0x027c/4)
#define DPHY_RX0_TESTCLR_MASK   (0x1<<16)
#define DPHY_RX0_TESTCLK_MASK   (0x1<<17)
#define DPHY_RX0_TESTEN_MASK    (0x1<<18)
#define DPHY_RX0_TESTDIN_MASK   (0xff<<19)

#define DPHY_RX0_TESTCLR    (1<<0)
#define DPHY_RX0_TESTCLK    (1<<1)
#define DPHY_RX0_TESTEN     (1<<2)
#define DPHY_RX0_TESTDIN_OFFSET    (3)

#define DPHY_TX1RX1_ENABLECLK_MASK   (0x1<<28)
#define DPHY_RX1_SRC_SEL_MASK        (0x1<<29)
#define DPHY_TX1RX1_MASTERSLAVEZ_MASK (0x1<<30)
#define DPHY_TX1RX1_BASEDIR_OFFSET  (0x1<<31)

#define DPHY_TX1RX1_ENABLECLK           (0x1<<12)
#define DPHY_TX1RX1_DISABLECLK          (0x0<<12)
#define DPHY_RX1_SRC_SEL_ISP          (0x1<<13)
#define DPHY_TX1RX1_SLAVEZ            (0x0<<14)
#define DPHY_TX1RX1_BASEDIR_REC       (0x1<<15)

//GRF_SOC_CON6
//bit 0 grf_con_disable_isp
//bit 1 grf_con_isp_dphy_sel  1'b0 mipi phy rx0
#define GRF_SOC_CON6_OFFSET    (0x025c/4)
#define MIPI_PHY_DISABLE_ISP_MASK       (0x1<<16)
#define MIPI_PHY_DISABLE_ISP            (0x0<<0)

#define DSI_CSI_TESTBUS_SEL_MASK        (0x1<<30)
#define DSI_CSI_TESTBUS_SEL_OFFSET_BIT  (14)


#define MIPI_PHY_DPHYSEL_OFFSET_MASK (0x1<<17)
#define MIPI_PHY_DPHYSEL_OFFSET_BIT (0x1)

//GRF_SOC_CON10
//bit12:15 grf_dphy_rx0_enable
//bit 0:3 turn disable
#define GRF_SOC_CON10_OFFSET                (0x026c/4)
#define DPHY_RX0_TURN_DISABLE_MASK          (0xf<<16)
#define DPHY_RX0_TURN_DISABLE_OFFSET_BITS   (0x0)
#define DPHY_RX0_ENABLE_MASK                (0xf<<28)
#define DPHY_RX0_ENABLE_OFFSET_BITS         (12)

//GRF_SOC_CON9
//bit12:15 grf_dphy_rx0_enable
//bit 0:3 turn disable
#define GRF_SOC_CON9_OFFSET                (0x0268/4)
#define DPHY_TX1RX1_TURN_DISABLE_MASK          (0xf<<16)
#define DPHY_TX1RX1_TURN_DISABLE_OFFSET_BITS   (0x0)
#define DPHY_TX1RX1_ENABLE_MASK                (0xf<<28)
#define DPHY_TX1RX1_ENABLE_OFFSET_BITS         (12)

//GRF_SOC_CON15
//bit 0:3   turn request
#define GRF_SOC_CON15_OFFSET                (0x03a4/4) 
#define DPHY_RX0_TURN_REQUEST_MASK          (0xf<<16)
#define DPHY_RX0_TURN_REQUEST_OFFSET_BITS   (0x0)

#define DPHY_TX1RX1_TURN_REQUEST_MASK          (0xf<<20)
#define DPHY_TX1RX1_TURN_REQUEST_OFFSET_BITS   (0x4)

#define CSIHOST_PHY_TEST_CTRL0            (0x30/4)
#define CSIHOST_PHY_TEST_CTRL1            (0x34/4)
#define CSIHOST_PHY_SHUTDOWNZ             (0x08/4)
#define CSIHOST_DPHY_RSTZ                 (0x0c/4)
#define CSIHOST_N_LANES                   (0x04/4)
#define CSIHOST_CSI2_RESETN               (0x10/4)
#define CSIHOST_PHY_STATE                 (0x14/4)
#define CSIHOST_DATA_IDS1                 (0x18/4)
#define CSIHOST_DATA_IDS2                 (0x1C/4)
#define CSIHOST_ERR1                      (0x20/4)
#define CSIHOST_ERR2                      (0x24/4)

#define MRV_REG_ADDR(a)                   (a/4)

struct pthread_fake {
    void* nothing[8];
    int pid;
};
/********add by cx end****/



#define CAM_OVERLAY_BUF_NEW  1
#define RK29_CAM_VERSION_CODE_1 KERNEL_VERSION(0, 0, 1)
#define RK29_CAM_VERSION_CODE_2 KERNEL_VERSION(0, 0, 2)

static void *m_v4l2Buffer[4];
static int v4l2Buffer_phy_addr = 0;
static int iCamFd, iDispFd =-1;
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
struct ion_phys_data  phys_data;
struct ion_custom_data data;
#define RK30_PLAT 1
#define RK29_PLAT 0
static int is_rk30_plat = RK30_PLAT;
#define  FB_NONSTAND ((is_rk30_plat == RK29_PLAT)?0x2:0x20)
static int cam_id = 0;

//hkw add;
static int camsys_fd;
#define HAL_DEVID_EXTERNAL  CAMSYS_DEVID_EXTERNAL 

static int camera_x=0,camera_y=0,camera_w=0,camera_h=0,camera_num=0;
static struct testcase_info *tc_info = NULL;

pthread_t camera_tid;

static camsys_phy_type_t phy_type;
static int image_w = 0;
static int image_h = 0;


/**add by cx start***/
static void* halIsrHandle(void *arg)
{
    camsys_irqsta_t irqsta;
    int pid,i,err;

    pid = (unsigned)gettid();

    for(i=0;i<15;i++) {
        err = ioctl(camsys_fd, CAMSYS_IRQWAIT, &irqsta);
        if (err<0) {
            printf("thread(pid:%d) CAMSYS_IRQWAIT failed\n",pid);
        } else {
            printf("thread(pid:%d) mis: 0x%x\n", pid,irqsta.mis);
        }
    }

    return NULL;
}

int marvin_config(MrvAllRegister_t *pMrvReg, unsigned int buf, int numLane)
{
    camsys_sysctrl_t sysctl;
    int err;
    
    sysctl.dev_mask = CAMSYS_DEVID_MARVIN;
    sysctl.ops = CamSys_ClkIn;
    sysctl.on = 1;

    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_ClkIn on failed\n");
    }
            
    pMrvReg->vi_ccl = 0x00;
    pMrvReg->vi_iccl = 0xFFFFFFFF;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    usleep(10);
    pMrvReg->vi_ircl = (1<<7);
    usleep(10);
    pMrvReg->vi_ircl = (0<<7);
    usleep(10);
    usleep(15000);
    pMrvReg->mipi[0].mipi_ctrl = 0x00;
	
    // set ISP to RGB mode, enable input, output formatter and update shadow registers
    // '0: ISP_enable
    // 321:  000: raw picture
    //       001: ccir656
    //       010: ccir601
    //       011: bayer rgb
    //       100: data mode
    // 4: inform enable
    // 5: bls enable   
    // 6: gamma_in enable       
    // 7: awb enable         
    // 8: ae enable          
    // 9: cfg_upd            
    // 10: gen_cfg_upd  
    // 11: gamma_out enable
    if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->isp_ctrl = 0x8c6;
	else
		pMrvReg->isp_ctrl = 0x8c4;
    
    //[5:0]: 2a-RAW8  2B-RAW10  2c-RAW12
    if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->mipi[0].mipi_img_data_sel = 0x2b;
	else
		pMrvReg->mipi[0].mipi_img_data_sel = 0x2c;
    //[5:0] data type selector for additional data output
    //regbase[MRV_REG_ADDR(MRV_MIPI_ADD_DATASEL1)]= 0x12;
    //regbase[MRV_REG_ADDR(MRV_MIPI_ADD_DATASEL2)]= 0x2b;
    //regbase[MRV_REG_ADDR(MRV_MIPI_ADD_DATASEL3)]= 0x2b;
    //regbase[MRV_REG_ADDR(MRV_MIPI_ADD_DATASEL4)]= 0x2b;

    //24: frame end send to output interface
    //23: checksum error
    //22: 1bit ecc error
    //21: 2bit ecc error
    //pMrvReg->mipi[0].mipi_imsc = (1<<24) | (0x07<<21);
    
    //[2]:enable IMSC_DATA_LOSS interrupt
    //[3]:enable IMSC_PIC_SIZE_ERR interrupt
    //regbase[MRV_REG_ADDR(MRV_ISP_IMSC)] = 0xC;   
    pMrvReg->isp_imsc = (1<<1)|(1<<5);

    //[0] sample edge   0- negative  1-positive
    //[1] hsync pol     0-high       1-low
    //[2] vsync pol     0-high       1-low
    //[4:3] color components from sensor, 01-first line:GRGR,sec line BGBG   11-first line:BGBG,sec line GRGR  
    //[6:5] 01-interleaved color subsampling Y0CB0-Y1Cr1
    //[8:7] 10-CbYCrY
    pMrvReg->isp_acq_prop = ((0x3<<3)|(0x1<<5)|(0x2<<7)|(0x05<<0)); //0x13d
                                                     
    // set ISP input aquisition window
    //regbase[MRV_REG_ADDR(MRV_ISP_ACQ_H_OFFS)]      = 0;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_ACQ_V_OFFS)]      = 0;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_ACQ_H_SIZE)]      = IMAGE_W;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_ACQ_V_SIZE)]      = IMAGE_H;  //
    pMrvReg->isp_acq_h_offs = 0 & MRV_ISP_ACQ_H_OFFS_MASK;
    pMrvReg->isp_acq_v_offs = 0 & MRV_ISP_ACQ_V_OFFS_MASK;
	if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->isp_acq_h_size = image_w & MRV_ISP_ACQ_H_SIZE_MASK;
	else
		pMrvReg->isp_acq_h_size = (image_w * 2) & MRV_ISP_ACQ_H_SIZE_MASK;
    pMrvReg->isp_acq_v_size = image_h & MRV_ISP_ACQ_V_SIZE_MASK;
    // set ISP output window        
    //regbase[MRV_REG_ADDR(MRV_ISP_OUT_H_OFFS)]      = 0;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_OUT_V_OFFS)]      = 0;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_OUT_H_SIZE)]      = IMAGE_W;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_OUT_V_SIZE)]      = IMAGE_H;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_IS_H_OFFS)]      = 0;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_IS_V_OFFS)]      = 0;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_IS_H_SIZE )]      = IMAGE_W;  //
    //regbase[MRV_REG_ADDR(MRV_ISP_IS_V_SIZE )]      = IMAGE_H;  //
	
    pMrvReg->isp_out_h_offs = 0 & MRV_ISP_ISP_OUT_H_OFFS_MASK;
	usleep(2);
    pMrvReg->isp_out_v_offs = 0 & MRV_ISP_ISP_OUT_V_OFFS_MASK;
    pMrvReg->isp_out_h_size = image_w & MRV_ISP_ISP_OUT_H_SIZE_MASK;
    pMrvReg->isp_out_v_size = image_h & MRV_ISP_ISP_OUT_V_SIZE_MASK;


    pMrvReg->isp_is_h_offs = 0 & MRV_IS_IS_H_OFFS_MASK;
	usleep(2);
    pMrvReg->isp_is_v_offs = 0 & MRV_IS_IS_V_OFFS_MASK;                    
	pMrvReg->isp_is_h_size = image_w & MRV_IS_IS_H_SIZE_MASK;
	pMrvReg->isp_is_v_size = image_h & MRV_IS_IS_V_SIZE_MASK;

    //[1:0] data path select: 01-from main resize to MI, uncompressed
    //[3:2] yc splitter channel mode: 01-main path and raw data mode
	//[9:8] select input interface : 00-parallel-interface 10-MIPI1-interface
	if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->vi_dpcl = 0x205;
	else
		pMrvReg->vi_dpcl = 0x005;

    
    // set ISP input number of frames to 0 (continuous)
    //regbase[MRV_REG_ADDR(MRV_ISP_ACQ_NR_FRAMES)]   = 0x0;
    if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->isp_acq_nr_frames = 0x10;
	else
		pMrvReg->isp_acq_nr_frames = 0x00;
    /*
    //regbase[MRV_REG_ADDR(MRV_ISP_DEMOSAIC)]        = 0x008; //number of input frames to be sampled  
    pMrvReg->isp_demosaic = 0x08;
    // enable filter
    //[0]:filt_enable
    //[1]:filt_mode:1-dynamic noise reduction/sharpen Default
	//[5:4]:chroma filter vertical mode:01-vertical chroma filter 1 static[8 16 8]
	//[7:6]:chroma filter horizontal mode:10-horizontal chroma filter2(dynamic blur1)
    //regbase[MRV_REG_ADDR(MRV_ISP_FILT_MODE)]       = (0x1|(0x1<<1)|(0x1<<4)|(0x2<<6));    
    pMrvReg->isp_filt_mode = (0x1|(0x1<<1)|(0x1<<4)|(0x2<<6));    
     */                                                        
    // set MI main Y buffer position and size
    pMrvReg->mi_mp_y_base_ad_init = buf;
    pMrvReg->mi_mp_y_size_init = image_w*image_h*2;
    // set MI main Cb buffer position and size
    pMrvReg->mi_mp_cb_base_ad_init = 0;
    pMrvReg->mi_mp_cb_size_init = 0;
    // set MI main Cr buffer position and size    
    pMrvReg->mi_mp_cr_base_ad_init = 0;
    pMrvReg->mi_mp_cr_size_init = 0;
	
    // enable all MI interrupts
    //main picture end of frame interrupt
    pMrvReg->mi_imsc = 0x01;
    
    // set MI in main picture only mode and update shadow registers immediately
    //sp_output_format = YUV422;
    //sp_input_format  = YUV422;
    //sp_write_format  = PLANAR;
    //mp_write_format  = PLANAR;
    //init_offs_en     = 1
    //init_base_en     = 1;
    //burst_len_chrom  = BURST_4;
    //burst_len_lum    = BURST_4;
    //mp_enable        = 1;
    //regbase[MRV_REG_ADDR(MRV_MI_CTRL)]             = 0x28300001;
    //pMrvReg->mi_ctrl = 0x28300001;
    //pMrvReg->mi_ctrl = 0x752001;//mi_mp_y_size_init = IMAGE_W*IMAGE_H;
    if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->mi_ctrl = 0xba2001;//mi_mp_y_size_init = IMAGE_W*IMAGE_H*2;
    else
		pMrvReg->mi_ctrl = 0xb00001;
    // soft update
    //regbase[MRV_REG_ADDR(MRV_MI_INIT)]             = 0x10;
    pMrvReg->mi_init = 0x10;
    //[0]: 1-output to add data fifo and to output interfaceis enable
    //[12:13] :num data lanes:  01 lane 1 and 2 used,
    //[18] : enable_clk
    if (numLane==2)
       pMrvReg->mipi[0].mipi_ctrl = ((0x01<<12)|0x1<<18|(0x0f<<8));  
    else if (numLane == 1)
       pMrvReg->mipi[0].mipi_ctrl = ((0x00<<12)|0x1<<18|(0x0f<<8));  
    else if (numLane == 4)
       pMrvReg->mipi[0].mipi_ctrl = ((0x03<<12)|0x1<<18|(0x0f<<8));  

    printf("marvin_config:  numLane: %d  mipi_ctrl: 0x%x\n",
        numLane,pMrvReg->mipi[0].mipi_ctrl); 
    return 0;
}

int marvin_check_vsync(MrvAllRegister_t *pMrvReg)
{
    camsys_sysctrl_t sysctl;
	int result = 0;
   
   //31  S_HSYNC	state of ISP input port s_hsync, for test purposes
   //30  S_VSYNC	state of ISP input port s_vsync, for test purposes
   //29:28  ---  unused
   //27:16  S_DATA  state of ISP input port s_data, for test purposes
   //15:3  ---  unused
   // 2  INFORM_FIELD	current field information (0=odd, 1=even)
   // 1  ISP_INFORM_ENABLE_SHD  Input formatter enable shadow register
   // 0  ISP_ENABLE_SHD  ISP enable shadow register shows, if ISP currently outputs data (1) or not (0)

    if(pMrvReg->isp_flags_shd && 0x40000000) {
		result = 0;
		printf("check vsync is high! \n",pMrvReg->isp_flags_shd); 
	} else {
		result = 1;
		printf("check vsync is low! \n",pMrvReg->isp_flags_shd); 		
	}

    printf("pMrvReg->isp_flags_shd: 0x%08x\n",pMrvReg->isp_flags_shd); 
    return result;
}


int marvin_start(MrvAllRegister_t *pMrvReg)
{
    pMrvReg->mipi[0].mipi_ctrl |= 0x01;

	if (phy_type == CamSys_Phy_Mipi)
    	pMrvReg->isp_ctrl = (0x8c6|(1<<9)|(1<<4)|(1<<0));
	else
		pMrvReg->isp_ctrl = (0x8c4|(1<<9)|(1<<4)|(1<<0));
    printf("isp turn on!\n");
    return 0;
}

int marvin_stop(MrvAllRegister_t *pMrvReg)
{
    pMrvReg->isp_ctrl = (1<<8); 
    pMrvReg->mipi[0].mipi_ctrl = 0x00; 
    sleep(1);
    pMrvReg->vi_iccl = 0x00;
    pMrvReg->vi_ccl = (1<<2);

    return 0;
}

int extdev_register(unsigned int numLane)
{
    int err;
    camsys_devio_name_t extdev;

	memset(&extdev,0x00, sizeof(camsys_devio_name_t));
    extdev.dev_id = CAMSYS_DEVID_SENSOR_1A;
	strlcpy((char*)extdev.dev_name, SENSOR_NAME,sizeof(SENSOR_NAME));
    strlcpy((char*)extdev.avdd.name, "NC",sizeof(extdev.avdd.name)); 
	extdev.avdd.min_uv = 0x0;
    extdev.avdd.max_uv = 0x0;
    strlcpy((char*)extdev.dovdd.name,"NC",sizeof(extdev.dovdd.name));
    extdev.dovdd.min_uv = 18000000;
    extdev.dovdd.max_uv = 18000000;
    strlcpy((char*)extdev.dvdd.name, "NC",sizeof(extdev.dvdd.name));
    extdev.dvdd.min_uv = 0x0;
    extdev.dvdd.max_uv = 0x0;	
    strlcpy((char*)extdev.afvdd.name, "NC",sizeof(extdev.afvdd.name));
    extdev.afvdd.min_uv = 0x0;
    extdev.afvdd.max_uv = 0x0;
    strlcpy((char*)extdev.pwrdn.name, "RK30_PIN1_PB5",sizeof(extdev.pwrdn.name));
    extdev.pwrdn.active = 0x00;
    strlcpy((char*)extdev.pwren.name, "RK30_PIN0_PD0",sizeof(extdev.pwren.name));//RK30_PIN0_PD0
    extdev.pwren.active = 0x01;
    strlcpy((char*)extdev.rst.name,"NC",sizeof(extdev.rst.name));
	extdev.rst.active = 0x0;
    strlcpy((char*)extdev.afpwrdn.name,"NC",sizeof(extdev.afpwrdn.name));
	extdev.afpwrdn.active = 0x0;
    strlcpy((char*)extdev.afpwr.name,"NC",sizeof(extdev.afpwr.name));
	extdev.afpwr.active = 0x0;

	extdev.dev_cfg |= CAMSYS_DEVCFG_FLASHLIGHT;
	extdev.fl.fl.active = 0;
	extdev.fl.fl_en.active = 0; 
	strlcpy((char*)extdev.fl.fl.name,"NC",sizeof(extdev.fl.fl.name));
	strlcpy((char*)extdev.fl.fl_en.name,"NC",sizeof(extdev.fl.fl_en.name));
	strlcpy((char*)extdev.fl.fl_drv_name,"NC",sizeof(extdev.fl.fl_drv_name));

	if (phy_type == CamSys_Phy_Mipi)
    	extdev.phy.type = CamSys_Phy_Mipi;
	else
		extdev.phy.type = CamSys_Phy_Cif;
	if(extdev.phy.type == CamSys_Phy_Cif){
        //extdev.phy.type = CamSys_Phy_Cif;
        extdev.phy.info.cif.fmt = CamSys_Fmt_Yuv422_10b;
        extdev.phy.info.cif.cif_num = 0;
        extdev.phy.info.cif.cifio = CamSys_SensorBit0_CifBit4;
    } else if(extdev.phy.type == CamSys_Phy_Mipi){
	    extdev.phy.info.mipi.phy_index = MIPI_PHY_INDEX;
	    if (numLane == 1) {
	        extdev.phy.info.mipi.data_en_bit = 0x01;
	        extdev.phy.info.mipi.bit_rate = 720;//656;
	    } else if (numLane == 2) {
	        extdev.phy.info.mipi.data_en_bit = 0x03;
	        extdev.phy.info.mipi.bit_rate = 328;
	    } else if (numLane == 4) {
	        extdev.phy.info.mipi.data_en_bit = 0x0f;
	        extdev.phy.info.mipi.bit_rate = 408;
	    }
    } else{
        printf("%s %d: unknow phy type(%d)\n",__func__,__LINE__, extdev.phy.type);
    }
    extdev.clk.in_rate = SENSOR_MCLK;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    err = ioctl(camsys_fd, CAMSYS_REGISTER_DEVIO, &extdev);
    if (err<0) {
        printf("CAMSYS_REGISTER_DEVIO failed\n");
    }

    return err;

}
int extdev_init(int camsys_fd,unsigned int *i2cbase)
{
    int err,i;
    camsys_sysctrl_t sysctl;    
    camsys_mipiphy_t mipiphy;
	int id = 0;

    sysctl.dev_mask = (CAMSYS_DEVID_SENSOR_1A|CAMSYS_DEVID_MARVIN);
    sysctl.ops = CamSys_ClkIn;
    sysctl.on = 1;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_ClkIn on failed\n");
		goto end;
		
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  	
    usleep(15000);	

    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1A;
    sysctl.ops = CamSys_Phy;
    sysctl.on = 1;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
    
    mipiphy.phy_index = MIPI_PHY_INDEX;
    if (MIPI_LANE_NUM == 1) {
        mipiphy.data_en_bit = 0x01;
        mipiphy.bit_rate = 720;//656;
    } else if (MIPI_LANE_NUM == 2) {
        mipiphy.data_en_bit = 0x03;
        mipiphy.bit_rate = 720;
    } else if (MIPI_LANE_NUM == 4) {
        mipiphy.data_en_bit = 0x0f;
        mipiphy.bit_rate = 720;
    } 
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  


    memcpy(sysctl.rev,&mipiphy, sizeof(camsys_mipiphy_t));
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);     
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_Phy on failed");
		goto end;
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  	
    usleep(5000);

    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1A;
    sysctl.ops = CamSys_PwrEn;
    sysctl.on = 1;
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_PwrEn on failed");
		goto end;
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  	
	usleep(1000);
    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1A;
    sysctl.ops = CamSys_Rst;
    sysctl.on = 0;
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_PwrDn on failed\n");
		goto end;
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    usleep(1000);
    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1A;
    sysctl.ops = CamSys_PwrDn;
    sysctl.on = 0;
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_PwrDn on failed");
		goto end;
    }
    usleep(2000);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    if (!strcmp(BACK_SENSOR_NAME, "ov8858"))
		err = Ov8858_sensor_reg_init(camsys_fd,i2cbase);
	else
		printf(" %s sensor_reg_init fail", BACK_SENSOR_NAME);

end:
	//ui_print_xy_rgba(0,tc_info->y,0,255,0,255,"Back Camera:[%s] { ID:0x%x }\n",err ? PCBA_FAILED : PCBA_SECCESS,id);
    return err;
}

int extdev_register_front(unsigned int numLane)
{
    int err;
    camsys_devio_name_t extdev;

	memset(&extdev,0x00, sizeof(camsys_devio_name_t));
    extdev.dev_id = CAMSYS_DEVID_SENSOR_1B;
	strlcpy((char*)extdev.dev_name, SENSOR_NAME,sizeof(SENSOR_NAME));
    strlcpy((char*)extdev.avdd.name, "NC",sizeof(extdev.avdd.name)); 
	extdev.avdd.min_uv = 0x0;
    extdev.avdd.max_uv = 0x0;
    strlcpy((char*)extdev.dovdd.name,"NC",sizeof(extdev.dovdd.name));
    extdev.dovdd.min_uv = 18000000;
    extdev.dovdd.max_uv = 18000000;
    strlcpy((char*)extdev.dvdd.name, "NC",sizeof(extdev.dvdd.name));
    extdev.dvdd.min_uv = 0x0;
    extdev.dvdd.max_uv = 0x0;	
    strlcpy((char*)extdev.afvdd.name, "NC",sizeof(extdev.afvdd.name));
    extdev.afvdd.min_uv = 0x0;
    extdev.afvdd.max_uv = 0x0;
    strlcpy((char*)extdev.pwrdn.name, "RK30_PIN1_PB4",sizeof(extdev.pwrdn.name));
    extdev.pwrdn.active = 0x01;
    strlcpy((char*)extdev.pwren.name, "RK30_PIN0_PD0",sizeof(extdev.pwren.name));
    extdev.pwren.active = 0x01;
    strlcpy((char*)extdev.rst.name,"NC",sizeof(extdev.rst.name));
	extdev.rst.active = 0x0;
    strlcpy((char*)extdev.afpwrdn.name,"NC",sizeof(extdev.afpwrdn.name));
	extdev.afpwrdn.active = 0x0;
    strlcpy((char*)extdev.afpwr.name,"NC",sizeof(extdev.afpwr.name));
	extdev.afpwr.active = 0x0;

	extdev.dev_cfg |= CAMSYS_DEVCFG_FLASHLIGHT;
	extdev.fl.fl.active = 0;
	extdev.fl.fl_en.active = 0; 
	strlcpy((char*)extdev.fl.fl.name,"NC",sizeof(extdev.fl.fl.name));
	strlcpy((char*)extdev.fl.fl_en.name,"NC",sizeof(extdev.fl.fl_en.name));
	strlcpy((char*)extdev.fl.fl_drv_name,"NC",sizeof(extdev.fl.fl_drv_name));

    if (phy_type == CamSys_Phy_Mipi)
    	extdev.phy.type = CamSys_Phy_Mipi;
	else
		extdev.phy.type = CamSys_Phy_Cif;

    if(extdev.phy.type == CamSys_Phy_Cif){
        //extdev.phy.type = CamSys_Phy_Cif;
        extdev.phy.info.cif.fmt = CamSys_Fmt_Yuv422_10b;
        extdev.phy.info.cif.cif_num = 0;
        extdev.phy.info.cif.cifio = CamSys_SensorBit0_CifBit4;
    }else if(extdev.phy.type == CamSys_Phy_Mipi){
		extdev.phy.info.mipi.phy_index = MIPI_PHY_INDEX;

		if (numLane == 1) {
			extdev.phy.info.mipi.data_en_bit = 0x01;
			extdev.phy.info.mipi.bit_rate = 720;//656;
		} else if (numLane == 2) {
			extdev.phy.info.mipi.data_en_bit = 0x03;
			extdev.phy.info.mipi.bit_rate = 328;
		} else if (numLane == 4) {
			extdev.phy.info.mipi.data_en_bit = 0x0f;
			extdev.phy.info.mipi.bit_rate = 408;
		}

    }else{
        ALOGE("%s %d: unknow phy type(%d)\n",__func__,__LINE__, extdev.phy.type);
    }

    extdev.clk.in_rate = SENSOR_MCLK;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    err = ioctl(camsys_fd, CAMSYS_REGISTER_DEVIO, &extdev);
    if (err<0) {
        printf("CAMSYS_REGISTER_DEVIO failed\n");
    }

    return err;

}

int extdev_init_front(int camsys_fd,unsigned int *i2cbase)
{
    int err,i;
    camsys_sysctrl_t sysctl;    
    camsys_mipiphy_t mipiphy;

    sysctl.dev_mask = (CAMSYS_DEVID_SENSOR_1B|CAMSYS_DEVID_MARVIN);
    sysctl.ops = CamSys_ClkIn;
    sysctl.on = 1;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_ClkIn on failed\n");
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  	
    usleep(15000);	

    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1B;
    sysctl.ops = CamSys_Phy;
    sysctl.on = 1;
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
    
    mipiphy.phy_index = MIPI_PHY_INDEX;
    if (MIPI_LANE_NUM == 1) {
        mipiphy.data_en_bit = 0x01;
        mipiphy.bit_rate = 720;//656;
    } else if (MIPI_LANE_NUM == 2) {
        mipiphy.data_en_bit = 0x03;
        mipiphy.bit_rate = 720;
    } else if (MIPI_LANE_NUM == 4) {
        mipiphy.data_en_bit = 0x0f;
        mipiphy.bit_rate = 720;
    } 
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  


    memcpy(sysctl.rev,&mipiphy, sizeof(camsys_mipiphy_t));
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);     
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_Phy on failed");
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  	
    usleep(5000); 

    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1B;
    sysctl.ops = CamSys_PwrEn;
    sysctl.on = 1;
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_PwrEn on failed");
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  	
	usleep(1000);
    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1B;
    sysctl.ops = CamSys_Rst;
    sysctl.on = 0;
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_PwrDn on failed\n");
    }
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    usleep(1000);
    sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1B;
    sysctl.ops = CamSys_PwrDn;
    sysctl.on = 0;
    err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
    if (err<0) {
        printf("CamSys_PwrDn on failed");
    }
    usleep(2000);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

    if (!strcmp(FRONT_SENSOR_NAME, "gc2155"))
		err = Gc2155_sensor_reg_init(camsys_fd,i2cbase);
	else if (!strcmp(FRONT_SENSOR_NAME, "gc2145"))
		err = Gc2145_sensor_reg_init(camsys_fd,i2cbase);
	else
		printf(" %s sensor_reg_init fail", FRONT_SENSOR_NAME);
    return err;

}


int thread_create_and_connect(pthread_t *thread)
{
    int err;
    camsys_irqcnnt_t irqcnnt;    
    struct pthread_fake *pth_fake;
    
    err = pthread_create(&thread[0],NULL,halIsrHandle,NULL);
    if (err<0) {
        printf("thread[0] create failed\n");
    } else {
        pth_fake = (struct pthread_fake*)thread[0];
        irqcnnt.pid = pth_fake->pid;
        irqcnnt.mis = MRV_ISP_MIS;
        irqcnnt.icr = MRV_ISP_ICR;
        irqcnnt.timeout = 200000;
        err = ioctl(camsys_fd, CAMSYS_IRQCONNECT, &irqcnnt);
        if (err<0) {
            printf("thread(ISP pid:%d) CAMSYS_IRQCONNECT failed\n",pth_fake->pid);
        } else {
            printf("thread(ISP pid:%d) CAMSYS_IRQCONNECT success\n",irqcnnt.pid);
        }
        
    }

    err = pthread_create(&thread[1],NULL,halIsrHandle,NULL);
    if (err<0) {
        printf("thread[1] create failed\n");
    } else {
        pth_fake = (struct pthread_fake*)thread[1];
        irqcnnt.pid = pth_fake->pid;
        irqcnnt.mis = MRV_MI_MIS;
        irqcnnt.icr = MRV_MI_ICR;
        irqcnnt.timeout = 200000;
        err = ioctl(camsys_fd, CAMSYS_IRQCONNECT, &irqcnnt);
        if (err<0) {
            printf("thread(MI pid:%d) CAMSYS_IRQCONNECT failed\n",pth_fake->pid);
        } else {
            printf("thread(MI pid:%d) CAMSYS_IRQCONNECT success\n",irqcnnt.pid);
        }
    }

    return err;
}

int thread_disconnect(pthread_t *thread)
{
    struct pthread_fake *pth_fake;
    camsys_irqcnnt_t irqcnnt;  
    
    pth_fake = (struct pthread_fake*)thread[0];
    irqcnnt.pid = pth_fake->pid;
    if (ioctl(camsys_fd, CAMSYS_IRQDISCONNECT, &irqcnnt)<0) {
        printf("thread(pid: %d) disconnect failed\n",irqcnnt.pid);
    } else {
        printf("thread(pid: %d) disconnect success\n",irqcnnt.pid);
    }
    
    pth_fake = (struct pthread_fake*)thread[1];
    irqcnnt.pid = pth_fake->pid;
    if (ioctl(camsys_fd, CAMSYS_IRQDISCONNECT, &irqcnnt)<0) {
        printf("thread(pid: %d) disconnect failed\n",irqcnnt.pid);
    } else {
        printf("thread(pid: %d) disconnect success\n",irqcnnt.pid);
    }

    return 0;
}


static void phy1_WriteReg(unsigned int *csi_host_base, uint8_t addr, uint8_t data)
{
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL1) = 0x00010000|addr;    //TESTEN =1,TESTDIN=addr
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL0) = 0x00000000;         //TESTCLK=0
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL1) = 0x00000000|data;    //TESTEN =0,TESTDIN=data
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL0) = 0x00000002;         //TESTCLK=1
}
static uint8_t phy1_ReadReg(unsigned int *csi_host_base,uint8_t addr)  
{
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL1) = (0x00010000|addr);   //TESTEN =1,TESTDIN=addr
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL0) = 0x00000000;          //TESTCLK=0

   return 0;
}


void phy1_start (unsigned int *csi_host_base,int numLane)
{
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_SHUTDOWNZ) = 0x00000000;           //SHUTDOWNZ=0
   *(volatile unsigned int *)(csi_host_base+CSIHOST_DPHY_RSTZ) = 0x00000000;               //RSTZ=0
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL0) = 0x00000002;          //TESTCLK=1
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL0) = 0x00000003;          //TESTCLR=1  

   //set clock lane
    phy1_WriteReg(csi_host_base,0x34,0x15);

   //set lane 0
   /********************
    500-550M 0x0E
    600-650M 0x10
    720M     0x12
    360M     0x2A
    *******************/
    #if (MIPI_LANE_NUM == 1)
    phy1_WriteReg(csi_host_base,0x44,0x10);
    if(numLane == 2)
        phy1_WriteReg(csi_host_base,0x54,0x10);
    #elif (MIPI_LANE_NUM == 2)
    phy1_WriteReg(csi_host_base,0x44,0x2a);
    if(numLane == 2)
        phy1_WriteReg(csi_host_base,0x54,0x2a);
    #endif
        
   phy1_ReadReg(csi_host_base,0x0);
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL0) = 0x00000002;       //TESTCLK=1
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_TEST_CTRL1) = 0x00000000;       //TESTEN =0
   *(volatile unsigned int *)(csi_host_base+CSIHOST_N_LANES) = numLane-1;               //set data lane
   *(volatile unsigned int *)(csi_host_base+CSIHOST_DATA_IDS1) = 0x2b;
   *(volatile unsigned int *)(csi_host_base+CSIHOST_PHY_SHUTDOWNZ) = 0x00000001;        //SHUTDOWNZ=1
   *(volatile unsigned int *)(csi_host_base+CSIHOST_DPHY_RSTZ) = 0x00000001;            //RSTZ=1
   *(volatile unsigned int *)(csi_host_base+CSIHOST_CSI2_RESETN) = 0x01;
   return; 
}

void phy0_WriteReg(unsigned int *grf_base, uint8_t addr, int data)
{
   // uint8_t test_data = 0;
    //TESTEN =1,TESTDIN=addr
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (( addr << DPHY_RX0_TESTDIN_OFFSET) |DPHY_RX0_TESTDIN_MASK | DPHY_RX0_TESTEN| DPHY_RX0_TESTEN_MASK); 
    //	//TESTCLK=1
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTCLK_MASK |DPHY_RX0_TESTCLK);
    
    //TESTCLK=0
	*(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTCLK_MASK); 
  
    if(data != -1){ //write data ?
    	 //TESTEN =0,TESTDIN=data
        *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = 
            (( data << DPHY_RX0_TESTDIN_OFFSET)|DPHY_RX0_TESTDIN_MASK |DPHY_RX0_TESTEN); 

        //TESTCLK=1
        *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = 
            (DPHY_RX0_TESTCLK_MASK |DPHY_RX0_TESTCLK); 
    }
}

static uint8_t phy0_ReadReg(unsigned int *grf_base, uint8_t addr)  
{
    uint8_t data = 0;
    
    //TESTEN =1,TESTDIN=addr
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = 
        ((( addr << DPHY_RX0_TESTDIN_OFFSET) |DPHY_RX0_TESTDIN_MASK | DPHY_RX0_TESTEN| DPHY_RX0_TESTEN_MASK)); 

    //TESTCLK=0
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTCLK_MASK); 

    return data ;

    
}

static void phy0_start(unsigned int *grf_base)
{
//TESTCLK=1
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTCLK_MASK |DPHY_RX0_TESTCLK); 
//TESTCLR=1    
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTCLR_MASK |DPHY_RX0_TESTCLR);   
//TESTCLR=0  zyc
//    write_grf_reg(GRF_SOC_CON14_OFFSET, DPHY_RX0_TESTCLR_MASK); 
    usleep(1000);

//**********************************************************************//
//set clock lane
    phy0_WriteReg(grf_base,0x34,0x14);

//set lane 0
 /********************
    500-550M 0x0E
    600-650M 0x10
    720M     0x12
    360M     0x2A
    *******************/
    #if (MIPI_LANE_NUM == 1)
    phy0_WriteReg(grf_base,0x44,0x10);
    #elif (MIPI_LANE_NUM == 2)
    phy0_WriteReg(grf_base,0x44,0x2a);
    phy0_WriteReg(grf_base,0x54,0x2a);
    #endif
 
 //**********************************************************************//

    //Normal operation
    phy0_WriteReg(grf_base,0x0,-1);
    //TESTCLK=1
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTCLK_MASK |DPHY_RX0_TESTCLK); 

    //TESTEN =0
    *(volatile unsigned int *)(grf_base +GRF_SOC_CON14_OFFSET) = (DPHY_RX0_TESTEN_MASK); 

    return ;

}

void CSI_Config(unsigned int *csi_host_base, unsigned int *grf_base,int numLane)
{
    unsigned char lane_mask = 0,i = 0;
    
    //*(grf_base + GRF_SOC_CON6_OFFSET) = MIPI_PHY_DPHYSEL_OFFSET_MASK | (0<<MIPI_PHY_DPHYSEL_OFFSET_BIT);
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET) = (DSI_CSI_TESTBUS_SEL_MASK | (1<<DSI_CSI_TESTBUS_SEL_OFFSET_BIT));
    //*(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET) = (0<<13) | DPHY_RX1_SRC_SEL_MASK;
    //*(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET) = (0<<14) | DPHY_TX1RX1_MASTERSLAVEZ_MASK;
    //*(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET) = DPHY_TX1RX1_BASEDIR_REC | DPHY_TX1RX1_BASEDIR_OFFSET;
    //*(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET) = (DPHY_TX1RX1_ENABLECLK|DPHY_TX1RX1_ENABLECLK_MASK);

    *(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET) = 
        ((0<<13)|(0<<14)|DPHY_TX1RX1_BASEDIR_REC|DPHY_TX1RX1_ENABLECLK)|
        (DPHY_RX1_SRC_SEL_MASK|DPHY_TX1RX1_MASTERSLAVEZ_MASK|DPHY_TX1RX1_BASEDIR_OFFSET|DPHY_TX1RX1_ENABLECLK_MASK);
    
    for(i=0;i<numLane;i++){
        lane_mask |= 1<<i;
    }

    //  set lan turndisab as 1
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET) = DPHY_TX1RX1_TURN_DISABLE_MASK | (0xf << DPHY_TX1RX1_TURN_DISABLE_OFFSET_BITS);

    //  set lane num
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET) = DPHY_TX1RX1_ENABLE_MASK | (lane_mask << DPHY_TX1RX1_ENABLE_OFFSET_BITS);
    
    //  set lan turnrequest as 0 
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON15_OFFSET) = DPHY_TX1RX1_TURN_REQUEST_MASK | (0x0 << DPHY_TX1RX1_TURN_REQUEST_OFFSET_BITS);

    printf("GRF+SOC_CON14(0x%x): 0x%x\n",GRF_SOC_CON14_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET));
    printf("GRF+SOC_CON9(0x%x): 0x%x\n",GRF_SOC_CON9_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET));
    printf("GRF+SOC_CON15(0x%x): 0x%x\n",GRF_SOC_CON15_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON15_OFFSET));
    printf("GRF+SOC_CON6(0x%x): 0x%x\n",GRF_SOC_CON6_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET));
    
    phy1_start(csi_host_base,numLane);    
}

void ISPPHY0_Config(unsigned int *grf_base,int numLane)
{
    unsigned char lane_mask = 0,i = 0;

    for(i=0;i<numLane;i++){
        lane_mask |= 1<<i;
    }

    *(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET) = ( MIPI_PHY_DISABLE_ISP_MASK | 1);
    
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET) = ((0<<1)|(1<<(16+1)));
    //  set lane num
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON10_OFFSET) = (
        (DPHY_RX0_ENABLE_MASK | (lane_mask << DPHY_RX0_ENABLE_OFFSET_BITS))|
        (DPHY_RX0_TURN_DISABLE_MASK | (0xf << DPHY_RX0_TURN_DISABLE_OFFSET_BITS))|
        ((0xf<<4)|(0xf<<(16+4)))
        ); 
    //  set lan turnrequest as 0   
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON15_OFFSET) = (
        DPHY_RX0_TURN_REQUEST_MASK | (0x0 << DPHY_RX0_TURN_REQUEST_OFFSET_BITS)
        );


    phy0_start(grf_base);

    *(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET) = ( MIPI_PHY_DISABLE_ISP_MASK | 0);


    usleep(200);
    return ;
}


void ISPPHY1_Config(unsigned int *csi_host_base, unsigned int *grf_base,int numLane)
{
    unsigned char lane_mask = 0,i = 0;
    
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET) = 
        ((1<<DSI_CSI_TESTBUS_SEL_OFFSET_BIT) | (1<<1)|
        DSI_CSI_TESTBUS_SEL_MASK|(1<<(16+1)));
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET) = 
        ((0<<13)|(0<<14)|DPHY_TX1RX1_BASEDIR_REC|DPHY_TX1RX1_ENABLECLK)|
        (DPHY_RX1_SRC_SEL_MASK|DPHY_TX1RX1_MASTERSLAVEZ_MASK|DPHY_TX1RX1_BASEDIR_OFFSET|DPHY_TX1RX1_ENABLECLK_MASK);
    
    for(i=0;i<numLane;i++){
        lane_mask |= 1<<i;
    }

    //  set lan turndisab as 1
    //*(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET) = DPHY_TX1RX1_TURN_DISABLE_MASK | (0xf << DPHY_TX1RX1_TURN_DISABLE_OFFSET_BITS);
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET) = DPHY_TX1RX1_TURN_DISABLE_MASK | (0xC << DPHY_TX1RX1_TURN_DISABLE_OFFSET_BITS);

    //  set lane num
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET) = DPHY_TX1RX1_ENABLE_MASK | (lane_mask << DPHY_TX1RX1_ENABLE_OFFSET_BITS);
    
    //  set lan turnrequest as 0 
    *(volatile unsigned int *)(grf_base + GRF_SOC_CON15_OFFSET) = DPHY_TX1RX1_TURN_REQUEST_MASK | (0x0 << DPHY_TX1RX1_TURN_REQUEST_OFFSET_BITS);

    printf("GRF+SOC_CON14(0x%x): 0x%x\n",GRF_SOC_CON14_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON14_OFFSET));
    printf("GRF+SOC_CON9(0x%x): 0x%x\n",GRF_SOC_CON9_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON9_OFFSET));
    printf("GRF+SOC_CON15(0x%x): 0x%x\n",GRF_SOC_CON15_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON15_OFFSET));
    printf("GRF+SOC_CON6(0x%x): 0x%x\n",GRF_SOC_CON6_OFFSET*4,*(volatile unsigned int *)(grf_base + GRF_SOC_CON6_OFFSET));
    
    phy1_start(csi_host_base,numLane); 
    return;
}


typedef struct ionbuf {
    int ion_fd;
    int buf_fd;
    unsigned int len;
    struct ion_handle *handle;
    unsigned int phy;
    unsigned int *vir;
} ionbuf_t;

int allocbuf_ion(ionbuf_t *ionbuf)
{
    int err;
    ionbuf->ion_fd = ion_open();
    if (ionbuf->ion_fd < 0) {
        printf("Open ion device failed\n");
    } else {
        //err = ion_alloc(ionbuf->ion_fd, ION_LEN, 4, ION_HEAP(ION_CMA_HEAP_ID),0,&ionbuf->handle);
        err = ion_alloc(ionbuf->ion_fd, ION_LEN, 4, ION_HEAP(4),0,&ionbuf->handle);
        if (err < 0) {
            printf("ion_alloc failed\n");
        } else {
            err = ion_share(ionbuf->ion_fd,ionbuf->handle,&ionbuf->buf_fd);
            if (err < 0) {
                printf("ion share failed\n");
            } else {
                err = ion_get_phys(ionbuf->ion_fd, ionbuf->handle, &ionbuf->phy);
                if (err<0) {
                    printf("ion get phys failed\n");
                } else{
                    printf("ion get phys: 0x%x\n", ionbuf->phy);
                }

                ionbuf->vir = (unsigned int*)mmap(0,ION_LEN,
                        PROT_READ|PROT_WRITE, MAP_SHARED,
                        ionbuf->buf_fd,0);

                if (ionbuf->vir == MAP_FAILED) {
                    printf("ion buffer mmap failed\n");
                } else {
                    printf("ion buffer: %p\n", ionbuf->vir);

                    memset((char*)ionbuf->vir,0x7a, ION_LEN);

                    return 0;
                }
            }
        }
    }

    return -1;
}

int freebuf_ion(ionbuf_t *ionbuf)
{
    if (ionbuf->vir) {
        munmap((void*)ionbuf->vir,ionbuf->len);
        ionbuf->vir = NULL;
    }

    if (ionbuf->buf_fd>0) {
        close(ionbuf->buf_fd);
        ionbuf->buf_fd = -1;
    }

    if (ionbuf->handle != NULL) {
        ion_free(ionbuf->ion_fd, ionbuf->handle);
        ionbuf->handle = NULL;
    }

    if (ionbuf->ion_fd>0) {
        close(ionbuf->ion_fd);
        ionbuf->ion_fd = -1;
    }

    return 0;
}

#define CIF_CTRL            (0/4)
#define CIF_INITSTA         (0x08/4)
#define CIF_FOR             (0x0C/4)
#define CIF_FRM0_Y          (0x14/4)
#define CIF_FRM1_Y          (0x1C/4)
#define CIF_VIR_LINE_W      (0x24/4)
#define CIF_SET_SIZE        (0x28/4)
#define CIF_CROP            (0x44/4)
#define CIF_SCL_CTRL        (0x48/4)
#define CIF_FIFO_ENTRY      (0x54/4)
#define CIF_FRAME_STATUS    (0x60/4)
#define CIF_CUR_DST         (0X64/4)
#define CIF_LAST_LINE       (0x68/4)
#define CIF_LAST_PIX        (0x6C/4)


int RAW8_Dump(unsigned int *raw, unsigned int w, unsigned int h)
{
    unsigned int i,pixels,j;
    unsigned char *src,*dst;
    FILE *fd;
    
    pixels = w*h;
    dst = src = (unsigned char*)raw;
    
    for (i=0; i<(pixels/4); i++) {
        for (j=0; j<4; j++)
            *dst++ = *src++;
        src++;
    }
    
    fd = fopen("/data/raw8.bin","w");
    if (fd==NULL) {
        printf("/data/raw8.bin open failed!\n");
        goto end;
    } else {
        fwrite(raw,1,pixels,fd);
        fclose(fd);
        printf("Write %d bytes to /data/raw8.bin success!\n",pixels);
    }
    
    return 0;  
        
end:
    return 0;
}

void arm_yuyv_to_nv12(int src_w, int src_h,char *srcbuf, char *dstbuf){

    char *srcbuf_begin;
    int *dstint_y, *dstint_uv, *srcint;
    int i = 0,j = 0;
    int y_size = 0;

	y_size = src_w*src_h;
	dstint_y = (int*)dstbuf;
	srcint = (int*)srcbuf;
	dstint_uv =  (int*)(dstbuf + y_size);

    for(i=0;i<src_h; i++) {
		for (j=0; j<(src_w>>2); j++) {
			if(i%2 == 0){
			    *dstint_uv++ = (*(srcint+1)&0xff000000)|((*(srcint+1)&0x0000ff00)<<8)
				        |((*srcint&0xff000000)>>16)|((*srcint&0x0000ff00)>>8);
			 }
			 *dstint_y++ = ((*(srcint+1)&0x00ff0000)<<8)|((*(srcint+1)&0x000000ff)<<16)
			            |((*srcint&0x00ff0000)>>8)|(*srcint&0x000000ff);
		     srcint += 2;
		 }
	 }

}

void arm_uyvy_to_nv12(int src_w, int src_h,char *srcbuf, char *dstbuf){

    char *srcbuf_begin;
    int *dstint_y, *dstint_uv, *srcint;
    int i = 0,j = 0;
    int y_size = 0;

	y_size = src_w*src_h;
	dstint_y = (int*)dstbuf;
	srcint = (int*)srcbuf;
	dstint_uv =  (int*)(dstbuf + y_size);

    for(i=0;i<src_h; i++) {
		for (j=0; j<(src_w>>2); j++) {
			if(i%2 == 1){
			    *dstint_uv++ = ((*(srcint+1)&0x00ff0000)<<8)|((*(srcint+1)&0x000000ff)<<16)
			            |((*srcint&0x00ff0000)>>8)|(*srcint&0x000000ff);
			 }
			 *dstint_y++ = (*(srcint+1)&0xff000000)|((*(srcint+1)&0x0000ff00)<<8)
				        |((*srcint&0xff000000)>>16)|((*srcint&0x0000ff00)>>8);
		     srcint += 2;
		 }
	 }

}




/**add by cx end***/


int Camera_Click_Event(int x,int y)
{	
	struct list_head *pos;
	int x_start,x_end;
	int y_start,y_end;
	int err;

	if(tc_info == NULL)
		return -1;		

	if(camera_num < 2)
		return -1;
	
	get_camera_size();

	x_start = camera_x;
	x_end = x_start + camera_w;
	y_start = camera_y;
	y_end = y_start + camera_h;

	if( (x >= x_start) && (x <= x_end) && (y >= y_start) && (y <= y_end))
	{
		
		printf("Camera_Click_Event : change \r\n");	
		stopCameraTest();
		usleep(100000);
		pthread_create(&camera_tid, NULL, startCameraTest, NULL); 
	}
		
	return 0;
	
}


int CameraCreate(void)
{
    int err,size;
	struct v4l2_format format;

    if (iCamFd == 0) {
        iCamFd = open(videodevice, O_RDWR|O_CLOEXEC);
       
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
    if(v4l2Buffer_phy_addr !=0)
		goto suc_alloc;
    if(access(PMEM_DEV_NAME, O_RDWR) < 0) {
            iIonFd = open(ION_DEVICE, O_RDONLY|O_CLOEXEC);

            if(iIonFd < 0 ) {
                printf("%s: Failed to open ion device - %s",
                        __FUNCTION__, strerror(errno));
                iIonFd = -1;
        		err = -1;
                goto exit1;
            }
            ionAllocData.len = 0x200000;
            ionAllocData.align = 4*1024;
	        ionAllocData.heap_id_mask = ION_HEAP(ION_CMA_HEAP_ID);
			ionAllocData.flags = 0;
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
	    	//err = ioctl(fd_data.fd, PMEM_GET_PHYS, &sub);yzm
	    	phys_data.handle = ionAllocData.handle;
			phys_data.phys = 0;
	    	data.cmd = ION_IOC_GET_PHYS;
			data.arg = (unsigned long)&phys_data;
	    	err = ioctl(iIonFd, ION_IOC_CUSTOM, &data);
	    	if (err < 0) {
	        	printf(" ion get phys_data fail !!!!\n");
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
			v4l2Buffer_phy_addr = phys_data.phys;
        }
    memset(m_v4l2Buffer[0], 0x00, size);
suc_alloc:   
          err = ioctl(iCamFd, VIDIOC_QUERYCAP, &mCamDriverCapability);
        if (err < 0) {
        	printf("Error opening device unable to query device.\n");
    	    goto exit;
        }  
    return 0;

exit3:
	munmap(m_v4l2Buffer[0], size);
exit2:

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
//	memset(m_v4l2Buffer[i],0x0,buffer.length);
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



/*int DispCreate(int corx ,int cory,int preview_w,int preview_h )
{
	int err = 0;
	struct fb_var_screeninfo var;
	unsigned int panelsize[2];
	int x_phy,y_phy,w_phy,h_phy;
	int x_visual,y_visual,w_visual,h_visual;
	struct fb_fix_screeninfo finfo;
	struct color_key_cfg clr_key_cfg;

	int data[2];
	if(iDispFd !=-1)
		goto exit;
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
	#if 0
	data[0] = v4l2Buffer_phy_addr;
	data[1] = (int)(data[0] + preview_w *preview_h);
	if (ioctl(iDispFd, 0x5002, data) == -1) 
	{
		printf("%s ioctl fb1 queuebuf fail!\n",__FUNCTION__);
		err = -1;
		goto exit;
	}
	#endif
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
	
	clr_key_cfg.win0_color_key_cfg = 0;		//win0 color key disable
	clr_key_cfg.win1_color_key_cfg = 0x01000000; 	// win1 color key enable
	clr_key_cfg.win2_color_key_cfg = 0;  
	if (ioctl(iDispFd,RK_FBIOPUT_COLOR_KEY_CFG, &clr_key_cfg) == -1) {
                printf("%s set fb color key failed!\n",__FUNCTION__);
                err = -1;
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
}*/

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
   //     return -1;
    }
	if (iDispFd > 0) {
		int disable = 0;
		printf("Close disp\n");
		ioctl(iDispFd, FBIOSET_ENABLE,&disable);
		close(iDispFd);
		iDispFd = -1;
	}
	if (iCamFd > 0) {
		close(iCamFd);
		iCamFd = 0;
	}
	printf("\n%s: stop ok!\n",__func__);
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
			if (ioctl(iDispFd,RK_FBIOSET_CONFIG_DONE, NULL) < 0) {
        			perror("set config done failed");
    			}


		}
	if (ioctl(iCamFd, VIDIOC_QBUF, &cfilledbuffer1) < 0) {
		printf("%s VIDIOC_QBUF Failed!!!\n",__FUNCTION__);
		err = -1;
		goto exit;
	}

	    fps++;
	}
//	hasstoped = 1;

exit:
	return err;
}
// the func is a while loop func , MUST  run in a single thread.
int startCameraTest(){
	int err = 0, err1 = 0,i = 0;	  
	camsys_sysctrl_t sysctl;	
	camsys_querymem_t qmem;
	camsys_version_t camsys_ver;
	pthread_t thread[3];
	unsigned int m_Y = 0,m_Cr,m_Cb = 0,*regbase = 0,*i2cbase = 0,*bufbase = 0;	
	FILE *image;	   
	MrvAllRegister_t *pMrvReg;
	int numLane;
	ionbuf_t ionbuf;  
	int sleep_time = 1000;
	unsigned int *phy_addr = 0;
	unsigned int vsync_flag = 0;
	char test_result[200] = "";
	char tmp[100];
	
	if(tc_info->y <= 0)
		tc_info->y  = get_cur_print_y();

	ui_print_xy_rgba(0, tc_info->y , 255, 255, 0, 255, "%s:[%s..]\n", PCBA_CAMERA,
			 PCBA_TESTING);
	
	printf("%s %d back camera start test\n",__FUNCTION__,__LINE__);
	if (!strcmp(BACK_SENSOR_TYPE, "MIPI"))
		phy_type = CamSys_Phy_Mipi;
	else
		phy_type = CamSys_Phy_Cif;

	if (!strcmp(BACK_SENSOR_NAME, "ov8858"))
		Ov8858_getResolution(&image_w,&image_h);
	else
		printf("%s getResolution fail\n", BACK_SENSOR_NAME);
	printf("Camera %s phy_type:%d image_w_h:%dx%d\n", BACK_SENSOR_NAME,
		phy_type, image_w, image_h);
	
	ionbuf.ion_fd = -1;
	ionbuf.buf_fd = -1;
	ionbuf.handle = NULL;
	ionbuf.phy = 0;
	ionbuf.vir = NULL;
	ionbuf.len = ION_LEN;
	err = allocbuf_ion(&ionbuf);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);
	if (err) {
		printf("allocbuf_ion  failed\n");
		goto Next;
	}
 
	camsys_fd = open(CAMSYS_DEPATH_MARVIN,O_RDWR);
	printf("camsys_fd:%d\n", camsys_fd);
	if (camsys_fd < 0) {
		err = -1;
		printf("Open %s  failed\n", CAMSYS_DEPATH_MARVIN);
		goto Next;
	}
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

	qmem.mem_type = CamSys_Mmap_RegisterMem;
	err = ioctl(camsys_fd, CAMSYS_QUREYMEM, &qmem);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);	
	if (err<0) {
		printf("CamSys_Mmap_RegisterMem query failed,%d\n",err);
		goto Next;
	} else {
		printf("regmem size: 0x%x	offset: 0x%lx\n",qmem.mem_size,qmem.mem_offset);
		regbase = (unsigned int*)mmap(0,qmem.mem_size,
						PROT_READ|PROT_WRITE, MAP_SHARED,
						camsys_fd,qmem.mem_offset);
		if (regbase == MAP_FAILED) {
			printf("register memory mmap failed!\n");
		} else {
			printf("register memory mmap success!\n");
			pMrvReg = (MrvAllRegister_t*)regbase;
		}		 
	}
		
	qmem.mem_type = CamSys_Mmap_I2cMem;
	err = ioctl(camsys_fd, CAMSYS_QUREYMEM, &qmem);
	if (err<0) {
		printf("CamSys_Mmap_I2cMem query failed\n");
		goto Next;
	} else {
		printf("i2cmem size: 0x%x	offset: 0x%lx\n",qmem.mem_size,qmem.mem_offset);
		i2cbase = (unsigned int*)mmap(0,qmem.mem_size,
						PROT_READ|PROT_WRITE, MAP_SHARED,
						camsys_fd,qmem.mem_offset);
		if (i2cbase == MAP_FAILED) {
			printf("i2c memory mmap failed!\n");
		} else {
			printf("i2cbase: %p   i2cbase[0]: 0x%x \n", i2cbase,*i2cbase);
		}		  
	}

	numLane = MIPI_LANE_NUM;
	err = extdev_register(numLane);
	if (err < 0) {
		printf("extdev_register failed.\n");
		goto Next;
	}
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);	   
	
	err = extdev_init(camsys_fd,i2cbase);
	if (err < 0) {
		printf("extdev_init failed.\n");
		goto Next;
	}
	phy_addr = (unsigned int)ionbuf.phy;
	marvin_config((MrvAllRegister_t*)regbase, phy_addr, numLane);
	marvin_start((MrvAllRegister_t*)regbase);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
	   
	usleep(100000);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
	
	ioctl(camsys_fd, CAMSYS_VERCHK, &camsys_ver);
	if (camsys_ver.head_ver == CAMSYS_HEAD_VERSION) {
		printf("CamSys head version: v%d.%d.%d\n",(camsys_ver.head_ver&0xff0000)>>16,
			(camsys_ver.head_ver&0xff00)>>8,(camsys_ver.head_ver&0xff));
	} else {
		printf("CamSys head version check failed!User: v%d.%d.%d Kernel: v%d.%d.%d\n",
			(CAMSYS_HEAD_VERSION&0xff0000)>>16,
			(CAMSYS_HEAD_VERSION&0xff00)>>8,(CAMSYS_HEAD_VERSION&0xff),
			(camsys_ver.head_ver&0xff0000)>>16,
			(camsys_ver.head_ver&0xff00)>>8,(camsys_ver.head_ver&0xff));

	}
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__); 

	if (!strcmp(BACK_SENSOR_NAME, "ov8858"))
		err = Ov8858_sensor_streamon(camsys_fd,1);
	else
		printf("%s sensor streamon not ok\n", BACK_SENSOR_NAME);
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

	usleep(1000000);

	for(i=0;i<200;i++){
		if(marvin_check_vsync((MrvAllRegister_t *) pMrvReg)){
			vsync_flag = vsync_flag | 0x1;
		} else {
			vsync_flag = vsync_flag | 0x2;
		}
		if(vsync_flag && 0x3) {
			printf("%s %d %s ISP input data is successed!\n",__FUNCTION__,__LINE__,
				BACK_SENSOR_NAME);
			err = 0;
			break;
		}
		usleep(10000);//1fps(200*10ms/2)<framrate<100fps(10ms)
	}
	if (i == 200) {
		err = -1;
		goto Next;
	}


	FILE *fd;
	char filename[64];
	sprintf(filename,"/data/back_camera_%d_%d_yuv.bin",image_w,image_h);
	fd = fopen(filename,"wb");
	if(fd){
		fwrite(ionbuf.vir,0x01,image_w*image_h*2,fd);
		fclose(fd);
		printf("yuyv image write to file\n");
	} else {
		printf("/data/back_camera_%d_%d_yuv.bin open failed!\n",image_w,image_h);
		goto Next;
	}

#if 0	
	phy_addr = (void*)((unsigned long)ionbuf.vir + image_w*image_h*2);	
	arm_yuyv_to_nv12(image_w,image_h,(char*)ionbuf.vir,(char*)phy_addr);

	//FILE *fd;
	//char filename[64];
	sprintf(filename,"/data/%d_%d_nv12.bin",image_w,image_h);
	fd = fopen(filename,"wb");
	if(fd){
		fwrite(phy_addr,0x01,image_w*image_h*3/2,fd);
		fclose(fd);
		 printf("nv12 image write to file\n");
	} else {
		printf("/data/%d_%d_nv12.bin open failed!\n",image_w,image_h);
		goto end;
	}

#endif

	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);	   
	//thread_disconnect(thread);
	marvin_stop((MrvAllRegister_t*)regbase);

	
	if (regbase) {
		munmap((void*)regbase, qmem.mem_size);
		regbase = NULL;
	}

	if (i2cbase) {
		munmap((void*)i2cbase, qmem.mem_size);
		i2cbase = NULL;
	}
	
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
	sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1A;
	sysctl.ops = CamSys_PwrDn;
	sysctl.on = 1;
	err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
	if (err<0) {
		printf("CamSys_PwrDn off failed");
		//goto end;
	}
	
	sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1A;
	sysctl.ops = CamSys_Phy;
	sysctl.on = 0;
	err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
	if (err<0) {
		printf("CamSys_Phy off failed");
		//goto end;
	}
	
	sysctl.dev_mask = (CAMSYS_DEVID_MARVIN|CAMSYS_DEVID_SENSOR_1A);
	sysctl.ops = CamSys_ClkIn;
	sysctl.on = 0;

	err = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
	if (err<0) {
		printf("CamSys_ClkIn off failed\n");
		//goto end;
	}


Next:		
	

	/*err = close(camsys_fd);
	printf("close camsys result:%d\n", err);
	if (err)
		printf("close camsys error: %d, errno:%d, %s\n", err,errno,strerror(errno));
	camsys_fd = 0;*/
	printf("%s %d  back camera test over.\n",__FUNCTION__,__LINE__);

	sprintf(tmp, "Back Camera:[%s] { ID:0x%x }", err < 0 ? PCBA_FAILED : PCBA_SECCESS,0);
	strcat(test_result, tmp);


	
	if (camera_num > 1) {
		usleep(1000000);
		freebuf_ion(&ionbuf);
		
		printf("%s %d front camera start test\n",__FUNCTION__,__LINE__);
		if (!strcmp(FRONT_SENSOR_TYPE, "MIPI"))
			phy_type = CamSys_Phy_Mipi;
		else
			phy_type = CamSys_Phy_Cif;

		if (!strcmp(FRONT_SENSOR_NAME, "gc2155"))
			Gc2155_getResolution(&image_w,&image_h);
		else if (!strcmp(FRONT_SENSOR_NAME, "gc2145"))
			Gc2145_getResolution(&image_w,&image_h);
		else
			printf("%s getResolution fail\n", FRONT_SENSOR_NAME);
		printf("Camera %s phy_type:%d image_w_h:%dx%d\n", FRONT_SENSOR_NAME,
			phy_type, image_w, image_h);
	
		ionbuf.ion_fd = -1;
		ionbuf.buf_fd = -1;
		ionbuf.handle = NULL;
		ionbuf.phy = 0;
		ionbuf.vir = NULL;
		ionbuf.len = ION_LEN;
		err1 = allocbuf_ion(&ionbuf);
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);
		if (err1) {
			printf("allocbuf_ion  failed\n");
			goto end;
		}
 
		/*camsys_fd = open(CAMSYS_DEPATH_MARVIN,O_RDWR);
		if (camsys_fd < 0) {
			err1 = -1;
			printf("Open %s  failed\n", CAMSYS_DEPATH_MARVIN);
			goto end;
		}*/
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

		qmem.mem_type = CamSys_Mmap_RegisterMem;
		err1 = ioctl(camsys_fd, CAMSYS_QUREYMEM, &qmem);
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);	
		if (err1<0) {
			printf("CamSys_Mmap_RegisterMem query failed,%d\n",err1);
			goto end;
		} else {
			printf("regmem size: 0x%x	offset: 0x%lx\n",qmem.mem_size,qmem.mem_offset);
			regbase = (unsigned int*)mmap(0,qmem.mem_size,
							PROT_READ|PROT_WRITE, MAP_SHARED,
							camsys_fd,qmem.mem_offset);
			if (regbase == MAP_FAILED) {
				printf("register memory mmap failed!\n");
			} else {
				printf("register memory mmap success!\n");
				pMrvReg = (MrvAllRegister_t*)regbase;
			}		 
		}
		
		qmem.mem_type = CamSys_Mmap_I2cMem;
		err1 = ioctl(camsys_fd, CAMSYS_QUREYMEM, &qmem);
		if (err1<0) {
			printf("CamSys_Mmap_I2cMem query failed\n");
			goto end;
		} else {
			printf("i2cmem size: 0x%x	offset: 0x%lx\n",qmem.mem_size,qmem.mem_offset);
			i2cbase = (unsigned int*)mmap(0,qmem.mem_size,
							PROT_READ|PROT_WRITE, MAP_SHARED,
							camsys_fd,qmem.mem_offset);
			if (i2cbase == MAP_FAILED) {
				printf("i2c memory mmap failed!\n");
			} else {
				printf("i2cbase: %p   i2cbase[0]: 0x%x \n", i2cbase,*i2cbase);
			}		  
		}

		numLane = MIPI_LANE_NUM;
		err1 = extdev_register_front(numLane);
		if (err1 < 0) {
			printf("extdev_register failed.\n");
			goto end;
		}
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);	   
		
		err1 = extdev_init_front(camsys_fd,i2cbase);
		if (err1 < 0) {
			printf("extdev_init failed.\n");
			goto end;
		}
		phy_addr = (unsigned int)ionbuf.phy;
		marvin_config((MrvAllRegister_t*)regbase, phy_addr, numLane);
		marvin_start((MrvAllRegister_t*)regbase);
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

	    usleep(1000000);
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
		
		ioctl(camsys_fd, CAMSYS_VERCHK, &camsys_ver);
		if (camsys_ver.head_ver == CAMSYS_HEAD_VERSION) {
			printf("CamSys head version: v%d.%d.%d\n",(camsys_ver.head_ver&0xff0000)>>16,
				(camsys_ver.head_ver&0xff00)>>8,(camsys_ver.head_ver&0xff));
		} else {
			printf("CamSys head version check failed!User: v%d.%d.%d Kernel: v%d.%d.%d\n",
				(CAMSYS_HEAD_VERSION&0xff0000)>>16,
				(CAMSYS_HEAD_VERSION&0xff00)>>8,(CAMSYS_HEAD_VERSION&0xff),
				(camsys_ver.head_ver&0xff0000)>>16,
				(camsys_ver.head_ver&0xff00)>>8,(camsys_ver.head_ver&0xff));

		}
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__); 

		if (!strcmp(FRONT_SENSOR_NAME, "gc2155"))
			err1 = Gc2155_sensor_streamon(camsys_fd,1);
		else if (!strcmp(FRONT_SENSOR_NAME, "gc2145"))
			err1 = Gc2145_sensor_streamon(camsys_fd,1);
		else
			printf("%s snesor streamon fail\n", FRONT_SENSOR_NAME);
		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  

		usleep(1000000);

		for(i=0;i<200;i++){
			if(marvin_check_vsync((MrvAllRegister_t *) pMrvReg)){
				vsync_flag = vsync_flag | 0x1;
			} else {
				vsync_flag = vsync_flag | 0x2;
			}
			if(vsync_flag && 0x3) {
				printf("%s %d %s ISP input data is successed!\n",__FUNCTION__,__LINE__,
					FRONT_SENSOR_NAME);
				err1 = 0;
				break;
			}
			usleep(10000);//1fps(200*10ms/2)<framrate<100fps(10ms)
		}
		if (i == 200) {
			err1 = -1;
			goto end;
		}

		FILE *fd;
		char filename[64];
		sprintf(filename,"/data/front_camera_%d_%d_yuv.bin",image_w,image_h);
		fd = fopen(filename,"wb");
		if(fd){
			fwrite(ionbuf.vir,0x01,image_w*image_h*2,fd);
			fclose(fd);
			 printf("yuyv image write to file\n");
		} else {
			printf("/data/back_camera_%d_%d_yuv.bin open failed!\n",image_w,image_h);
			goto end;
		}

#if 0	
		phy_addr = (void*)((unsigned long)ionbuf.vir + image_w*image_h*2);	
		arm_yuyv_to_nv12(image_w,image_h,(char*)ionbuf.vir,(char*)phy_addr);

		//FILE *fd;
		//char filename[64];
		sprintf(filename,"/data/%d_%d_nv12.bin",image_w,image_h);
		fd = fopen(filename,"wb");
		if(fd){
			fwrite(phy_addr,0x01,image_w*image_h*3/2,fd);
			fclose(fd);
			 printf("nv12 image write to file\n");
		} else {
			printf("/data/%d_%d_nv12.bin open failed!\n",image_w,image_h);
			goto end;
		}

#endif

		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);	   
		//thread_disconnect(thread);
		marvin_stop((MrvAllRegister_t*)regbase);

		
		if (regbase) {
			munmap((void*)regbase, qmem.mem_size);
			regbase = NULL;
		}

		if (i2cbase) {
			munmap((void*)i2cbase, qmem.mem_size);
			i2cbase = NULL;
		}

		printf("%s %d  hcc\n",__FUNCTION__,__LINE__);  
		sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1B;
		sysctl.ops = CamSys_PwrDn;
		sysctl.on = 1;
		err1 = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
		if (err1<0) {
			printf("CamSys_PwrDn off failed");
			//goto end;
		}
		
		sysctl.dev_mask = CAMSYS_DEVID_SENSOR_1B;
		sysctl.ops = CamSys_Phy;
		sysctl.on = 0;
		err1 = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
		if (err1<0) {
			printf("CamSys_Phy off failed");
			//goto end;
		}
		
		sysctl.dev_mask = (CAMSYS_DEVID_MARVIN|CAMSYS_DEVID_SENSOR_1B);
		sysctl.ops = CamSys_ClkIn;
		sysctl.on = 0;

		err1 = ioctl(camsys_fd, CAMSYS_SYSCTRL, &sysctl);
		if (err1<0) {
			printf("CamSys_ClkIn off failed\n");
			//goto end;
		}
		
		freebuf_ion(&ionbuf);

		if (camsys_fd) {
			close(camsys_fd);	  
			camsys_fd = 0;
		}
		printf("%s %d  front camera test over\n",__FUNCTION__,__LINE__);
		
	}

end:
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);
	freebuf_ion(&ionbuf);
    printf("%s %d  hcc\n",__FUNCTION__,__LINE__);
	if (camsys_fd) {
		close(camsys_fd);	  
		camsys_fd = 0;
	}
    printf("%s %d  hcc\n",__FUNCTION__,__LINE__);
	if (camera_num > 1) {
		sprintf(tmp, ",Front Camera:[%s] { ID:0x%x }", err1 < 0 ? PCBA_FAILED : PCBA_SECCESS,1);
		strcat(test_result, tmp);
	}
	printf("%s %d  hcc test_result:%s\n",__FUNCTION__,__LINE__,test_result);
	if (camera_num > 1) {
                if (err < 0 || err1 < 0)
                   ui_print_xy_rgba(0,tc_info->y,255,0,0,255, "Back Camera:[%s] { ID:0x%x } Front Camera:[%s] { ID:0x%x }\n",err < 0 ? PCBA_FAILED : PCBA_SECCESS,0
                        ,err1 < 0 ? PCBA_FAILED : PCBA_SECCESS,1);
                else
		    ui_print_xy_rgba(0,tc_info->y,0,255,0,255, "Back Camera:[%s] { ID:0x%x } Front Camera:[%s] { ID:0x%x }\n",err < 0 ? PCBA_FAILED : PCBA_SECCESS,0
			,err1 < 0 ? PCBA_FAILED : PCBA_SECCESS,1);
	} else {
                if (err < 0 || err1 < 0)
                    ui_print_xy_rgba(0,tc_info->y,255,0,0,255, "Back Camera:[%s] { ID:0x%x }\n",err < 0 ? PCBA_FAILED : PCBA_SECCESS,0);
                else
		    ui_print_xy_rgba(0,tc_info->y,0,255,0,255, "Back Camera:[%s] { ID:0x%x }\n",err < 0 ? PCBA_FAILED : PCBA_SECCESS,0);
	}
	printf("%s %d  hcc\n",__FUNCTION__,__LINE__);
	//exit(0);
	printf("%s %d camera test over\n",__FUNCTION__,__LINE__);	
	return 0;

/*	  
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
				tc_info->result = -1;
				printf("%s display create wrong!\n",__FUNCTION__);
			}
		}
		else
		{
			tc_info->result = -1;
			printf("%s camera start erro\n",__FUNCTION__);
		}
	}
	else
	{
		tc_info->result = -1;
		printf("%s camera create erro\n",__FUNCTION__);
	}
	//isstoped = 1;
	hasstoped = 1;
	*/
	//printf("camrea%d test over\n",cameraId);
	//return 0;
}

int stopCameraTest(){
	
	sprintf(videodevice,"/dev/video%d",(cam_id%2));
	if(access(videodevice, O_RDWR) <0 ){
	   printf("access %s failed,so dont't switch to camera %d\n",videodevice,(cam_id%2));
	   //recover videodevice
	   sprintf(videodevice,"/dev/video%d",(1-(cam_id%2)));
	   return 0;
	 }
	printf("%s enter stop -----\n",__func__);
	return TaskStop();
}
void finishCameraTest(){
		TaskStop();
	
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
		
}

int get_camera_size()
{
	if(camera_x>0 && camera_y>0 && camera_w>0 && camera_h >0)
		return 0;	

	if(gr_fb_width() > gr_fb_height()){
		camera_w = ((gr_fb_width() >> 1) & ~0x03);//camera_msg->w;
		camera_h = ((gr_fb_height()*2/3) & ~0x03);// camera_msg->h;
	}
	else{
		camera_h = ((gr_fb_width() >> 1) & ~0x03);//camera_msg->w;
		camera_w = ((gr_fb_height()*2/3) & ~0x03);// camera_msg->h;
	}

	if(camera_w > 640)
		camera_w = 640;
	if(camera_h > 480)
		camera_h=480;			
	
	camera_x = gr_fb_width() >> 1; 	
	camera_y = 0;

	return 0;
}


void * camera_test(void *argc)
{
	int ret,num;
	printf("------------enter camera_test------- \n");
	tc_info = (struct testcase_info *)argc; 

	if (script_fetch("camera", "number",&num, 1) == 0) {
		printf("camera_test num:%d\r\n",num);
		camera_num = num;	
	}

	pthread_create(&camera_tid, NULL, startCameraTest, NULL); 
		
	return argc;
}

