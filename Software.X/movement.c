#include "header.h"
#include "MPU9150.h"
#include "movement.h"

extern s16 g_xbias;
extern s16 g_ybias;
extern s16 g_zbias;

/*
** If the accel is null during too long, we consider velocity is null
*/
static void	check_no_movement(struct s_accel *accel, struct s_velocity *velocity) {
	static u32 countX = 0;
	static u32 countY = 0;
	static u32 countZ = 0;

	if (accel[CURR].accelX == 0)
		++countX;
	if (countX >= NO_ACCEL_TRESHOLD) {
		velocity[CURR].velocityX = 0;
		velocity[PREV].velocityX = 0;
	}
	 
	if (accel[CURR].accelY == 0)
		++countY;
	if (countY >= NO_ACCEL_TRESHOLD) {
		velocity[CURR].velocityY = 0;
		velocity[PREV].velocityY = 0;
	}
	 
	if (accel[CURR].accelZ == 0)
		++countZ;
	if (countZ >= NO_ACCEL_TRESHOLD) {
		velocity[CURR].velocityZ = 0;
		velocity[PREV].velocityZ = 0;
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
e 		accel[CURR].accelX = 0;
	if (accel[CURR].accelY <= WINDOW_HIGH || accel[CURR].accelY >= WINDOW_LOW)
		accel[CURR].accelY = 0;
	if (accel[CURR].accelZ <= WINDOW_HIGH || accel[CURR].accelZ >= WINDOW_LOW)
		accel[CURR].accelZ = 0;

	/* Integration */
	velocity[CURR].velocityX = velocity[PREV].velocityX + accel[CURR].accelX;
	velocity[CURR].velocityY = velocity[PREV].velocityY + accel[CURR].accelY;
	velocity[CURR].velocityZ = velocity[PREV].velocityZ + accel[CURR].accelZ;
															
	/*
	** Movement = Velocity * time, but as we sample at a regular time we can
	** consider time to be 1. Thus :
	** Movement = Velocity
    */

	/* Curr becomes prev */
	memcpy(&velocity[PREVIOUS], &velocity[CURRENT], sizeof(struct s_velocity));
	memcpy(&accel[PREVIOUS], &accel[CURRENT], sizeof(struct s_accel));

	/* Blank spot for new values */
	memset(&velocity[CURR], 0, sizeof(struct s_velocity));
	memset(&accel[CURR], 0, sizeof(struct s_accel));
}
