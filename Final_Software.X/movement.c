#include "header.h"
#include "MPU9150.h"
#include "movement.h"
#include "RN42.h"

extern struct s_accel g_calibration;
extern struct s_gravity g_accel;
extern struct s_gravity g_angle;

extern float g_accelR;

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

        /* giving value between -2 +2 */
	g_accel.accelX = TRANS_ACCEL_TO_G(accel.accelX);
	g_accel.accelY = TRANS_ACCEL_TO_G(accel.accelY);
	g_accel.accelZ = TRANS_ACCEL_TO_G(accel.accelZ);
}

void    angle_calculation(struct s_gravity *arcos){

        float x = 0;
        float y = 0;
        float z = 0;

	g_accelR = sqrt(powf(g_accel.accelX, 2.0)+powf(g_accel.accelY, 2.0)+ powf(g_accel.accelZ, 2.0));

        x = (g_accel.accelX - 1) * 9.81;
        y = (g_accel.accelY - 1) * 9.81;
        z = (g_accel.accelZ - 1) * 9.81;

        print_accelR(g_accel.accelX * 100);
        print_accelR(g_accel.accelY * 100);
        print_accelR(g_accel.accelZ * 100);
        
	/* Give angle in degrees of the vector */
	arcos->accelX = acosf(g_accel.accelX/ g_accelR) * 57.2958;
	arcos->accelY = acosf(g_accel.accelY/ g_accelR) * 57.2958;
	arcos->accelZ = acosf(g_accel.accelZ/ g_accelR) * 57.2958;

	/* calibration */
	arcos->accelX -= g_angle.accelX;
	arcos->accelY -= g_angle.accelY;
	arcos->accelZ -= g_angle.accelZ;
}

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

        sampling_acceleration_value();
        angle_calculation(&arcos);
	/* Output */
//	if (arcos.accelX < 20 && arcos.accelX > -20)
//            arcos.accelX = 0;
//	if (arcos.accelZ < 20 && arcos.accelZ > -20)
//            arcos.accelY = 0;
	send_report(create_report(arcos.accelX / 2, -arcos.accelZ / 2));

        print_arcos(&arcos);
	/* Blank spot for new values */
	memset(&g_accel, 0, sizeof(struct s_accel));
        memset(&arcos, 0, sizeof(struct s_gravity));
}
