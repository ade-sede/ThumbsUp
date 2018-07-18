#include "header.h"
#include "MPU9150.h"
#include "movement.h"

extern s16 g_xbias;
extern s16 g_ybias;
extern s16 g_zbias;

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
void	movement(struct s_accel *accel, struct s_velocity *velocity) {
	u16 count = 0;
	struct s_accel sample;
	 
	/* SAMPLING */
	while (count <= AVERAGE_SAMPLE_NUMBER) {
		memset(&sample, 0, sizeof(struct s_velocity));
		read_accel(&sample);
		accel[CURR].accelX += sample.accelX;
		accel[CURR].accelY += sample.accelY;
		accel[CURR].accelZ += sample.accelZ;
	}
	/* Retrieving average */
	accel[CURR].accelX /= AVERAGE_SAMPLE_NUMBER;
	accel[CURR].accelY /= AVERAGE_SAMPLE_NUMBER;
	accel[CURR].accelZ /= AVERAGE_SAMPLE_NUMBER;

	/*
	** Interpret acceleration close to 0, as if they were 0
	** Everything beetwen window _low and window_high is considered to be 0.
	** Window_low is a negative integer, window_high a positive one
	*/

	if (accel[CURR].accelX <= WINDOW_HIGH || accel[CURR].accelX >= WINDOW_LOW)
		accel[CURR].accelX = 0;
	if (accel[CURR].accelY <= WINDOW_HIGH || accel[CURR].accelY >= WINDOW_LOW)
		accel[CURR].accelY = 0;
	if (accel[CURR].accelZ <= WINDOW_HIGH || accel[CURR].accelZ >= WINDOW_LOW)
		accel[CURR].accelZ = 0;
															
	/* Curr becomes prev */
	memcpy(&velocity[PREVIOUS], &velocity[CURRENT], sizeof(struct s_velocity));
	memcpy(&accel[PREVIOUS], &accel[CURRENT], sizeof(struct s_accel));

	/* Blank spot for new values */
	memset(&velocity[CURR], 0, sizeof(struct s_velocity));
	memset(&accel[CURR], 0, sizeof(struct s_accel));
}
