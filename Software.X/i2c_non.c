/**1. Start Condition
- 1 BRG (Baud Rate Generator) time after falling edge of SDA
2. Repeated Start Sequence
- 1 BRG time after falling edge of SDA
3. Stop Condition
- 1 BRG time after the rising edge of SDA
4. Data transfer byte received
- 8th falling edge of SCL
(After receiving eight bits of data from slave)
5. During SEND ACK sequence
- 9th falling edge of SCL
(After sending ACK or NACK to slave)
6. Data transfer byte transmitted
- 9th falling edge of SCL
(Regardless of receiving ACK from slave)
7. During a slave-detected Stop
- When slave sets P bit
**/
 void __ISR (_I2C_1_VECTOR, IPL7SRS) master_interrupt(void){
	IFS0bits.I2C1MIF = 0;
	switch (g_i2c_state) {
	case 0:
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
    case 1:
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
	i2c_master_send(source);
    case 2:
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
	I2C1CONbits.RSEN = 1;
    case 3:
	i2c_master_send(ADDR_READ_MODE(SLAVE_ADDR));
    case 4:
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
	I2C1CONbits.RCEN = 1;
    case 5:
	i2c_master_receive(dest);
	i2c_master_answer(NACK);
    case 6:
	I2C1CONbits.PEN = 1;
	case 7:
	g_i2c_state = 0;
    }
    ++g_i2c_state;
}

void set_interrupt_right_click() {
	INTCONbits.INT3EP = g_edge_int3;

	IPC6bits.I2C1IP = 7;
	IPC6bits.I2C1IS = 3;
	IFS0bits.I2C1MIF = 0;
	IEC0bits.I2C1MIE = 1;
	I2C1CONbits.ON = 1;
}
