/*
 * i2c.c
 *
 *  Created on: Jan 13, 2022
 *      Author: EmreDemir
 */
#include "i2c.h"


void Pinmux_Config(i2cPort_t port)
{
	if(port == I2C0)
	{
		*SYSMGR_I2C0USEFPGA = 0;
		*SYSMGR_GENERALIO7 = 1;
		*SYSMGR_GENERALIO8 = 1;
	}
	else if(port ==I2C1)
	{
		*SYSMGR_I2C1USEFPGA = 0;
		*SYSMGR_GENERALIO9 = 2;
		*SYSMGR_GENERALIO10 = 2;
	}
}


void I2C_Init(i2c_t *i2c, uint8_t slaveAddress)
{
	// Abort any ongoing transmits and disable I2C0.
	i2c->ic_enable = 2;

	// Wait until I2C0 is disabled
	while(((i2c->ic_enable_status)&0x1) == 1);

	// Configure the config reg with the desired setting (act as
 	// a master, use 7bit addressing, fast mode (400kb/s)).
	i2c->ic_con = 0x65;

	// Set target address (disable special commands, use 7bit addressing)
	i2c->ic_tar = slaveAddress;

	// Set SCL high/low counts (Assuming default 100MHZ clock input to I2C0 Controller).
	// The minimum SCL high period is 0.6us, and the minimum SCL low period is 1.3us,
	// However, the combined period must be 2.5us or greater, so add 0.3us to each.
	i2c->ic_fs_scl_hcnt = 60 + 30; // 0.6us + 0.3us
	i2c->ic_fs_scl_lcnt = 130 + 30; // 1.3us + 0.3us

	// Enable the controller
	i2c->ic_enable = 1;
	// Wait until controller is powered on
	while(((i2c->ic_enable_status)&0x1) == 0);
}


void I2C_readReg(i2c_t *i2c, uint8_t address, uint8_t *value)
{
	// Send reg address (+0x400 to send START signal)
	i2c->ic_data_cmd = address + 0x400;

	// Send read signal
	i2c->ic_data_cmd = 0x100;

	// Read the response (first wait until RX buffer contains data)
	while (i2c->ic_rxflr == 0);

	*value = i2c->ic_data_cmd;
}


void I2C_writeReg(i2c_t *i2c, uint8_t address, uint8_t value)
{
	// Send reg address (+0x400 to send START signal)
	i2c->ic_data_cmd = address + 0x400;

	// Send value
	i2c->ic_data_cmd = value;
}


void I2C_multiReadReg(i2c_t *i2c,uint8_t address, uint8_t values[], uint8_t len)
{
	// Send reg address (+0x400 to send START signal)
	i2c->ic_data_cmd = address + 0x400;

	// Send read signal len times
	int i;
	for (i=0;i<len;i++)
		i2c->ic_data_cmd = 0x100;

	// Read the bytes
	int nth_byte=0;
	while (len){
		if ((i2c->ic_rxflr) > 0)
		{
			values[nth_byte] = i2c->ic_data_cmd;
			nth_byte++;
			len--;
		}
	}
}



