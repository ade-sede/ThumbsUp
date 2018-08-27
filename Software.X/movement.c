#include "header.h"
#include "MPU9150.h"
#include "movement.h"
#include "RN42.h"

extern s32 g_xbias;
extern s32 g_ybias;
extern s32 g_zbias;

extern s32 g_xctrl;
extern s32 g_yctrl;
extern s32 g_zctrl;

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
		++count;
	}
	/* Retrieving average */
	accel[CURR].accelX /= AVERAGE_SAMPLE_NUMBER;
	accel[CURR].accelY /= AVERAGE_SAMPLE_NUMBER;
	accel[CURR].accelZ /= AVERAGE_SAMPLE_NUMBER;

	/* Remove priously calibrated values */
	accel[CURR].accelX -= g_xbias;
	accel[CURR].accelY -= g_ybias;
	accel[CURR].accelZ -= g_zbias;



	/*
	** Interpret acceleration close to 0, as if they were 0
	** Everything beetwen window _low and window_high is considered to be 0.
	** Window_low is a negative integer, window_high a positive one
	*/
//	uart2_putstr("Acceleration before filter\n\r");
//	print_accel(accel[CURR]);
//	char buff[4096];
//        uart2_putstr("Calibration accelerometre : \n\r");
//	sprintf(buff, "%d	%d	%d\n\r", g_xbias, g_ybias, g_zbias);
//	uart2_putstr(buff);

	if (INVALID_VALUE(accel[CURR].accelX))
 		accel[CURR].accelX = 0;
	if (INVALID_VALUE(accel[CURR].accelY))
 		accel[CURR].accelY = 0;
	if (INVALID_VALUE(accel[CURR].accelZ))
 		accel[CURR].accelZ = 0;


//	char buff[4096];
//        uart2_putstr("transf : \n\r");
//	sprintf(buff, "%f	%f	%f\n\r", ((((2.0 * (accel[CURR].accelX)) / 32768.0)) * 9.81), TRANS_ACCEL_TO_MS2(accel[CURR].accelY), TRANS_ACCEL_TO_MS2(accel[CURR].accelZ));
//	uart2_putstr(buff);

//	uart2_putstr("Acceleration after filter\n\r");
//	print_accel(accel[CURR]);
	/* Integration */
	velocity[CURR].velocityX = velocity[PREV].velocityX + accel[PREV].accelX + ((accel[CURR].accelX - accel[PREV].accelX) / 2);
	velocity[CURR].velocityY = velocity[PREV].velocityY + accel[PREV].accelY + ((accel[CURR].accelY - accel[PREV].accelY) / 2);
	velocity[CURR].velocityZ = velocity[PREV].velocityZ + accel[PREV].accelZ + ((accel[CURR].accelZ - accel[PREV].accelZ) / 2);
	
	/*
	** Movement = Velocity * time, but as we sample at a regular time we can
	** consider time to be 1. Thus :
	** Movement = Velocity
    */

	/* Output */

        if (velocity[CURR].velocityX != 0 || velocity[CURR].velocityY != 0 || velocity[CURR].velocityZ != 0){
            uart2_putstr("Velocity / Movement\n\r");
            print_velocity(velocity[CURR]);
            print_ctrl();
        }

	send_report(create_report(TRANS_ACCEL_TO_MS2(velocity[CURR].velocityX), TRANS_ACCEL_TO_MS2(velocity[CURR].velocityY)));
	check_no_movement(accel, velocity);
        
	/* Curr becomes prev */
	memcpy(&velocity[PREVIOUS], &velocity[CURRENT], sizeof(struct s_velocity));
	memcpy(&accel[PREVIOUS], &accel[CURRENT], sizeof(struct s_accel));

	/* Blank spot for new values */
	memset(&velocity[CURR], 0, sizeof(struct s_velocity));
	memset(&accel[CURR], 0, sizeof(struct s_accel));
}
