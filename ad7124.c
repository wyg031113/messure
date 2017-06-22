/***************************************************************************//**
*   @file    AD7124.c
*   @brief   AD7124 implementation file.
*   @devices AD7124-4, AD7124-8
*
********************************************************************************
* Copyright 2015(c) Analog Devices, Inc.
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
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
* THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT, MERCHANTABILITY
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "Communication.h"
#include "AD7124.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/* Error codes */
#define INVALID_VAL -1 /* Invalid argument */
#define COMM_ERR    -2 /* Communication error on receive */
#define TIMEOUT     -3 /* A timeout has occured */


/*Ad7124 all have 8 settins.
 */
#define AD7124_SET_NUM 8
static struct ad7124_setting  ad7124_settings[AD7124_SET_NUM] = {
	/*config					filter					offset						gain	*/
	{{0x19, 0x860, 2, 1}, {0x21, 0x060180, 3, 1}, {0x29, 0x800000, 3, 1}, {0x31, 0x5555f7, 3, 1}},
	{{0x1a, 0x860, 2, 1}, {0x22, 0x060180, 3, 1}, {0x2a, 0x800000, 3, 1}, {0x32, 0x5555f7, 3, 1}},
	{{0x1b, 0x860, 2, 1}, {0x23, 0x060180, 3, 1}, {0x2b, 0x800000, 3, 1}, {0x33, 0x5555f7, 3, 1}},
	{{0x1c, 0x860, 2, 1}, {0x24, 0x060180, 3, 1}, {0x2c, 0x800000, 3, 1}, {0x34, 0x5555f7, 3, 1}},
	{{0x1d, 0x860, 2, 1}, {0x25, 0x060180, 3, 1}, {0x2d, 0x800000, 3, 1}, {0x35, 0x5555f7, 3, 1}},
	{{0x1e, 0x860, 2, 1}, {0x26, 0x060180, 3, 1}, {0x2e, 0x800000, 3, 1}, {0x36, 0x5555f7, 3, 1}},
	{{0x1f, 0x860, 2, 1}, {0x27, 0x060180, 3, 1}, {0x2f, 0x800000, 3, 1}, {0x37, 0x5555f7, 3, 1}},
	{{0x20, 0x860, 2, 1}, {0x28, 0x060180, 3, 1}, {0x30, 0x800000, 3, 1}, {0x38, 0x5555f7, 3, 1}},
};

#define AD7124_CH_NUM 16
static ad7124_st_reg channel[16] = {
	{0x09, 0x8001, 2, 1}, {0x0a, 0x0001, 2, 1}, {0x0b, 0x0001, 2, 1}, {0x0c, 0x0001, 2, 1},
	{0x0d, 0x0001, 2, 1}, {0x0e, 0x0001, 2, 1}, {0x0f, 0x0001, 2, 1}, {0x10, 0x0001, 2, 1},
	{0x11, 0x0001, 2, 1}, {0x12, 0x0001, 2, 1}, {0x13, 0x0001, 2, 1}, {0x14, 0x0001, 2, 1},
	{0x15, 0x0001, 2, 1}, {0x16, 0x0001, 2, 1}, {0x17, 0x0001, 2, 1}, {0x18, 0x0001, 2, 1},
};

/***************************************************************************//**
* @brief	get a channel config
* @param id - channel code, 0<=id<=16
* @return Returns channel id's pointer for success or NULL for failed.
*******************************************************************************/
ad7124_st_reg * ad7124_get_ch(int id)
{
	if(id < 0 || id >= AD7124_CH_NUM)
		return NULL;
	return &channel[id];
}

/***************************************************************************//**
* @brief	get a setting.
* @param id - setting code, 0<=id<=16
* @return Returns settings id's pointer for success or NULL for failed.
*******************************************************************************/
struct ad7124_setting *ad7124_get_setting(int id)
{
	if(id < 0 || id >= AD7124_SET_NUM)
		return NULL;
	return &ad7124_settings[id];
}

/***************************************************************************//**
* @brief Apply setting to ad7124, include config, filter, offset, gain regs.
* @param device - The handler of the instance of the driver.
* @param s - Pointer to the settings group
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int ad7124_setting_apply(struct ad7124_device *dev, struct ad7124_setting *s)
{
	int ret;
	CHECK_RET((ret = AD7124_WriteRegister(dev, s->config)), SUCCESS, ret);
	CHECK_RET((ret = AD7124_WriteRegister(dev, s->filter)), SUCCESS, ret);
	CHECK_RET((ret = AD7124_WriteRegister(dev, s->offset)), SUCCESS, ret);
	CHECK_RET((ret = AD7124_WriteRegister(dev, s->gain)), SUCCESS, ret);
	return SUCCESS;
}

/***************************************************************************//**
* @brief Apply channel set to ad7124
* @param device - The handler of the instance of the driver.
* @param ch - Pointer to the channel reg
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int ad7124_channel_apply(struct ad7124_device *dev,  ad7124_st_reg  *ch)
{
	int ret;
	CHECK_RET((ret = AD7124_WriteRegister(dev, *ch)), SUCCESS, ret);
	return SUCCESS;
}

/***************************************************************************//**
* @brief Apply a sensor's config. after setup, can get sample value from data reg
* @param sc		- Pointer to a sensor's config struct
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int ad7124_config_sensor(struct sensor_config *sc)
{
	int ret;
	CHECK_RET((ret = ad7124_change_mode(sc->ad7124_handler, -1, -1, -1, AD7124_WORK_MODE_STAND_BY)),
				SUCCESS, ret);
	CHECK_RET((ret = ad7124_setting_apply(sc->ad7124_handler, sc->set)), SUCCESS, ret);
	CHECK_RET((ret = ad7124_channel_apply(sc->ad7124_handler, sc->ch)), SUCCESS, ret);
	CHECK_RET((ret = ad7124_change_mode(sc->ad7124_handler, 1, 0, AD7124_POWER_MODE_LOW,
				AD7124_WORK_MODE_CONTINUOUS)),	SUCCESS, ret);
	return SUCCESS;
}

/***************************************************************************//**
* @brief setup a channle, but no apply to ad7124 reg.
* @param ch		- channel reg pointer
* @param bipolar - double or single 
* @param set_n  - num of 8 settings.
* @param ainp   - num of the input, can be a extern input line, or inner input
*				  it's represent a positive input
* @param ainm   - num of the input, can be a extern input line, or inner input
*				  it's represent a negtive input
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int ad7124_setup_channel(ad7124_st_reg *ch, int set_n, int ainp, int ainm)
{
	if(ch == NULL)
		return NULL_POINTER_ERR;
	ch->value = AD7124_CH_MAP_REG_SETUP(set_n) | AD7124_CH_MAP_REG_AINP(ainp) |
	             AD7124_CH_MAP_REG_AINM(ainm) | AD7124_CH_MAP_REG_CH_ENABLE;
	return SUCCESS;
}

/***************************************************************************//**
* @brief Disable a channel, apply to ad7124 reg.
* @param device - The handler of the instance of the driver.
* @param ch		- Pointer to a chanel
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int ad7124_disable_channel(ad7124_device *dev, ad7124_st_reg *ch)
{
	int ret;
	if(ch == NULL)
		return NULL_POINTER_ERR;
	ch->value &= ~ AD7124_CH_MAP_REG_CH_ENABLE;
	DEBUG("Ch->value=%x reg_addr:%x\n",  ch->value, ch->addr);
	CHECK_RET((ret = AD7124_WriteRegister(dev, *ch)), SUCCESS, ret);
	return SUCCESS;
}

/***************************************************************************//**
* @brief set ad7124 adc control register
* @param device - The handler of the instance of the driver.
* @param data_status - if data_status is -1, not set, else set to data-status(0 or 1)
* @param ref_en		 - if ref_en is -1, out put  inner ref votage to refout
* @param power_mode	 - low power, middle power, or full power
* @param work_mode	 - 0-continuous convert, 1-single convert, 2-standy, 3-off, 4-idle
*						macros of this mode see AD7124_regs.h
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int ad7124_change_mode(ad7124_device *dev, int data_status, int ref_en, 
						int power_mode, int work_mode)
{
	int ret;
	ad7124_st_reg adc_control = {0x01, 0x0000, 2, 1};
	CHECK_RET((ret = AD7124_ReadRegister(dev, &adc_control)), SUCCESS, ret);
	if(data_status != -1){
		adc_control.value &= ~AD7124_ADC_CTRL_REG_DATA_STATUS;
		adc_control.value |= data_status ? AD7124_ADC_CTRL_REG_DATA_STATUS : 0;
	}
	
	if(ref_en != -1){
		adc_control.value &= ~AD7124_ADC_CTRL_REG_REF_EN;
		adc_control.value |= ref_en ? AD7124_ADC_CTRL_REG_REF_EN : 0;
	}
	if(power_mode != -1){
		adc_control.value &= ~AD7124_ADC_CTRL_REG_POWER_MODE(0x3);
		adc_control.value |= AD7124_ADC_CTRL_REG_POWER_MODE(power_mode);
	}
	if(work_mode != -1){
		adc_control.value &= ~AD7124_ADC_CTRL_REG_MODE(0xf);
		adc_control.value |= AD7124_ADC_CTRL_REG_MODE(work_mode);
	}

	CHECK_RET((ret = AD7124_WriteRegister(dev, adc_control)), SUCCESS, ret);
	return SUCCESS;
}
/***************************************************************************//**
* @brief Reads the value of the specified register without checking if the
*        device is ready to accept user requests.
*
* @param device - The handler of the instance of the driver.
* @param pReg - Pointer to the register structure holding info about the
*               register to be read. The read value is stored inside the
*               register structure.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_NoCheckReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{
	int32_t ret       = 0;
	uint8_t buffer[8] = {0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0};
	int i         = 0;

	if(!device || !pReg)
		return INVALID_VAL;
	/* Build the Command word */
	buffer[3] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD |
	  		AD7124_COMM_REG_RA(pReg->addr);

	/* Read data from the device */
	ret = SPI_Read(device->slave_select_id,buffer, 4);
	if(ret != 4){
		printf("spi read failed in no check read.\n");
		return SPI_READ_ERR;
	}

	/* Build the result */
	pReg->value = 0;
	pReg->value = 0;
	for(i = 2; i >= 3 - pReg->size && i>=0; i--)
	{
		pReg->value <<= 8;
		pReg->value += buffer[i];
	}
	return SUCCESS;
}

/***************************************************************************//**
* @brief Writes the value of the specified register without checking if the
*        device is ready to accept user requests.
*
* @param device - The handler of the instance of the driver.
* @param reg - Register structure holding info about the register to be written
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_NoCheckWriteRegister(ad7124_device *device, ad7124_st_reg reg)
{
	int32_t ret      = 0;
	int32_t regValue = 0;
	uint8_t wrBuf[8] = {0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0};
	int i        = 0;

	if(!device)
		return INVALID_VAL;

	/* Build the Command word */
	wrBuf[3] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_WR |
	  		AD7124_COMM_REG_RA(reg.addr);
	if(reg.size >= 4){
		printf("error, reg size too large, can't write.\n");
		return INVALID_VAL;
	}
	/* Fill the write buffer */
	regValue = reg.value;
	for(i = 0; i < reg.size; i++)
	{
		wrBuf[3-reg.size+i] = regValue & 0xFF;
		regValue >>= 8;
	}

	/* Write data to the device */
	ret = SPI_Write(device->slave_select_id, wrBuf, 4);
	if(ret != 4){
		INFO("SPI Writ failed.\n");
		return SPI_WRITE_ERR;
	}
	return SUCCESS;
}

/***************************************************************************//**
* @brief Reads the value of the specified register only when the device is ready
*        to accept user requests. If the device ready flag is deactivated the
*        read operation will be executed without checking the device state.
*
* @param device - The handler of the instance of the driver.
* @param pReg - Pointer to the register structure holding info about the
*               register to be read. The read value is stored inside the
*               register structure.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_ReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{
	int32_t ret;
	
	if (pReg->addr != AD7124_ERR_REG && device->check_ready)
	{
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0){
			printf("ad7124 read failed.\n");
			return ret;
		}
	}
	ret = AD7124_NoCheckReadRegister(device, pReg);
	
	return ret;
}

/***************************************************************************//**
* @brief Writes the value of the specified register only when the device is
*        ready to accept user requests. If the device ready flag is deactivated
*        the write operation will be executed without checking the device state.
*
* @param device - The handler of the instance of the driver.
* @param reg - Register structure holding info about the register to be written
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WriteRegister(ad7124_device *device, ad7124_st_reg pReg)
{
	int32_t ret;
	
	if (device->check_ready)
	{  
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0){
			printf("wait for spi failed.\n");
			return ret;
		}
	}
	ret = AD7124_NoCheckWriteRegister(device, pReg);
	
	return ret;
}
/***************************************************************************//**
* @brief Get ad7124 device ID:0x12.
*
* @param device - The handler of the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Device_ID_Get(ad7124_device *device)
{
	int ret;
	ad7124_st_reg devid = {AD7124_ID_REG, 0, 1, AD7124_R};
	ret = AD7124_ReadRegister(device, &devid);
	if(ret != SUCCESS){
		return ret;
	}else{
		return devid.value;
	}
}

/***************************************************************************//**
* @brief Get ad7124 status reg
* @param device - The handler of the instance of the driver.
* @return Returns status for success or negative error code.
*******************************************************************************/
int32_t AD7124_Status_Get(ad7124_device *device)
{
	int ret;
	ad7124_st_reg status = {AD7124_STATUS_REG, 0, 1, AD7124_R};
	ret = AD7124_ReadRegister(device, &status);
	if(ret != SUCCESS){
		return ret;
	}else{
		return status.value;
	}
}

/***************************************************************************//**
* @brief Get convert channel, must call this after get convert data
* @param device - The handler of the instance of the driver.
* @return Returns convert channel num for success or negative error code.
*******************************************************************************/
int32_t AD7124_Convert_channel_Get(ad7124_device *device)
{
	int ret;
	ad7124_st_reg status = {AD7124_STATUS_REG, 0, 1, AD7124_R};
	ret = AD7124_ReadRegister(device, &status);
	printf("GetStatus:%d status:%x\n", ret, status.value);
	if(ret != SUCCESS){
		return ret;
	}else{
		return status.value & 0xf;
	}
}

/***************************************************************************//**
* @brief Resets the device.
*
* @param device - The handler of the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Reset(ad7124_device *device)
{
	int32_t ret = 0;
	uint32_t status;
	uint8_t wrBuf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	if(!device)
		return INVALID_VAL;

	ret = SPI_Write(device->slave_select_id, wrBuf, 8);
	if(ret < 0){
		INFO("reset ad7124 failed.\n");
		return RESET_AD7124_ERR;
	}
	status = AD7124_Status_Get(device);
	if(status & AD7124_STATUS_REG_POR_FLAG)
		return SUCCESS;
	else
		return ret;
}

/***************************************************************************//**
* @brief Waits until the device can accept read and write user actions.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitForSpiReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!ready && --timeout)
	{
		/* Read the value of the Error Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Error]);
		if(ret < 0){
			printf("wait:read reg failed.\n");
			return ret;
		}

		/* Check the SPI IGNORE Error bit in the Error Register */
		ready = (regs[AD7124_Error].value &
			 	AD7124_ERR_REG_SPI_IGNORE_ERR) == 0;
	}

	return timeout ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Waits until a new conversion result is available.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns if no new data is available.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitForConvReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!ready && --timeout)
	{
		/* Read the value of the Status Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Status]);
		if(ret < 0)
			return ret;

		/* Check the RDY bit in the Status Register */
		ready = (regs[AD7124_Status].value &
				AD7124_STATUS_REG_RDY) == 0;
	}

	return timeout ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Reads the conversion result from the device.
*
* @param device - The handler of the instance of the driver.
* @param pData - Pointer to store the read data.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_ReadData(ad7124_device *device, int32_t* pData)
{
	ad7124_st_reg *regs;
	int32_t ret;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	/* Read the value of the Data Register */
	ret = AD7124_ReadRegister(device, &regs[AD7124_Data]);

	/* Get the read result */
	*pData = regs[AD7124_Data].value;

	return ret;
}

/***************************************************************************//**
* @brief Computes the CRC checksum for a data buffer.
*
* @param pBuf - Data buffer
* @param bufSize - Data buffer size in bytes
*
* @return Returns the computed CRC checksum.
*******************************************************************************/
uint8_t AD7124_ComputeCRC8(uint8_t * pBuf, uint8_t bufSize)
{
	uint8_t i   = 0;
	uint8_t crc = 0;

	while(bufSize)
	{
		for(i = 0x80; i != 0; i >>= 1)
		{
			if(((crc & 0x80) != 0) != ((*pBuf & i) != 0)) /* MSB of CRC register XOR input Bit from Data */
			{
				crc <<= 1;
				crc ^= AD7124_CRC8_POLYNOMIAL_REPRESENTATION;
			}
			else
			{
				crc <<= 1;
			}
		}
		pBuf++;
		bufSize--;
	}
	return crc;
}

/***************************************************************************//**
* @brief Updates the CRC settings.
*
* @param device - The handler of the instance of the driver.
*
* @return None.
*******************************************************************************/
void AD7124_UpdateCRCSetting(ad7124_device *device)
{
	ad7124_st_reg *regs;

	if(!device)
		return;

	regs = device->regs;
	
	/* Get CRC State. */
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_CRC_ERR_EN)
	{
		device->useCRC = AD7124_USE_CRC;
	}
	else
	{
		device->useCRC = AD7124_DISABLE_CRC;
	}
}

/***************************************************************************//**
* @brief Updates the device SPI interface settings.
*
* @param device - The handler of the instance of the driver.
*
* @return None.
*******************************************************************************/
void AD7124_UpdateDevSpiSettings(ad7124_device *device)
{
	ad7124_st_reg *regs;

	if(!device)
		return;

	regs = device->regs;
	
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_IGNORE_ERR_EN)
	{
		device->check_ready = 1;
	}
	else
	{
		device->check_ready = 0;
	}
}

/***************************************************************************//**
* @brief Initializes the AD7124.
*
* @param device - The handler of the instance of the driver.
* @param slave_select - The Slave Chip Select Id to be passed to the SPI calls.
* @param regs - The list of registers of the device (initialized or not) to be
*               added to the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Setup(const char *dev, ad7124_device *device, int slave_select,
			ad7124_st_reg *regs)
{
	int32_t ret;
	if(!device || !regs)
		return INVALID_VAL;

	device->regs = regs;
	device->slave_select_id = slave_select;
	device->spi_rdy_poll_cnt = 25000;

	/* Initialize the SPI communication. */
	ret = SPI_Init(dev, 0, 10000, 1, 1);
	if (ret < 0){
		INFO("SPI_Init failed.\n");
		return SPI_INIT_ERR;
	}else{
		device->slave_select_id = ret;
	}
	INFO("SPI INIT OK.\n");

	/*Reset the device interface.*/
	ret = AD7124_Reset(device);
	if (ret < 0){
		INFO("ad reset failed.\n");
		goto failed2;
	}else{
		INFO("AD7124 reset successuflly.\n");
	}

	/*Get device to confirm ad7124 work ok.*/
	ret = AD7124_Device_ID_Get(device);
	if (ret < 0){
		INFO("AD7124 device id get failed.\n");
		goto failed2;
	}else {
		INFO("get device id:%x.\n", ret);
	}
	return SUCCESS;
failed2:
	SPI_Close(device->slave_select_id);
	return ret;
}

/***************************************************************************//**
* @brief stop ad7124
* @param device - The handler of the instance of the driver.
* @return None
*******************************************************************************/
void Ad7124_Stop(ad7124_device *device)
{
	ad7124_change_mode(device, -1, -1, -1, AD7124_WORK_MODE_OFF);
	SPI_Close(device->slave_select_id);
}
