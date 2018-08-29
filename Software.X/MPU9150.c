#include "i2c.h"
#include "header.h"
#include "MPU9150.h"
#include "uart.h"
#include "movement.h"
#include "debug.h"

extern struct s_accel g_accel_bias;
extern struct s_gyro g_gyro_bias;

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
void read_gyro(struct s_gyro *gyro) {
	u8 gyroX_LOW;
	u8 gyroX_HIGH;
	u8 gyroY_LOW;
	u8 gyroY_HIGH;
	u8 gyroZ_LOW;
	u8 gyroZ_HIGH;

	MPU9150_read(GYRO_XOUT_L, &gyroX_LOW);
	MPU9150_read(GYRO_XOUT_H, &gyroX_HIGH);
	MPU9150_read(GYRO_YOUT_L, &gyroY_LOW);
	MPU9150_read(GYRO_YOUT_H, &gyroY_HIGH);
	MPU9150_read(GYRO_ZOUT_L, &gyroZ_LOW);
	MPU9150_read(GYRO_ZOUT_H, &gyroZ_HIGH);

	gyro->gyroX = (s16)(gyroX_HIGH << 8 | gyroX_LOW);
	gyro->gyroY = (s16)(gyroY_HIGH << 8 | gyroY_LOW);
	gyro->gyroZ = (s16)(gyroZ_HIGH << 8 | gyroZ_LOW);
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
 * during a no-move condition at the beginnig
 * and shorter during execution
 * We average those measures using CALIBRATION_SAMPLE_NUMBER samples
 */

void calibration() {
	u16 count = 0;
	struct s_accel sample_accel;
	struct s_gyro sample_gyro;

	while (count <= CALIBRATION_SAMPLE_NUMBER) {
		memset(&sample_accel, 0, sizeof(struct s_accel));
		memset(&sample_gyro, 0, sizeof(struct s_gyro));
		read_accel(&sample_accel);
		read_gyro(&sample_gyro);
		g_accel_bias.accelX += sample_accel.accelX;
		g_accel_bias.accelY += sample_accel.accelY;
		g_accel_bias.accelZ += sample_accel.accelZ;
		g_gyro_bias.gyroX += sample_gyro.gyroX;
		g_gyro_bias.gyroY += sample_gyro.gyroY;
		g_gyro_bias.gyroZ += sample_gyro.gyroZ;
		++count;
	}

	g_accel_bias.accelX /= CALIBRATION_SAMPLE_NUMBER;
	g_accel_bias.accelY /= CALIBRATION_SAMPLE_NUMBER;
	g_accel_bias.accelZ /= CALIBRATION_SAMPLE_NUMBER;
	g_gyro_bias.gyroX /= CALIBRATION_SAMPLE_NUMBER;
	g_gyro_bias.gyroY /= CALIBRATION_SAMPLE_NUMBER;
	g_gyro_bias.gyroZ /= CALIBRATION_SAMPLE_NUMBER;

	uart2_putstr("Calibration acceleration : \n\r");
	print_accel(g_accel_bias);

	uart2_putstr("Calibration gyroscope : \n\r");
	print_gyro(g_gyro_bias);
}
