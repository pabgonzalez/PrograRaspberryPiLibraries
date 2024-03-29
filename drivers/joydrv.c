/*
 * Joystick Driver
 *
 * Copyright (c) 2017  Catedra Programacion 1 ITBA GEDA
 * 
 * Author(s):
 * 
 *  Nicolas Magliola
 *  Clementina Calvo
 *  Daniel Jacoby
 * 
 * 
 * Compile
 * 
 * gcc  joydrv.c      				// Stand alone (Rename mainTBJ function to main)
 * gcc -c joydrv.c    				// As object library (Rename main function to mainTBJ)
 * gcc joytb.c jotdrv.o 			// Linking
 * gcc joytb.c joydrv.o termlib.o 	// Linking (if termlib is used)
 */

 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdbool.h>
#include "termlib.h"
#include "MCP3008.h"
#include "joydrv.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define J_INV_FALSE  1			// Keep Default x and y axis Direction
#define J_INV_TRUE  -1			// Invert default x and y axis Direction

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum { JOY_NORMAL } jaxis_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint16_t convert(int fd, uint8_t channel, uint8_t mode);
static void pabort(const char* s);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
/***************Driver internal Variables*************************/

// SPI variables

//static const char *device0 = "/dev/spidev0.0"; // CS0 GPIO8
static const char *device1 = "/dev/spidev0.1"; // CS1 GPIO7
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;

// Driver Variables

static int16_t Sw,Vx,Vy;
static int8_t offset_x = 0;
static int8_t offset_y = 0;
static jcoord_t joy_coord;
static bool calib = false;
static const jaxis_t axis = JOY_NORMAL;

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/********************Driver Public Services*********************/


/* ------------------------------------------------------------------------
* jcoord_t joy_get_coord(void) 
* 
* Get Joystick Coordinates: returns a structure containing x and y angles
* 
* typedef struct {
*	int16_t x;
*	int16_t y;
*	
* } jcoord_t;
* 
* Example :
* * 
* jcoord_t joy_coord1;
* int16_t xangle,yangle;
* 
* joy_update(); // Read joystick Hardware
* joy_coord1=joy_get_coord();  
*  
* xangle=joy_coord1.x  
* yangle=joy_coord1.y
* 
* 
------------------------------------------------*/ 
jcoord_t joy_get_coord(void)
{
	return joy_coord;
}	

/* ----------------------------------------------
* jswitch_t joy_get_switch(void)
* 
* Get Joystick Switch state
* 
* Returns J_NOPRESS ,J_PRESS 
* 
* Example: 
* 
* jswitch_t joy_switch1;
*
*  		joy_update();						 // Read joystick Hardware
*		joy_switch1=joy_get_switch(); // And get switch value
* 
* 
------------------------------------------------*/  
jswitch_t joy_get_switch(void)
{
	return ((Sw>100) ? J_NOPRESS : J_PRESS);
}

/* ----------------------------------------------
   Initialize Joystick System:
   Call this function only ONCE
   at he very beginning of your Application
----------------------------------------------- */

void joy_init(void)
{ 
	calib = true;				// Auto cero 
}



/* ---------------------------------------------------------
   Update Joystick Values:
   Call this function before each measurement.
   It MUST be called before calling:
   
   - Joystick_get_switch_value()
   - joy_get_coord()
------------------------------------------------------------*/ 
int joy_update(void)
{
	int ret = 0;
	int fd;
	
	
	fd = open(device1, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

//	printf("spi mode: %d\n", mode);
//	printf("bits per word: %d\n", bits);
//	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);


	Sw = convert(fd,CH0,SINGLE);
	Vy = convert(fd,CH1,SINGLE);
	Vx = convert(fd,CH2,SINGLE);

	
		

//  Translate joystick coordinates so that default joystick position is cero 
	
	Vx = (Vx-1023/2);
	Vy = -(Vy-1023/2); 
// Retain 8 most significant bits 

	joy_coord.x = Vx >> 2;  
	joy_coord.y = Vy >> 2; 
	
	
	
// Calibrate zero position
	
	if (calib == true)  
	{
	  offset_x = joy_coord.x;
	  offset_y = joy_coord.y;
		printf(BLUE_TEXT "offset_x: %d , offfset_y: %d\n", offset_x, offset_y);
	  calib = false;
    }
    
	if( ((int)joy_coord.x - offset_x >= -128) && ((int)joy_coord.x - offset_x <= 127) )
    	joy_coord.x -= offset_x;
	if( ((int)joy_coord.y - offset_y >= -128) && ((int)joy_coord.y - offset_y <= 127) )
		joy_coord.y -= offset_y; 
	
	close(fd);
	return ret;
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/********************* Driver internal services ***********************/

static uint16_t convert(int fd,uint8_t channel,uint8_t mode)
{
	int ret;
	uint8_t tx0[] = {		
		0x01, 0x00, 0x00
	};
	uint8_t rx0[ARRAY_SIZE(tx0)] = {0, };
	struct spi_ioc_transfer tr0 = {
		.tx_buf = (unsigned long)tx0,
		.rx_buf = (unsigned long)rx0,
		.len = ARRAY_SIZE(tx0),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};


	tx0[1]= (channel | mode );

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr0);
	if (ret < 1)
		pabort("can't send spi message");

		//printf("%.2X ", rx0[1]&0x03);
		//printf("%.2X ", rx0[2]);
	    //puts("  ");
	
	return((rx0[1]&0x03)*256 + rx0[2]); 
}

static void pabort(const char* s)
{
	perror(s);
	abort();
}


