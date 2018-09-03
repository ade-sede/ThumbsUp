#include "header.h"
#include "MPU9150.h"
#include "movement.h"
#include "RN42.h"

extern struct s_accel g_calibration;
extern struct s_gravity g_accel;
extern struct s_gravity g_angle;

extern float g_accelR;

/*
 * This is the function that measures the forces present in the system, 
 * during a no-move condition at the beginnig
 * and shorter during execution
 * We average those measures using calibration_sample_number samples
 */

void calibration(void) {
	u16 count = 0;
	struct s_accel sample_accel;

	while (count <= CALIBRATION_SAMPLE_NUMBER) {
		memset(&sample_accel, 0, sizeof(struct s_accel));
		read_accel(&sample_accel);
		g_calibration.accelX += sample_accel.accelX;
		g_calibration.accelY += sample_accel.accelY;
		g_calibration.accelZ += sample_accel.accelZ;
		++count;
	}

	g_calibration.accelX /= CALIBRATION_SAMPLE_NUMBER;
	g_calibration.accelY /= CALIBRATION_SAMPLE_NUMBER;
	g_calibration.accelZ /= CALIBRATION_SAMPLE_NUMBER;
}

void    accel_to_angle_value() {
	/* accelometre value translate to g value between -/+2 */
	g_accel.accelX = TRANS_ACCEL_TO_G(g_calibration.accelX);
	g_accel.accelY = TRANS_ACCEL_TO_G(g_calibration.accelY);
	g_accel.accelZ = TRANS_ACCEL_TO_G(g_calibration.accelZ);

        /* Vector Acceleration R submitted to gravity */
	g_accelR = sqrt(powf(g_accel.accelX, 2.0) + powf(g_accel.accelY, 2.0) + powf(g_accel.accelZ, 2.0));

	/* Give angle in degrees of the vector */
	g_angle.accelX = acosf(g_accel.accelX/ g_accelR) * 57.2958;
	g_angle.accelY = acosf(g_accel.accelY/ g_accelR) * 57.2958;
	g_angle.accelZ = acosf(g_accel.accelZ/ g_accelR) * 57.2958;
}

void    sampling_acceleration_value() {
	u16 count = 0;
	struct s_accel sample;
        struct s_accel accel;

        memset(&accel, 0, sizeof(struct s_accel));
	/* SAMPLING */
	while (count <= AVERAGE_SAMPLE_NUMBER) {
		memset(&sample, 0, sizeof(struct s_accel));
		read_accel(&sample);
		accel.accelX += sample.accelX;
		accel.accelY += sample.accelY;
		accel.accelZ += sample.accelZ;
		++count;
	}
	/* Retrieving average */
	accel.accelX /= AVERAGE_SAMPLE_NUMBER;
	accel.accelY /= AVERAGE_SAMPLE_NUMBER;
	accel.accelZ /= AVERAGE_SAMPLE_NUMBER;

//	/* Remove priously calibrated values */
//	accel.accelX -= g_calibration.accelX;
//	accel.accelY -= g_calibration.accelY;
//	accel.accelZ -= g_calibration.accelZ;
        /* giving value between -2 +2 */
	g_accel.accelX = TRANS_ACCEL_TO_G(accel.accelX);
	g_accel.accelY = TRANS_ACCEL_TO_G(accel.accelY);
	g_accel.accelZ = TRANS_ACCEL_TO_G(accel.accelZ);
}

void    angle_calculation(struct s_gravity *arcos){
	g_accelR = sqrt(powf(g_accel.accelX, 2.0)+powf(g_accel.accelY, 2.0)+ powf(g_accel.accelZ, 2.0));

	/* Give angle in degrees of the vector */
	arcos->accelX = acosf(g_accel.accelX/ g_accelR) * 57.2958;
	arcos->accelY = acosf(g_accel.accelY/ g_accelR) * 57.2958;
	arcos->accelZ = acosf(g_accel.accelZ/ g_accelR) * 57.2958;

	/* calibration */
	arcos->accelX -= g_angle.accelX;
	arcos->accelY -= g_angle.accelY;
	arcos->accelZ -= g_angle.accelZ;
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
        struct s_gravity arcos;

        memset(&arcos, 0, sizeof(struct s_gravity));

	/*
	** Interpret acceleration close to 0, as if they were 0
	** Everything beetwen window _low and window_high is considered to be 0.
	** Window_low is a negative integer, window_high a positive one
//	*/
//	if (INVALID_VALUE(g_accel[CURR].accelX))
// 		g_accel[CURR].accelX = 0;
//	if (INVALID_VALUE(g_accel[CURR].accelY))
// 		g_accel[CURR].accelY = 0;
//	if (INVALID_VALUE(g_accel[CURR].accelZ))
// 		g_accel[CURR].accelZ = 0;

        calibration();
        sampling_acceleration_value();
        angle_calculation(&arcos);
	/* Output */
//	if (arcos.accelX < 20 && arcos.accelX > -20)
//            arcos.accelX = 0;
//	if (arcos.accelZ < 20 && arcos.accelZ > -20)
//            arcos.accelY = 0;
	//send_report(create_report(arcos.accelX / 2, -arcos.accelZ / 2));
        print_arcos(&arcos);
	/* Blank spot for new values */
	memset(&g_accel, 0, sizeof(struct s_accel));
        memset(&arcos, 0, sizeof(struct s_gravity));
}
