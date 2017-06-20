/***************************************************************************//**
 *   @file   Communication.c
 *   @brief  Implementation of Communication Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2012-2015(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*******************************************************************************/

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "Communication.h"
#include "debug.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <memory.h>
#include <linux/spi/spidev.h>
static const char *device = "/dev/spidev1.0";
static uint8_t mode = SPI_MODE_3;
static uint8_t bits = 32;
static uint32_t speed = 5000;
static uint16_t delay;

/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 * @param lsbFirst - Transfer format (0 or 1).
 *                   Example: 0x0 - MSB first.
 *                            0x1 - LSB first.
 * @param clockFreq - SPI clock frequency (Hz).
 *                    Example: 1000 - SPI clock frequency is 1 kHz.
 * @param clockPol - SPI clock polarity (0 or 1).
 *                   Example: 0x0 - Idle state for clock is a low level; active
 *                                  state is a high level;
 *	                      0x1 - Idle state for clock is a high level; active
 *                                  state is a low level.
 * @param clockEdg - SPI clock edge (0 or 1).
 *                   Example: 0x0 - Serial output data changes on transition
 *                                  from idle clock state to active clock state;
 *                            0x1 - Serial output data changes on transition
 *                                  from active clock state to idle clock state.
 * @return status - Result of the initialization procedure.
 *                  Example: 0 - if initialization was successful;
 *                           negtive - if initialization was unsuccessful.
*******************************************************************************/
int SPI_Init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockEdg)
{
    /* Add your code here. */
	int fd = open(device, O_RDWR);
	if (fd < 0){
        INFO("can't open device");
		return fd;
	}
	mode = (clockPol << 1) | clockEdg;
	speed = clockFreq;

	// set spi mode
    int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1){
        INFO("can't set spi mode");
		goto failed2;
	}
	//get spi mode to check if set success
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1){
        INFO("can't get spi mode");
		goto failed2;
	}

	//set spi bits per word
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);  
    if (ret == -1){
        INFO("can't set bits per word");  
		goto failed2;
	}

    //set spi  speed hz 
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1){
        INFO("can't set max speed hz");
		goto failed2;
	}

	//read spi speed to confirm set success.
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1){
        INFO("can't get max speed hz");
		goto failed2;
	}

    INFO("spi mode: %d\n", mode);
    INFO("bits per word: %d\n", bits);
    INFO("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
    return fd;
failed2:
	close(fd);
	return ret;
}

/***************************************************************************//**
 * @brief Reads data from SPI.
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer as an input parameter and the
 *               read buffer as an output parameter.
 * @param bytesNumber - Number of bytes to read.
 * @return Number of read bytes.
*******************************************************************************/
int SPI_Read(int slaveDeviceId,
                       unsigned char* data,
                       int bytesNumber)
{
    /* Add your code here. */
    int ret;
    uint8_t rx[8] = {0};
	if(bytesNumber > 4){
		printf("error, bytes Number > 8!\n");
		return 0;
	}
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)data,
        .rx_buf = (unsigned long)rx,
        .len = bytesNumber,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };
    ret = ioctl(slaveDeviceId, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1){
		printf("error, spi write failed\n");
		return 0;
	}
	memcpy(data, rx, bytesNumber);
    return bytesNumber;	
}

/***************************************************************************//**
 * @brief Writes data to SPI.
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer.
 * @param bytesNumber - Number of bytes to write.
 * @return Number of written bytes.
*******************************************************************************/
int SPI_Write(int slaveDeviceId,
                        unsigned char* data,
                        int bytesNumber)
{
    /* Add your code here. */
    int ret;
    uint8_t rx[8] = {0};
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)data,
        .rx_buf = (unsigned long)rx,
        .len = bytesNumber,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };
	if(bytesNumber > 4 && bytesNumber != 8){
		printf("error, bytes Number > 8!\n");
		return 0;
	}
    
    ret = ioctl(slaveDeviceId, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1){
		printf("error, spi write failed\n");
		return 0;
	}
    return bytesNumber;
}

/******************************************************************************
 * @brief Close SPI fd.
 * @return None
*******************************************************************************/
void SPI_Close(int fd)
{
	close(fd);
}
