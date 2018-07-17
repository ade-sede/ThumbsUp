/* 
 * File:   MPU9150.h
 * Author: bocal
 *
 * Created on July 13, 2018, 3:56 PM
 */

#ifndef MPU9150_H
# define MPU9150_H
# include "header.h"


/*
 * Struct used to receive the data from the MPU9150
 * Each field corresponds to a register
 */
struct s_data {
	u8 accelX_LOW;
	u8 accelX_HIGH;
	u8 accelY_LOW;
	u8 accelY_HIGH;
	u8 accelZ_LOW;
	u8 accelZ_HIGH;
	s16 accelX;
	s16 accelY;
	s16 accelZ;
};

struct s_fifo {
	u8 fifo_rw;
	u8 fifo_count_h;
	u8 fifo_count_l;
};

/* Local values we use */
#define PWR_MGMT_ON_NO_TEMP 0b00001000  /* Enables power management, disable temp. sensor */
#define ACCEL_FIFO_EN 0b00001000/* Accel data store in fifo register */



/* Registers and their address */
#define ACCEL_XOUT_L 0x3C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_L 0x3E
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_L 0x40
#define ACCEL_ZOUT_H 0x3F
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SLAVE_ADDR 0x68
#define FIFO_EN 0x23
#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74

void config_MPU9150_register(u8 register_name, u8 bit_config);

#endif	/* MPU9150_H */

