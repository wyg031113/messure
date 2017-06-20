#define AD7124_INIT   /* We want to use the ad7124_regs array defined in ad7124_regs.h */
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
extern "C" {
    #include"AD7124.h"
    #include"AD7124_regs.h"
}
int g_gain = 0;
int g_power_mode = 2;
void show_all_reg(struct ad7124_device *ad7124_handler);
struct sensor_config inner_temp;
struct sensor_config inner_ref;
int test_ref(struct ad7124_device *device)
{
	int ret;
	int i;
	int sample;
	int ch_num;
    long timeout = 1000;

	inner_ref.ad7124_handler = device;
	inner_ref.set = ad7124_get_setting(2);
	inner_ref.ch = ad7124_get_ch(2);
	inner_ref.set->config.value |= AD7124_CFG_REG_REF_SEL(0);
	inner_ref.set->config.value &= ~AD7124_CFG_REG_BIPOLAR;
	ad7124_setup_channel(inner_ref.ch, 2, 0x12, 0x11);
	CHECK_RET((ret = ad7124_config_sensor(&inner_ref)), SUCCESS, ret);
	ad7124_change_mode(inner_ref.ad7124_handler, -1, 1, 2, -1);
//	show_all_reg(device);    
	for(i = 0; i < 10; i++)
	{

		CHECK_RET((ret = AD7124_WaitForConvReady(inner_ref.ad7124_handler, timeout)),
					SUCCESS, ret);
		CHECK_RET((ret = AD7124_ReadData(inner_ref.ad7124_handler, &sample)), 
					SUCCESS, ret);
		ch_num = AD7124_Convert_channel_Get(inner_ref.ad7124_handler);
		printf("ch:%d sample:%x battery votage:%.5fV\n", ch_num, sample, sample*1.0/0x01000000*3.255);

	}
    return 0;
}
int test_battery(struct ad7124_device *device)
{
	int ret;
	int i;
	int sample;
	int ch_num;
    long timeout = 1000;
	struct sensor_config battery;
	int gain=0; //0表示增益为2^gain
	int power_mode = 2;
	gain = g_gain;
	power_mode = g_power_mode;
	battery.ad7124_handler = device;
	battery.set = ad7124_get_setting(0); //设置
	battery.ch = ad7124_get_ch(0); //通道
	battery.set->config.value |= AD7124_CFG_REG_REF_SEL(0)|gain; //参考电压选择和增益设置
	battery.set->config.value |= AD7124_CFG_REG_BIPOLAR;		 //选择双极性
	//battery.set->config.value &= ~AD7124_CFG_REG_BIPOLAR;
	ad7124_setup_channel(battery.ch, 0, 0x0, 0x1);				 //选择Ain0 和 Ain1
	CHECK_RET((ret = ad7124_config_sensor(&battery)), SUCCESS, ret);
	ad7124_change_mode(battery.ad7124_handler, -1, -1, power_mode, -1);   //功耗模式2
//	show_all_reg(device);    
	for(i = 0; i < 10; i++)
	{

		CHECK_RET((ret = AD7124_WaitForConvReady(battery.ad7124_handler, timeout)),
					SUCCESS, ret);
		CHECK_RET((ret = AD7124_ReadData(battery.ad7124_handler, &sample)), 
					SUCCESS, ret);
		ch_num = AD7124_Convert_channel_Get(battery.ad7124_handler);
		printf("ch:%d sample:%x battery votage:%.5fV\n", ch_num, sample, (sample*1.0/(1<<23)-1)*(1.798 + 1.799) / (1<<gain));

	}
    return 0;
}
int test_battery2(struct ad7124_device *device)
{
	int ret;
	int i;
	int sample;
	int ch_num;
    long timeout = 1000;
struct sensor_config battery;
	battery.ad7124_handler = device;
	battery.set = ad7124_get_setting(1);
	battery.ch = ad7124_get_ch(1);
	battery.set->config.value |= AD7124_CFG_REG_REF_SEL(0)|2;
	battery.set->config.value &= ~AD7124_CFG_REG_BIPOLAR;
	ad7124_setup_channel(battery.ch, 1, 0x2, 0x3);
	CHECK_RET((ret = ad7124_config_sensor(&battery)), SUCCESS, ret);
	ad7124_change_mode(battery.ad7124_handler, -1, -1, 2, -1);
//	show_all_reg(device);    
	for(i = 0; i < 10; i++)
	{

		CHECK_RET((ret = AD7124_WaitForConvReady(battery.ad7124_handler, timeout)),
					SUCCESS, ret);
		CHECK_RET((ret = AD7124_ReadData(battery.ad7124_handler, &sample)), 
					SUCCESS, ret);
		ch_num = AD7124_Convert_channel_Get(battery.ad7124_handler);
		printf("ch:%d sample:%x battery votage:%.3fV\n", ch_num, sample, sample*1.0/0x01000000*3.25);

	}
    return 0;
}
int test_sensor(struct ad7124_device *device)
{
	int ret;
	int i;
	int sample;
	int ch_num;
    long timeout = 1000;
	inner_temp.ad7124_handler = device;
	inner_temp.set = ad7124_get_setting(7);
	inner_temp.ch = ad7124_get_ch(7);
	inner_temp.set->config.value |= AD7124_CFG_REG_REF_SEL(2)|0;
	inner_temp.set->config.value &= ~AD7124_CFG_REG_BIPOLAR;
	ad7124_setup_channel(inner_temp.ch, 7, 0x10, 0x12);
	CHECK_RET((ret = ad7124_config_sensor(&inner_temp)), SUCCESS, ret);
	ad7124_change_mode(inner_temp.ad7124_handler, -1, 1, 2, -1);
//	show_all_reg(device);    
	for(i = 0; i < 10; i++)
	{

		CHECK_RET((ret = AD7124_WaitForConvReady(inner_temp.ad7124_handler, timeout)),
					SUCCESS, ret);
		CHECK_RET((ret = AD7124_ReadData(inner_temp.ad7124_handler, &sample)), 
					SUCCESS, ret);
		ch_num = AD7124_Convert_channel_Get(inner_temp.ad7124_handler);
		printf("ch:%d sample:%x\n", ch_num, sample);
		if(ch_num == 7)
			printf("chip temp:%.2f\n", 1.0*(sample)/13584-272.5);

	}
    return 0;
}

/* Read all registers */
void show_all_reg(struct ad7124_device *ad7124_handler)
{
	int ret;
    int regNr;                       /* Variable to iterate through registers */
    for (regNr = AD7124_Status; (regNr < AD7124_REG_NO) && !(ret < 0); regNr++){
        ret = AD7124_ReadRegister(ad7124_handler, &ad7124_regs[regNr]);
		printf("ret:%d regno:%d size:%d value:%x\n", ret, regNr, ad7124_regs[regNr].size, ad7124_regs[regNr].value);
    }
}
int test(int argc, char *argv[])
{
    struct ad7124_device my_ad7124;                    /* A new driver instance */
    struct ad7124_device *ad7124_handler = &my_ad7124; /* A driver handle to pass around */
    //enum ad7124_registers regNr;                       /* Variable to iterate through registers */
    long timeout = 1000;                               /* Number of tries before a function times out */
    long ret = 0;                                      /* Return value */
    int sample;                                       /* Stores raw value read from the ADC */
	int i;
	if(argc != 3){
		printf("Usage:addriver gain power_mode\n");
		return 1;
	}
	g_gain = atoi(argv[1]);
	g_power_mode = atoi(argv[2]);
    /* Initialize AD7124 device. */
    ret = AD7124_Setup(ad7124_handler, AD7124_SLAVE_ID, (ad7124_st_reg *)&ad7124_regs);
    if (ret < 0){
		INFO("setup 7124 failed.\n");
		exit(-1);
    }
    else{
		printf("setup ad7124 success.\n");
    } 

	show_all_reg(ad7124_handler);    
 
    /* Read data from the ADC */
    ret = AD7124_WaitForConvReady(ad7124_handler, timeout);
    if (ret < 0){
		printf("wait failed.\n");
    }

	for(i = 0; i < 10; i++){
		ret = AD7124_ReadData(ad7124_handler, &sample);
		if (ret < 0){
			INFO("Read data failed.\n");
			break;
		}
		printf("sample data:%x\n", sample);
	}//end for
	//test_sensor(ad7124_handler);
	test_battery(ad7124_handler);
//	test_battery2(ad7124_handler);
//	test_ref(ad7124_handler);
	INFO("main finished.\n");
	return 0;
}
