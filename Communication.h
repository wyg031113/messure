/***************************************************************************//**
 *   @file   Communication.h
 *   @brief  Header file of Communication Driver.
 *   @author wyg
*******************************************************************************/
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include <unistd.h>
#include <stdio.h>
/******************************************************************************/
/* Functions Prototypes                                                       */
/******************************************************************************/

/*Initializes the SPI communication peripheral. */
int SPI_Init(unsigned char lsbFirst,unsigned long clockFreq, 
		unsigned char clockPol, unsigned char clockEdg);

/*Reads data from SPI. */
int SPI_Read(int slaveDeviceId, unsigned char* data, int bytesNumber);

/*Writes data to SPI. */
int SPI_Write(int slaveDeviceId, unsigned char* data, int bytesNumber);

/*close spi fd*/
void SPI_Close(int fd);
#endif /* _COMMUNICATION_H_ */
