/* 
 * File:   i2c.h
 * Author: bocal
 *
 * Created on July 13, 2018, 4:17 PM
 */

#ifndef I2C_H
# define I2C_H
# include "MPU9150.h"

#ifndef PBCLK
# include "clock.h"
#endif
#define FSCK 4000 // 400kHz
#define I2CBRG (PBCLK / (2 * FSCK)) - 2

#define RW_READ 1
#define RW_WRITE 0
#define ADDR_READ_MODE(addr) ((addr << 1) | RW_READ)
#define ADDR_WRITE_MODE(addr) (addr << 1)
#define NACK 1
#define ACK 0

/* Prototypes */
void i2c_read(u8 source, u8 *dest);
void i2c_read_accel(struct s_data *);
void i2c_read_fifo(struct s_fifo *);
void i2c_config_and_start(u8 brg);
void i2c_master_send(u8 data);
void i2c_master_receive(u8 *dest);
void i2c_master_answer(u8 value);
void i2c_process_data(struct s_data *buff, struct s_data *data);


#endif	/* I2C_H */

