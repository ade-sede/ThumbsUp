#include "header.h"
#include "MPU9150.h"
#include "movement.h"
#include "RN42.h"

extern struct s_accel g_accel_bias;
extern struct s_gyro g_gyro_bias;

extern struct s_accel g_accel[2];
extern struct s_velocity g_velocity[2];
extern struct s_gyro g_gyro[2];

/*
** If the accel is null during too long, we consider velocity is null
*/

static void	check_no_movement() {
	static u32 countX = 0;
	static u32 countY = 0;
	static u32 countZ = 0;

	if (g_accel[CURR].accelX == 0)
		++countX;
	if (countX >= NO_ACCEL_TRESHOLD) {
		g_velocity[CURR].velocityX = 0;
		g_velocity[PREV].velocityX = 0;
		countX = 0;
	}
 
	if (g_accel[CURR].accelY == 0)
		++countY;
	if (countY >= NO_ACCEL_TRESHOLD) {
		g_velocity[CURR].velocityY = 0;
		g_velocity[PREV].velocityY = 0;
		countY = 0;
	}
	 
	if (g_accel[CURR].accelZ == 0)
		++countZ;
	if (countZ >= NO_ACCEL_TRESHOLD) {
		g_velocity[CURR].velocityZ = 0;
		g_velocity[PREV].velocityZ = 0;
		countZ = 0;
	}
}

/* 
** This function is meant to:
** 1) Compute any movement
** 2) Send it to the bluetooth module.
**
** To make sure the measurements are meaningful,
** this code make 2 things :
** 1) Average the samples taken, AVERAGE_SAMPLE_NUMBER time
** 2) Apply a discrimination window, that every measurement
** comprised in a window going from WINDOW_LOW and WINDOW_HIGH
** is not considered a valid acceleration and is seen as accel = 0
**
** accel is an array len 2. Index 0 holds info about the previously
** measured accel. Index 1 about the current accel.
**
** velocity works on the same model
**
** It is this function's responsibility to :
** 1) At the end of processing, shift the current accel to the previous
** 2) Clean the space for the next measurement.
*/
void	movement(void) {
	u16 count = 0;
	struct s_accel sample;
	struct s_gyro sample_gyro;
	 
	/* SAMPLING */
	while (count <= AVERAGE_SAMPLE_NUMBER) {
		memset(&sample, 0, sizeof(struct s_velocity));
		read_accel(&sample);
		g_accel[CURR].accelX += sample.accelX;
		g_accel[CURR].accelY += sample.accelY;
		g_accel[CURR].accelZ += sample.accelZ;
		read_gyro(&sample_gyro);
		g_gyro[CURR].gyroX += sample_gyro.gyroX;
		g_gyro[CURR].gyroY += sample_gyro.gyroY;
		g_gyro[CURR].gyroZ += sample_gyro.gyroZ;
		++count;
	}
	/* Retrieving average */
	g_accel[CURR].accelX /= AVERAGE_SAMPLE_NUMBER;
	g_accel[CURR].accelY /= AVERAGE_SAMPLE_NUMBER;
	g_accel[CURR].accelZ /= AVERAGE_SAMPLE_NUMBER;
	g_gyro[CURR].gyroX /= AVERAGE_SAMPLE_NUMBER;
	g_gyro[CURR].gyroY /= AVERAGE_SAMPLE_NUMBER;
	g_gyro[CURR].gyroZ /= AVERAGE_SAMPLE_NUMBER;

	/* Remove priously calibrated values */
	g_accel[CURR].accelX -= g_accel_bias.accelX;
	g_accel[CURR].accelY -= g_accel_bias.accelY;
	g_accel[CURR].accelZ -= g_accel_bias.accelZ;
	g_gyro[CURR].gyroX -= g_gyro_bias.gyroX;
	g_gyro[CURR].gyroY -= g_gyro_bias.gyroY;
	g_gyro[CURR].gyroZ -= g_gyro_bias.gyroZ;

	/*
	** Interpret acceleration close to 0, as if they were 0
	** Everything beetwen window _low and window_high is considered to be 0.
	** Window_low is a negative integer, window_high a positive one
	*/
	if (INVALID_VALUE(g_accel[CURR].accelX))
 		g_accel[CURR].accelX = 0;
	if (INVALID_VALUE(g_accel[CURR].accelY))
 		g_accel[CURR].accelY = 0;
	if (INVALID_VALUE(g_accel[CURR].accelZ))
 		g_accel[CURR].accelZ = 0;

//	if (INVALID_VALUE(g_gyro[CURR].gyroX))
// 		g_gyro[CURR].gyroX = 0;
//	if (INVALID_VALUE(g_gyro[CURR].gyroY))
// 		g_gyro[CURR].gyroY = 0;
//	if (INVALID_VALUE(g_gyro[CURR].gyroZ))
// 		g_gyro[CURR].gyroZ = 0;

	/* Integration */
	g_velocity[CURR].velocityX = g_velocity[PREV].velocityX + g_accel[PREV].accelX + ((g_accel[CURR].accelX - g_accel[PREV].accelX) / 2);
	g_velocity[CURR].velocityY = g_velocity[PREV].velocityY + g_accel[PREV].accelY + ((g_accel[CURR].accelY - g_accel[PREV].accelY) / 2);
	g_velocity[CURR].velocityZ = g_velocity[PREV].velocityZ + g_accel[PREV].accelZ + ((g_accel[CURR].accelZ - g_accel[PREV].accelZ) / 2);

	/*
	** Movement = Velocity * time, but as we sample at a regular time we can
	** consider time to be 1. Thus :
	** Movement = Velocity
    */

	/* Output */

	if (g_velocity[CURR].velocityX != 0 || g_velocity[CURR].velocityY != 0 || g_velocity[CURR].velocityZ != 0){
		uart2_putstr("Velocity / Movement\n\r");
		print_velocity(g_velocity[CURR]);
		print_gyro(g_gyro[CURR]);
	}

	send_report(create_report(g_velocity[CURR].velocityX, g_velocity[CURR].velocityY));
	check_no_movement();

	/* Curr becomes prev */
	memcpy(&g_gyro[PREVIOUS], &g_gyro[CURRENT], sizeof(struct s_gyro));
	memcpy(&g_velocity[PREVIOUS], &g_velocity[CURRENT], sizeof(struct s_velocity));
	memcpy(&g_accel[PREVIOUS], &g_accel[CURRENT], sizeof(struct s_accel));

	/* Blank spot for new values */
	memset(&g_gyro[CURR], 0, sizeof(struct s_gyro));
	memset(&g_velocity[CURR], 0, sizeof(struct s_velocity));
	memset(&g_accel[CURR], 0, sizeof(struct s_accel));
}
