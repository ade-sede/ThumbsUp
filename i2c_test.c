#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>
#include "header.h"

#define RW_READ 1
#define RW_WRITE 0
#define ADDR_READ_MODE(addr) ((addr << 1) | RW_READ)
#define ADDR_WRITE_MODE(addr) (addr << 1)
#define NACK 1
#define ACK 0
#define ACCEL_XOUT_L 0x3C
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SLAVE_ADDR 0x68
#define LED_OFF LATFbits.LATF1 = 0
#define LED_ON LATFbits.LATF1 = 1
#define LED_TOGGLE LATFbits.LATF1 ^= 1

u8 volatile g_data_buffer[2];
u8 volatile g_target;
u8 volatile g_i2c_state = 0;
u8 volatile *g_data;
u8 volatile g_err = 0;

inline void master_send(u8 data) {
        I2C1TRN = data;
        while (I2C1STATbits.TRSTAT == 1)
            Nop();
//        Nop();
         if (I2C1STATbits.ACKSTAT == NACK)
            g_err = 1;
}

inline void master_receive() {
    I2C1CONbits.RCEN = 1;
    while (I2C1STATbits.RBF == 0) Nop();
    *g_data = I2C1RCV;
}

inline void master_answer(u8 value) {
    I2C1CONbits.ACKDT = value;
    I2C1CONbits.ACKEN = 1;
}

int main(void) {
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 0;

        g_data = &g_data_buffer[0];
        OSCCONbits.PBDIV = 0b011; // PBCLK = SYSCLK / 8 soit 10Mhz

//        I2C1BRG = 0x0F8;
        I2C1BRG = 0x030;
	I2C1CONbits.ON = 1;

//        while (I2C1STATbits.P == 0) Nop();
        I2C1CONbits.SEN = 1; // Master start
            while (I2C1CONbits.SEN == 1)
                Nop();

            master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
            master_send(PWR_MGMT_1);
            master_send(0b00001000); // Power management -> Module on, no temp sensor.
            I2C1CONbits.PEN = 1;
            Nop();
         //   while (I2C1STATbits.P == 0) Nop();
            I2C1CONbits.SEN = 1;
            master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
            master_send(WHO_AM_I);
            I2C1CONbits.RSEN = 1;
            master_send(ADDR_READ_MODE(SLAVE_ADDR));
            master_receive();
            master_answer(NACK);
            I2C1CONbits.PEN = 1;
            Nop();
        while (1) Nop();
}
