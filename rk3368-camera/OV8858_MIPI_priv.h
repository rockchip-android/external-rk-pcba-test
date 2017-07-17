#ifndef __OV8858_H__
#define __OV8858_H__
#define IMAGE_W							1632
#define IMAGE_H							1224
#define SENSOR_NAME						"OV8858"
#define MIPI_LANE_NUM					2

int Ov8858_sensor_reg_init(int camsys_fd,unsigned int *i2cbase);
int Ov8858_sensor_streamon(int camsys_fd,unsigned int on);
void Ov8858_getResolution(int *width, int *height);
#endif

