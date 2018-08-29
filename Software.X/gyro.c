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

extern struct s_gyro g_cal_gyro;
extern struct s_gyro g_degres_gyro;

extern u16 g_mtime;

void	movement_gyro(struct s_gyro *gyro)
{
	u16 count = 0;
        u8 i = 0;
	struct s_gyro sample;

        /* Echantillonnage */
	while (count <= AVERAGE_SAMPLE_NUMBER) {
		memset(&sample, 0, sizeof(struct s_gyro));
		read_gyro(&sample);
		gyro->gyroX += sample.gyroX;
		gyro->gyroY += sample.gyroY;
		gyro->gyroZ += sample.gyroZ;
		++count;
	}
	gyro->gyroX /= AVERAGE_SAMPLE_NUMBER;
	gyro->gyroY /= AVERAGE_SAMPLE_NUMBER;
	gyro->gyroZ /= AVERAGE_SAMPLE_NUMBER;

    /* calibration */
    gyro->gyroX -= g_cal_gyro.gyroX;
	gyro->gyroY -= g_cal_gyro.gyroY;
	gyro->gyroZ -= g_cal_gyro.gyroZ;

    /* Filtre */
    if (INVALID_VALUE_GYRO(gyro->gyroX))
    {
		gyro->gyroX = 0;
        i = 1;
    }
	if (INVALID_VALUE_GYRO(gyro->gyroY))
 		gyro->gyroY = 0;
	if (INVALID_VALUE_GYRO(gyro->gyroZ))
 		gyro->gyroZ = 0;

        /* Transformation en degres */
        if (i && (gyro->gyroX != 0 || gyro->gyroY != 0 || gyro->gyroZ != 0))
        {
            g_degres_gyro.gyroX += (250.0 * (gyro->gyroX / 32768.0));                     //    (TRANS_GYRO_TO_DEGRE(gyro->gyroX));// * g_mtime) / 1000);
            g_degres_gyro.gyroY += (TRANS_GYRO_TO_DEGRE(gyro->gyroY));// * g_mtime) / 1000);
            g_degres_gyro.gyroZ += (TRANS_GYRO_TO_DEGRE(gyro->gyroZ));// * g_mtime) / 1000);
            i = 0;
        }


//        uart2_putstr("gyroscope : \n\r");
//        print_gyro(gyro);

//    uart2_putstr("gyroscope degres : \n\r");
//    print_gyro(&g_degres_gyro);
}

void calibration_gyroscope(u8 calibration_sample_number) {
	u16 count = 0;
	struct s_gyro sample;

	while (count <= calibration_sample_number) {
		memset(&sample, 0, sizeof(struct s_gyro));
		read_gyro(&sample);
		g_cal_gyro.gyroX += sample.gyroX;
		g_cal_gyro.gyroY += sample.gyroY;
		g_cal_gyro.gyroZ += sample.gyroZ;
		++count;
	}
	g_cal_gyro.gyroX /= calibration_sample_number;
	g_cal_gyro.gyroY /= calibration_sample_number;
	g_cal_gyro.gyroZ /= calibration_sample_number;
//        uart2_putstr("Calibration gyroscope : \n\r");
//        print_gyro(&g_cal_gyro);
}

/*
* This function calculate gyroscope value less calibration number
*/

void check_gyroscope_position(struct s_gyro *gyro) {
	struct s_gyro ctrl;

	read_gyro(&ctrl);
	g_xctrl = ctrl.gyroX - gyro->gyroX;
	g_yctrl = ctrl.gyroY - gyro->gyroY;
	g_zctrl = ctrl.gyroZ - gyro->gyroZ;
}