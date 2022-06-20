/*
 * OV7670.h
 *
 *  Created on: Jan 13, 2022
 *      Author: EmreDemir
 *
 */

#ifndef OV7670_H_
#define OV7670_H_

#include <stdint.h>
#include "i2c.h"
#include "gpio.h"


/* Registers */
#define REG_GAIN    	0x00  /* Gain lower 8 bits (rest in vref) */
#define REG_BLUE    	0x01  /* blue gain */
#define REG_RED       	0x02  /* red gain */
#define REG_VREF   	 	0x03  /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1    	0x04  /* Control 1 */
#define COM1_CCIR656  	0x40    /* CCIR656 enable */

#define REG_BAVE    	0x05  /* U/B Average level */
#define REG_GbAVE   	0x06  /* Y/Gb Average level */
#define REG_AECHH   	0x07  /* AEC MS 5 bits */
#define REG_RAVE    	0x08  /* V/R Average level */
#define REG_COM2    	0x09  /* Control 2 */
#define COM2_SSLEEP     0x10  /* Soft sleep mode */
#define REG_PID         0x0A  /* Product ID MSB */
#define REG_VER         0x0B  /* Product ID LSB */
#define REG_COM3    	0x0C  /* Control 3 */
#define COM3_SWAP       0x40  /* Byte swap */
#define COM3_SCALEEN    0x08  /* Enable scaling */
#define COM3_DCWEN     	0x04  /* Enable downsamp/crop/window */
#define REG_COM4    	0x0D  /* Control 4 */
#define REG_COM5    	0x0E  /* All "reserved" */
#define REG_COM6    	0x0F  /* Control 6 */
#define REG_AECH    	0x10  /* More bits of AEC value */
#define REG_CLKRC   	0x11  /* Clocl control */
#define CLK_EXT         0x40  /* Use external clock directly */
#define CLK_SCALE   	0x3F  /* Mask for internal clock scale */
#define REG_COM7    	0x12  /* Control 7 */ //REG mean address.
#define COM7_RESET      0x80  /* Register reset */
#define COM7_FMT_MASK   0x38
#define COM7_FMT_VGA    0x00
#define COM7_FMT_CIF    0x20  /* CIF format */
#define COM7_FMT_QVGA   0x10  /* QVGA format */
#define COM7_FMT_QCIF   0x08  /* QCIF format */
#define COM7_RGB        0x04  /* bits 0 and 2 - RGB format */
#define COM7_YUV        0x00  /* YUV */
#define COM7_BAYER      0x01  /* Bayer format */
#define COM7_PBAYER     0x05  /* "Processed bayer" */
#define REG_COM8    	0x13  /* Control 8 */
#define COM8_FASTAEC    0x80  /* Enable fast AGC/AEC */
#define COM8_AECSTEP    0x40  /* Unlimited AEC step size */
#define COM8_BFILT    	0x20  /* Band filter enable */
#define COM8_AGC    	0x04  /* Auto gain enable */
#define COM8_AWB    	0x02  /* White balance enable */
#define COM8_AEC    	0x01  /* Auto exposure enable */
#define REG_COM9    	0x14  /* Control 9- gain ceiling */
#define REG_COM10   	0x15  /* Control 10 */
#define COM10_HSYNC     0x40  /* HSYNC instead of HREF */
#define COM10_PCLK_HB   0x20  /* Suppress PCLK on horiz blank */
#define COM10_HREF_REV  0x08  /* Reverse HREF */
#define COM10_VS_LEAD   0x04  /* VSYNC on clock leading edge */
#define COM10_VS_NEG    0x02  /* VSYNC negative */
#define COM10_HS_NEG    0x01  /* HSYNC negative */
#define REG_HSTART   	0x17  /* Horiz start high bits */
#define REG_HSTOP   	0x18  /* Horiz stop high bits */
#define REG_VSTART    	0x19  /* Vert start high bits */
#define REG_VSTOP   	0x1A  /* Vert stop high bits */
#define REG_PSHFT   	0x1B  /* Pixel delay after HREF */
#define REG_MIDH    	0x1C  /* Manuf. ID high */
#define REG_MIDL    	0x1D  /* Manuf. ID low */
#define REG_MVFP    	0x1E  /* Mirror / vflip */
#define MVFP_MIRROR     0x20  /* Mirror image */
#define MVFP_FLIP   	0x10  /* Vertical flip */

#define REG_AEW         0x24  /* AGC upper limit */
#define REG_AEB         0x25    /* AGC lower limit */
#define REG_VPT         0x26  /* AGC/AEC fast mode op region */
#define REG_HSYST   	0x30  /* HSYNC rising edge delay */
#define REG_HSYEN   	0x31  /* HSYNC falling edge delay */
#define REG_HREF    	0x32  /* HREF pieces */
#define REG_TSLB    	0x3A  /* lots of stuff */
#define TSLB_YLAST    	0x04  /* UYVY or VYUY - see com13 */
#define REG_COM11   	0x3B  /* Control 11 */
#define COM11_NIGHT     0x80  /* NIght mode enable */
#define COM11_NMFR      0x60  /* Two bit NM frame rate */
#define COM11_HZAUTO    0x10  /* Auto detect 50/60 Hz */
#define COM11_50HZ      0x08  /* Manual 50Hz select */
#define COM11_EXP   	0x02
#define REG_COM12  	 	0x3C  /* Control 12 */
#define COM12_HREF  	0x80  /* HREF always */
#define REG_COM13   	0x3D  /* Control 13 */
#define COM13_GAMMA     0x80  /* Gamma enable */
#define COM13_UVSAT     0x40  /* UV saturation auto adjustment */
#define COM13_UVSWAP  	0x01  /* V before U - w/TSLB */
#define REG_COM14   	0x3E  /* Control 14 */
#define COM14_DCWEN		0x10  /* DCW/PCLK-scale enable */
#define REG_EDGE    	0x3F  /* Edge enhancement factor */
#define REG_COM15   	0x40  /* Control 15 */
#define COM15_R10F0     0x00  /* Data range 10 to F0 */
#define COM15_R01FE     0x80  /* 01 to FE */
#define COM15_R00FF     0xC0  /* 00 to FF */
#define COM15_RGB565    0x10  /* RGB565 output */
#define COM15_RGB555    0x30  /* RGB555 output */
#define REG_COM16   	0x41  /* Control 16 */
#define COM16_AWBGAIN   0x08  /* AWB gain enable */
#define REG_COM17   	0x42  /* Control 17 */
#define COM17_AECWIN    0xC0  /* AEC window - must match COM4 */
#define COM17_CBAR      0x08  /* DSP Color bar */
/*
* This matrix defines how the colors are generated, must be
* tweaked to adjust hue and saturation.
*
* Order: v-red, v-green, v-blue, u-red, u-green, u-blue
* They are nine-bit signed quantities, with the sign bit
* stored in0x58.Sign for v-red is bit 0, and up from there.
*/
#define REG_CMATRIX_BASE  	0x4F
#define CMATRIX_LEN			6
#define REG_CMATRIX_SIGN  	0x58
#define REG_BRIGHT    		0x55  /* Brightness */
#define REG_CONTRAS         0x56  /* Contrast control */
#define REG_GFIX    		0x69  /* Fix gain control */
#define REG_REG76   		0x76  /* OV's name */
#define R76_BLKPCOR         0x80  /* Black pixel correction enable */
#define R76_WHTPCOR         0x40  /* White pixel correction enable */
#define REG_RGB444          0x8C  /* RGB 444 control */
#define R444_ENABLE         0x02  /* Turn on RGB444, overrides 5x5 */
#define R444_RGBX   		0x01  /* Empty nibble at end */
#define REG_HAECC1    		0x9F  /* Hist AEC/AGC control 1 */
#define REG_HAECC2    		0xA0  /* Hist AEC/AGC control 2 */
#define REG_BD50MAX         0xA5  /* 50hz banding step limit */
#define REG_HAECC3    		0xA6  /* Hist AEC/AGC control 3 */
#define REG_HAECC4    		0xA7  /* Hist AEC/AGC control 4 */
#define REG_HAECC5    		0xA8  /* Hist AEC/AGC control 5 */
#define REG_HAECC6    		0xA9  /* Hist AEC/AGC control 6 */
#define REG_HAECC7    		0xAA  /* Hist AEC/AGC control 7 */
#define REG_BD60MAX         0xAB  /* 60hz banding step limit */
#define REG_GAIN    		0x00  /* Gain lower 8 bits (rest in vref) */
#define REG_BLUE    		0x01  /* blue gain */
#define REG_RED           	0x02  /* red gain */
#define REG_VREF    		0x03  /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1    		0x04  /* Control 1 */
#define COM1_CCIR656    	0x40  /* CCIR656 enable */
#define REG_BAVE    		0x05  /* U/B Average level */
#define REG_GbAVE   		0x06  /* Y/Gb Average level */
#define REG_AECHH   		0x07  /* AEC MS 5 bits */
#define REG_RAVE    		0x08  /* V/R Average level */
#define REG_COM2    		0x09  /* Control 2 */
#define COM2_SSLEEP         0x10  /* Soft sleep mode */
#define REG_PID           	0x0A  /* Product ID MSB */
#define REG_VER           	0x0B  /* Product ID LSB */
#define REG_COM3    		0x0C  /* Control 3 */
#define COM3_SWAP         	0x40  /* Byte swap */
#define COM3_SCALEEN   	    0x08  /* Enable scaling */
#define COM3_DCWEN          0x04  /* Enable downsamp/crop/window */
#define REG_COM4    		0x0D  /* Control 4 */
#define REG_COM5    		0x0E  /* All "reserved" */
#define REG_COM6    		0x0F  /* Control 6 */
#define REG_AECH    		0x10  /* More bits of AEC value */
#define REG_CLKRC   		0x11  /* Clocl control */
#define CLK_EXT           	0x40  /* Use external clock directly */
#define CLK_SCALE   		0x3F  /* Mask for internal clock scale */
#define REG_COM7    		0x12  /* Control 7 */
#define COM7_RESET          0x80  /* Register reset */
#define COM7_FMT_MASK   	0x38
#define COM7_FMT_VGA     	0x00
#define COM7_FMT_CIF   		0x20  /* CIF format */
#define COM7_FMT_QVGA     	0x10  /* QVGA format */
#define COM7_FMT_QCIF     	0x08  /* QCIF format */
#define COM7_RGB    		0x04  /* bits 0 and 2 - RGB format */
#define COM7_YUV    		0x00  /* YUV */
#define COM7_BAYER          0x01  /* Bayer format */
#define COM7_PBAYER         0x05  /* "Processed bayer" */
#define REG_COM8    		0x13  /* Control 8 */
#define COM8_FASTAEC      	0x80  /* Enable fast AGC/AEC */
#define COM8_AECSTEP     	0x40  /* Unlimited AEC step size */
#define COM8_BFILT    		0x20  /* Band filter enable */
#define COM8_AGC    		0x04  /* Auto gain enable */
#define COM8_AWB    		0x02  /* White balance enable */
#define COM8_AEC    		0x01  /* Auto exposure enable */
#define REG_COM9    		0x14  /* Control 9- gain ceiling */
#define REG_COM10   		0x15  /* Control 10 */
#define COM10_HSYNC         0x40  /* HSYNC instead of HREF */
#define COM10_PCLK_HB     	0x20  /* Suppress PCLK on horiz blank */
#define COM10_HREF_REV     	0x08  /* Reverse HREF */
#define COM10_VS_LEAD      	0x04  /* VSYNC on clock leading edge */
#define COM10_VS_NEG      	0x02  /* VSYNC negative */
#define COM10_HS_NEG      	0x01  /* HSYNC negative */
#define REG_HSTART    		0x17  /* Horiz start high bits */
#define REG_HSTOP   		0x18  /* Horiz stop high bits */
#define REG_VSTART    		0x19  /* Vert start high bits */
#define REG_VSTOP   		0x1A  /* Vert stop high bits */
#define REG_PSHFT   		0x1B  /* Pixel delay after HREF */
#define REG_MIDH    		0x1C  /* Manuf. ID high */
#define REG_MIDL    		0x1D  /* Manuf. ID low */
#define REG_MVFP    		0x1E  /* Mirror / vflip */
#define MVFP_MIRROR         0x20  /* Mirror image */
#define MVFP_FLIP   		0x10  /* Vertical flip */
#define REG_AEW           	0x24  /* AGC upper limit */
#define REG_AEB           	0x25  /* AGC lower limit */
#define REG_VPT           	0x26  /* AGC/AEC fast mode op region */
#define REG_HSYST   		0x30  /* HSYNC rising edge delay */
#define REG_HSYEN   		0x31  /* HSYNC falling edge delay */
#define REG_HREF    		0x32  /* HREF pieces */
#define REG_TSLB    		0x3A  /* lots of stuff */
#define TSLB_YLAST    		0x04  /* UYVY or VYUY - see com13 */
#define REG_COM11   		0x3B  /* Control 11 */
#define COM11_NIGHT         0x80  /* NIght mode enable */
#define COM11_NMFR          0x60  /* Two bit NM frame rate */
#define COM11_HZAUTO      	0x10  /* Auto detect 50/60 Hz */
#define COM11_50HZ          0x08  /* Manual 50Hz select */
#define COM11_EXP   		0x02
#define REG_COM12   		0x3C  /* Control 12 */
#define COM12_HREF          0x80  /* HREF always */
#define REG_COM13   		0x3D  /* Control 13 */
#define COM13_GAMMA         0x80  /* Gamma enable */
#define COM13_UVSAT         0x40  /* UV saturation auto adjustment */
#define COM13_UVSWAP       	0x01  /* V before U - w/TSLB */
#define REG_COM14   		0x3E  /* Control 14 */
#define COM14_DCWEN         0x10  /* DCW/PCLK-scale enable */
#define REG_EDGE    		0x3F  /* Edge enhancement factor */
#define REG_COM15   		0x40  /* Control 15 */
#define COM15_R10F0         0x00  /* Data range 10 to F0 */
#define COM15_R01FE         0x80  /*      01 to FE */
#define COM15_R00FF         0xC0  /*      00 to FF */
#define COM15_RGB565       	0x10  /* RGB565 output */
#define COM15_RGB555       	0x30  /* RGB555 output */
#define REG_COM16   		0x41  /* Control 16 */
#define COM16_AWBGAIN     	0x08  /* AWB gain enable */
#define REG_COM17   		0x42  /* Control 17 */
#define COM17_AECWIN      	0xC0  /* AEC window - must match COM4 */
#define COM17_CBAR          0x08  /* DSP Color bar */

#define CMATRIX_LEN     	6
#define REG_BRIGHT    		0x55  /* Brightness */
#define REG_REG76   		0x76  /* OV's name */
#define R76_BLKPCOR         0x80  /* Black pixel correction enable */
#define R76_WHTPCOR         0x40  /* White pixel correction enable */
#define REG_RGB444          0x8C  /* RGB 444 control */
#define R444_ENABLE         0x02  /* Turn on RGB444, overrides 5x5 */
#define R444_RGBX   		0x01  /* Empty nibble at end */
#define REG_HAECC1    		0x9F  /* Hist AEC/AGC control 1 */
#define REG_HAECC2    		0xA0  /* Hist AEC/AGC control 2 */
#define REG_BD50MAX         0xA5  /* 50hz banding step limit */
#define REG_HAECC3    		0xA6  /* Hist AEC/AGC control 3 */
#define REG_HAECC4   		0xA7  /* Hist AEC/AGC control 4 */
#define REG_HAECC5    		0xA8  /* Hist AEC/AGC control 5 */
#define REG_HAECC6    		0xA9  /* Hist AEC/AGC control 6 */
#define REG_HAECC7    		0xAA  /* Hist AEC/AGC control 7 */
#define REG_BD60MAX         0xAB  /* 60hz banding step limit */
#define MTX1            	0x4F  /* Matrix Coefficient 1 */
#define MTX2            	0x50  /* Matrix Coefficient 2 */
#define MTX3            	0x51  /* Matrix Coefficient 3 */
#define MTX4            	0x52  /* Matrix Coefficient 4 */
#define MTX5            	0x53  /* Matrix Coefficient 5 */
#define MTX6            	0x54  /* Matrix Coefficient 6 */
#define REG_CONTRAS         0x56  /* Contrast control */
#define MTXS            	0x58  /* Matrix Coefficient Sign */
#define AWBC7           	0x59  /* AWB Control 7 */
#define AWBC8           	0x5A  /* AWB Control 8 */
#define AWBC9           	0x5B  /* AWB Control 9 */
#define AWBC10           	0x5C  /* AWB Control 10 */
#define AWBC11            	0x5D  /* AWB Control 11 */
#define AWBC12            	0x5E  /* AWB Control 12 */
#define REG_GFI           	0x69  /* Fix gain control */
#define GGAIN           	0x6A  /* G Channel AWB Gain */
#define DBLV            	0x6B
#define AWBCTR3           	0x6C  /* AWB Control 3 */
#define AWBCTR2           	0x6D  /* AWB Control 2 */
#define AWBCTR1           	0x6E  /* AWB Control 1 */
#define AWBCTR0           	0x6F  /* AWB Control 0 */

struct regval_list{
  uint8_t reg_num;
  uint8_t value;
};

#define CAMERA_SET_INPUT_MASK	0x000007FF
#define CAMERA_DATA_PINS_MASK 	0x000000FF //D0-D7 ==> JP1, D0-7
#define CAMERA_VSYNC_PIN_MASK	0x00000100 // JP1, D8
#define CAMERA_HREF_PIN_MASK	0x00000200 // JP1, D9
#define CAMERA_PCLK_PIN_MASK	0x00000400 // JP1, D10


// For RGB565, QVGA
#define CAMERA_WIDTH	320
#define CAMERA_HEIGHT	240
#define BYTES_PER_PIXEL	2

void CamInit();
void writeRegList(i2c_t *i2c, const struct regval_list reglist[] );
void captureImg(void* buffer);
void delay_ms(uint32_t milliseconds);



#endif /* OV7670_H_ */
