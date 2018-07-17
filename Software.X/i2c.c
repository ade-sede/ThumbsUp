#include "i2c.h"
#include "header.h"
#include "MPU9150.h"

void i2c_read(u8 source, u8 *dest) {
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
void i2c_encode(struct s_data *data) {
	data->accelX = (s16)(data->accelX_HIGH << 8 | data->accelX_LOW);
	data->accelY = (s16)(data->accelY_HIGH << 8 | data->accelY_LOW);
	data->accelZ = (s16)(data->accelZ_HIGH << 8 | data->accelZ_LOW);
}

void i2c_read_accel(struct s_data *data) {
	i2c_read(ACCEL_XOUT_L, &data->accelX_LOW);
	i2c_read(ACCEL_XOUT_H, &data->accelX_HIGH);
	i2c_read(ACCEL_YOUT_L, &data->accelY_LOW);
	i2c_read(ACCEL_YOUT_H, &data->accelY_HIGH);
	i2c_read(ACCEL_ZOUT_L, &data->accelZ_LOW);
	i2c_read(ACCEL_ZOUT_H, &data->accelZ_HIGH);
}

void i2c_read_fifo(struct s_fifo *fifo) {
	i2c_read(FIFO_COUNTH, &fifo->fifo_count_h);
	i2c_read(FIFO_COUNTL, &fifo->fifo_count_l);
	i2c_read(FIFO_R_W, &fifo->fifo_rw);
}

void i2c_map(struct s_data *data, void (*f)(u8 *elem)){
	f(&data->accelX);
	f(&data->accelY);
	f(&data->accelZ);
}

void i2c_process_data(struct s_data *buff, struct s_data *data) {
	i2c_read_accel(data);
	i2c_process_data(buff, data);
	i2c_read_accel(buff);
}

void config_MPU9150_register(u8 register_name, u8 bit_config) {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(register_name);
	i2c_master_send(bit_config);
	I2C1CONbits.PEN = 1; // Master stop
	while (I2C1CONbits.PEN == 1)
		Nop();
}
