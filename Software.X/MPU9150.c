#include "i2c.h"
#include "header.h"
#include "MPU9150.h"
#include "uart.h"
#include "debug.h"

extern s32 g_xbias;
extern s32 g_ybias;
extern s32 g_zbias;

/*
** Request a read from register addr source, stores
** Result at addr dest
*/

void MPU9150_read(u8 source, u8 *dest) {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(source);
	I2C1CONbits.RSEN = 1;
	while (I2C1CONbits.RSEN == 1)
		Nop();
	i2c_master_send(ADDR_READ_MODE(SLAVE_ADDR));
	i2c_master_receive(dest);
	i2c_master_answer(NACK);
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1)
		Nop();
}

/*
 * This function retrieves all the accelerometers measurement and stores
 * them in the corresponding structure
 */

void read_accel(struct s_accel *accel) {
	u8 accelX_LOW;
	u8 accelX_HIGH;
	u8 accelY_LOW;
	u8 accelY_HIGH;
	u8 accelZ_LOW;
	u8 accelZ_HIGH;
	
	MPU9150_read(ACCEL_XOUT_L, &accelX_LOW);
	MPU9150_read(ACCEL_XOUT_H, &accelX_HIGH);
	MPU9150_read(ACCEL_YOUT_L, &accelY_LOW);
	MPU9150_read(ACCEL_YOUT_H, &accelY_HIGH);
	MPU9150_read(ACCEL_ZOUT_L, &accelZ_LOW);
	MPU9150_read(ACCEL_ZOUT_H, &accelZ_HIGH);

	accel->accelX = (s16)(accelX_HIGH << 8 | accelX_LOW);
	accel->accelY = (s16)(accelY_HIGH << 8 | accelY_LOW);
	accel->accelZ = (s16)(accelZ_HIGH << 8 | accelZ_LOW);
}

/*
 * This function retrieves all the gyroscope measurement
 */
void read_gyro(void) {
	char buff[4096];
	u8 gyroX_LOW;
	u8 gyroX_HIGH;
	u8 gyroY_LOW;
	u8 gyroY_HIGH;
	u8 gyroZ_LOW;
	u8 gyroZ_HIGH;
	s16 gyroX;
	s16 gyroY;
	s16 gyroZ;

	MPU9150_read(GYRO_XOUT_L, &gyroX_LOW);
	MPU9150_read(GYRO_XOUT_H, &gyroX_HIGH);
	MPU9150_read(GYRO_YOUT_L, &gyroY_LOW);
	MPU9150_read(GYRO_YOUT_H, &gyroY_HIGH);
	MPU9150_read(GYRO_ZOUT_L, &gyroZ_LOW);
	MPU9150_read(GYRO_ZOUT_H, &gyroZ_HIGH);

	gyroX = (s16)(gyroX_HIGH << 8 | gyroX_LOW);
	gyroY = (s16)(gyroY_HIGH << 8 | gyroY_LOW);
	gyroZ = (s16)(gyroZ_HIGH << 8 | gyroZ_LOW);

	
	/*
	 *  Calibrating the Gyroscope
	 * 
	 */
	
	calibration_gyro(void);

	sprintf(buff, "%d		%d		%d\n\r", gyroX, gyroY, gyroZ);
    uart2_putstr("Gyroscope :\n\r");
	uart2_putstr(buff);
}
/* Fonction used to update any register of the MPU9150 */

void MPU9150_write(u8 register_addr, u8 value) {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(register_addr);
	i2c_master_send(value);
	I2C1CONbits.PEN = 1; // Master stop
	while (I2C1CONbits.PEN == 1)
		Nop();
}

/*
 * This is the function that measures the forces present in the system, 
 * during a no-move condition
 * We average those measures using 1024 samples
 */

void	calibration_gyro(void){
	u16 count = 0;
	struct s_gyro sample;

	while (count <= CALIBRATION_SAMPLE_NUMBER) {
		memset(&sample, 0, sizeof(struct s_gyro));
		read_gyro(&sample);
		g_xgyro += sample.gyroX;
		g_ygyro += sample.gyroY;
		g_zgyro += sample.gyroZ;
		++count;
	}

	g_xgyro /= CALIBRATION_SAMPLE_NUMBER;
	g_ygyro /= CALIBRATION_SAMPLE_NUMBER;
	g_zgyro /= CALIBRATION_SAMPLE_NUMBER;

}

void calibration(void) {
	u16 count = 0;
	struct s_accel sample;

	while (count <= CALIBRATION_SAMPLE_NUMBER) {
		memset(&sample, 0, sizeof(struct s_accel));
		read_accel(&sample);
		g_xbias += sample.accelX;
		g_ybias += sample.accelY;
		g_zbias += sample.accelZ;
		++count;
		//print_accel(sample);
	}

	g_xbias /= CALIBRATION_SAMPLE_NUMBER;
	g_ybias /= CALIBRATION_SAMPLE_NUMBER;
	g_zbias /= CALIBRATION_SAMPLE_NUMBER;

	char buff[4096];

	sprintf(buff, "%d	%d	%d\n\r", g_xbias, g_ybias, g_zbias);
	uart2_putstr(buff);
}