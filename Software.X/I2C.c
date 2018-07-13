#include "i2c.h"
#include "header.h"
#include "MPU9150.h"

inline void i2c_init() {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(PWR_MGMT_1);
	i2c_master_send(PWR_MGMT_ON_NO_TEMP); // Power management -> Module on, no temp sensor.
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1)
		Nop();
}

inline void i2c_read(u8 source, u8 *dest) {
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
 * them in the corresponding sturcture
 */

inline void i2c_loop(struct s_data *data) {
	i2c_read(ACCEL_XOUT_L, &data->accelX_LOW);
	i2c_read(ACCEL_XOUT_H, &data->accelX_HIGH);
	i2c_read(ACCEL_YOUT_L, &data->accelY_LOW);
	i2c_read(ACCEL_YOUT_H, &data->accelY_HIGH);
	i2c_read(ACCEL_ZOUT_L, &data->accelZ_LOW);
	i2c_read(ACCEL_ZOUT_H, &data->accelZ_HIGH);
}