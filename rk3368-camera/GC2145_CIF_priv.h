#ifndef __GC2145_MIPI_PRI_H__
#define __GC2145_MIPI_PRI_H__
/*
*              CAMSYS_TEST DRIVER VERSION NOTE
*
*v0.1.0 : gc2145 driver ok, can preview.
*
*/
#define Camsys_Teset_Driver_Version		"Camsys_Test_GC2145_v(0x0,0,1.0)"

#define IMAGE_W							800
#define IMAGE_H							600
#define SENSOR_NAME						"GC2145"
#define MIPI_LANE_NUM					2

int Gc2145_sensor_reg_init(int camsys_fd,unsigned int *i2cbase);
int Gc2145_sensor_streamon(int camsys_fd,unsigned int on);
void Gc2145_getResolution(int *width, int *height);
#endif

