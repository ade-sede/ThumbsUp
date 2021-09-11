 #include "i2c.h"
#include "header.h"
#include "MPU9150.h"
#include "uart.h"
#include "debug.h"

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
        /// if NACK is received you should trigger the interrupt ///
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

        /* Reading the raw data of the Accelerometer from the MPU */

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