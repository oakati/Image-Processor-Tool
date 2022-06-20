/*
 * i2c.h
 *
 *  Created on: Jan 13, 2022
 *      Author: EmreDemir
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

/********************************
 **********I2C PinMux************
 ********************************/
#define SYSMGR_GENERALIO7  ((volatile unsigned int *) 0xFFD0849C)//1 for i2c
#define SYSMGR_GENERALIO8  ((volatile unsigned int *) 0xFFD084A0)//1 for i2c
#define SYSMGR_GENERALIO9  ((volatile unsigned int *) 0xFFD084A4)//2 for i2c
#define SYSMGR_GENERALIO10 ((volatile unsigned int *) 0xFFD084A8)//2 for i2c

#define SYSMGR_I2C0USEFPGA ((volatile unsigned int *) 0xFFD08704)
#define SYSMGR_I2C1USEFPGA ((volatile unsigned int *) 0xFFD0872C)



#define I2C0_BASE 0xFFC04000
#define I2C1_BASE 0xFFC05000


typedef enum
{
	I2C0= 0,
	I2C1

}i2cPort_t;


typedef struct
{
	volatile uint32_t ic_con;
	volatile uint32_t ic_tar;
	volatile uint32_t reserved[2];
	volatile uint32_t ic_data_cmd;
	volatile uint32_t reserved_2[2];
	volatile uint32_t ic_fs_scl_hcnt;
	volatile uint32_t ic_fs_scl_lcnt;
	volatile uint32_t reserved_3[7];
	volatile uint32_t ic_clr_intr;
	volatile uint32_t reserved_4[10];
	volatile uint32_t ic_enable;
	volatile uint32_t reserved_5;
	volatile uint32_t ic_txflr;
	volatile uint32_t ic_rxflr;
	volatile uint32_t reserved_6[8];
	volatile uint32_t ic_enable_status;
}i2c_t;


void Pinmux_Config(i2cPort_t port);
void I2C_Init(i2c_t *i2c, uint8_t slaveAddress);
void I2C_readReg(i2c_t *i2c, uint8_t address, uint8_t *value);
void I2C_writeReg(i2c_t *i2c, uint8_t address, uint8_t value);
void I2C_multiReadReg(i2c_t *i2c,uint8_t address, uint8_t values[], uint8_t len);

#endif /* I2C_H_ */
