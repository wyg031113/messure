/*************************************************************************
    > File Name: meter.c
    > Author: wyg
    > Mail: wyg_0802@126.com 
    > Created Time: 2017年06月19日 星期一 14时14分18秒
 ************************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define AD7124_INIT   /* We want to use the ad7124_regs array defined in ad7124_regs.h */
#include "gpio_ctl.h"
#include "debug.h"
#include "AD7124.h"
#include "meter.h"

#define REF_OUT 0
#define N_CODE 5			//采样5个值
#define N_TEMP 1024
static int ntemp = 0;
static double ref_low_v		= -1.8;
static double ref_high_v	=  1.8;
const static int timeout    = 1000;      //ms

static struct ad7124_device my_ad7124;                    /* A new driver instance */
static struct ad7124_device *ad7124_handler = &my_ad7124; /* A driver handle to pass around */

static temperature_tab_t tabs[N_TEMP];      //

/*
 * 测试double变量d是不是0
 * d: 要测试的double类型变量
 * 返回：是0，真；非0,假。
 */
static int check_double_zero(double d)
{
	if(fabs(d) < 1e-6)
		return 1;
	else
		return 0;
}
/*
 * 配置参考电压
 * ref_low  负参考电压
 * ref_high 正参考电压
 */
void config_ref_voltage(double ref_low, double ref_high)
{
	if(check_double_zero(ref_high - ref_low)){
		INFO("set ref voltage failed, because ref_low == ref_high = %f\n", ref_low);
		return;
	}
	ref_low_v  = ref_low;
	ref_high_v = ref_high;
}
/*
 * GPIO上电初始化。GPIO0～GPIO4 输出0；
 * GPIO5输出1.
 * return:成功返回 SUCCESS
 */
int gpio_init(void)
{
	int i;
    int gpio[] =		{GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6};
    int init_value[] =	{0,		0,	   0,	  0,	 0,	    0    , 1    };
	int ret = SUCCESS;
	for(i = 0; i < sizeof(gpio)/sizeof(int); i++){
		int r = set_gpio_value(gpio[i], init_value[i]);
		if(r != 0){
			INFO("set gpio %d failed.\n", gpio[i]);
			ret |= r;
		}
	}
	return ret;
}


/* Read all registers */
void show_all_reg(struct ad7124_device *ad7124_handler)
{
	int ret;
    enum ad7124_registers regNr;                       /* Variable to iterate through registers */
    for (regNr = AD7124_Status; (regNr < AD7124_REG_NO) && !(ret < 0); regNr++){
        ret = AD7124_ReadRegister(ad7124_handler, &ad7124_regs[regNr]);
		printf("ret:%d regno:%d size:%d value:%x\n", ret, regNr, ad7124_regs[regNr].size, ad7124_regs[regNr].value);
    }
}

/*
 * 初始化AD7124 打开SPI，关闭默认已经开启的通道0
 * return:成功：0，失败：-1。
 */
int setup_ad7124(const char *spidev)
{
    int ret = 0;                                      /* Return value */
    /* Initialize AD7124 device. */
    ret = AD7124_Setup(spidev, ad7124_handler, AD7124_SLAVE_ID, (ad7124_st_reg *)&ad7124_regs);
    if (ret < 0){
		INFO("setup 7124 failed.\n");
		return -1;
    }
    else{
		INFO("setup ad7124 successfully.\n");
    } 

	int ch_num = AD7124_Convert_channel_Get(ad7124_handler);
	DEBUG("before disable channel 0:ch=%d \n", ch_num);	
	ret = ad7124_disable_channel(ad7124_handler, ad7124_get_ch(0));
	ch_num = AD7124_Convert_channel_Get(ad7124_handler);
	DEBUG("after disable channel 0:ch=%d \n", ch_num);	
	show_all_reg(ad7124_handler);    
	return SUCCESS;
}

/*
 * ad_in1 取值范围0-15
 * ad_in2 取值范围0-15
 * ad_set 取值范围0-7
 * ad_channel 取值范围0-15
 * ad_polar   取值范围UNIPOLAR, BIPOLAR
 * ad_gain    取值范围0~7
 * ad_power_mode 取值范围AD7124_POWER_MODE_LOW, AD7124_POWER_MODE_MIDDLE, AD7124_POWER_MODE_HIGH
 */
typedef struct Param
{
	int ad_in1;			//AD输入1
	int ad_in2;			//AD输入2
	int ad_set;			//AD设置编号
	int ad_channel;		//AD通道编号
	int ad_polar;		//AD输入极性，单极性，双极性
	int ad_gain;		//AD增益
	int ad_power_mode;	//AD功耗模式
}Param_t;

/*
 * 绝缘电阻测量
 */
Param_t ins_res_param = 
{
	.ad_in1		=	6,
	.ad_in2		=	7,
	.ad_set		=	0,
	.ad_channel	=	0,
	.ad_polar   = BIPOLAR,
	.ad_gain	=	0,
	.ad_power_mode = AD7124_POWER_MODE_HIGH,
};


/*
 *温度测量 
 */
Param_t temperature_param = 
{
	.ad_in1		=	8,
	.ad_in2		=	9,
	.ad_set		=	1,
	.ad_channel	=	1,
	.ad_polar   = BIPOLAR,
	.ad_gain	=	0,
	.ad_power_mode = AD7124_POWER_MODE_HIGH,
};

/*
 * 压力测量
 */
Param_t pressure_param1 = 
{
	.ad_in1		=	4,
	.ad_in2		=	5,
	.ad_set		=	2,
	.ad_channel	=	2,
	.ad_polar   = BIPOLAR,
	.ad_gain	=	0,
	.ad_power_mode = AD7124_POWER_MODE_HIGH,
};
Param_t pressure_param2 = 
{
	.ad_in1		=	2,
	.ad_in2		=	3,
	.ad_set		=	3,
	.ad_channel	=	3,
	.ad_polar   = BIPOLAR,
	.ad_gain	=	0,
	.ad_power_mode = AD7124_POWER_MODE_HIGH,
};

/*
 *输入输出阻抗测量
 */
Param_t in_out_resis_param = 
{
	.ad_in1		=	0,
	.ad_in2		=	1,
	.ad_set		=	4,
	.ad_channel	=	4,
	.ad_polar   = BIPOLAR,
	.ad_gain	=	0,
	.ad_power_mode = AD7124_POWER_MODE_HIGH,
};

int reset_ad7124(struct ad7124_device *device)
{

	int ret = 0;
	 ret = AD7124_Reset(device);
	 if(ret < 0){
		 INFO("Reset AD7124 Failed.\n");
		 return -1;
	 }
	 ret = ad7124_disable_channel(ad7124_handler, ad7124_get_ch(0));

	 if(ret < 0){
		 INFO("Reset disable channle 0 failed.\n");
		 return -1;
	 }
	 return 0;
}
/*
 * 配置AD
 * 启动转换
 * return:成功：0，失败：-1。
 */

int start_ad_convert(struct ad7124_device *device, const Param_t *param)
{
	int ret = 0;
	struct sensor_config sc;
	sc.ad7124_handler = device;
	sc.set = ad7124_get_setting(param->ad_set);							// 选择设置
	sc.ch  = ad7124_get_ch(param->ad_channel);								// 选择通道
	sc.set->config.value |= AD7124_CFG_REG_REF_SEL(REF_OUT);			// 参考电压源
	sc.set->config.value |= param->ad_gain;								// 参考电压源
	if(param->ad_polar == BIPOLAR){
		sc.set->config.value |= AD7124_CFG_REG_BIPOLAR;					// 选择双极性
	}else{
		sc.set->config.value &= ~AD7124_CFG_REG_BIPOLAR;				// 单极性
	}
	ad7124_setup_channel(sc.ch, param->ad_set, 
						 param->ad_in1, param->ad_in2);					// 选择Ain0 和 Ain1
	ret = ad7124_config_sensor(&sc);									// 应用配置，开始采集转换

	ad7124_change_mode(device, -1, -1,  
								param->ad_power_mode, -1);				// 功耗模式2
	//show_all_reg(device);
    DEBUG("start ad convert\n");
	usleep(100000);
	return ret;
}

/*
 * device: ad7124设备
 * val:	   接受AD采样值
 * return:成功：0，失败：-1。
 */
int get_ad_value(struct ad7124_device *device, ad_value_t *val)
{
	int ret = SUCCESS;

	CHECK_RET((ret = AD7124_WaitForConvReady(device, timeout)),
											 SUCCESS, ret);
	//val->ch_num = AD7124_Convert_channel_Get(device);
	val->ch_num = AD7124_Convert_channel_Get(device);
	CHECK_RET((ret = AD7124_ReadData(device, &val->code)), SUCCESS, ret);
	DEBUG("ch=%d code=%x\n", val->ch_num, val->code);	
   // ret = AD7124_ReadRegister(ad7124_handler, &ad7124_regs[regNr]);
	//printf("ret:%d regno:%d size:%d value:%x\n", ret, regNr, ad7124_regs[regNr].size, ad7124_regs[regNr].value);
	//val->ch_num = ad7124_regs[regNr].value & 0xf;
	return ret;
}

/*
 * 将val->code转换成val->voltage,注意要提供param->ad_ch == val->ch_num
 * val：提供码值
 * param:提供增益
 * return:成功：0，失败：-1。
 */
int convert_code2voltage(ad_value_t *val, Param_t *param)
{
	if(param->ad_channel != val->ch_num){
		INFO("error: param->ad_channel != val->ch_num %d %d\n", param->ad_channel, val->ch_num);
		return -1;
	}
	if(param->ad_polar == BIPOLAR){
		val->voltage = (val->code * 1.0/ (1 << 23) - 1) * (ref_high_v - ref_low_v) / (1 << param->ad_gain);
	}else{

		val->voltage = (val->code * 1.0/ (1 << 24)) * (ref_high_v - ref_low_v) / (1 << param->ad_gain);
	}
	return SUCCESS;
}


/*
 * 停止通道转换
 * device：7124设备
 * param: 提供要关闭单通到
 * return:成功：0，失败：-1。
 */
int stop_ad_convert(struct ad7124_device *device, const Param_t *param)
{
	ad7124_disable_channel(device, ad7124_get_ch(param->ad_channel));
	return reset_ad7124(device);
}

/*
 * 获得n个AD转换值
 * ad7124_handler: ad设备
 * vals: 存放转换值
 * n:    获得n个值
 * return:返回实际获得的转换值个数
 */
int get_ad_nval(struct ad7124_device *ad7124_handler, ad_value_t *vals, int n)
{
	int i;
	int ret;
	for(i = 0; i < n; i++){		//采样N_CODE个值
		ret = get_ad_value(ad7124_handler, &vals[i]);
		if(ret < 0){
			INFO("get ad value %d failed!\n", ret);
			break;
		}
	}
	return i;
}
/*
 * 求平均电压
 * vals:存有n个电压值
 * n: 电压值个数
 * return:成功：0，失败：-1。
 */
int average_voltage(ad_value_t *vals, int n, double *d)
{
	int i;
	double voltage = 0;
	if(n == 0){
		INFO("calc average voltage failed:n == 0.!\n");
		return -1;
	}
	for(i = 0; i < n; i++){				//求平均
		voltage += vals[i].voltage;
	}
	voltage /=  n;
	*d = voltage;
	return 0;
}

/***************************************************************************
 *绝缘电阻测量
 ***************************************************************************/
/*
 * 启动AD通道
 * return:成功：0，失败：-1。
 */
int resistance_start()
{
	DEBUG("before start resistance..\n");
	int ret =  start_ad_convert(ad7124_handler, &ins_res_param);
	DEBUG("after start resistance ret=%d\n", ret);
	return ret;
}

/*
 *获得绝缘电阻值
 * d: 用于返回电阻值
 * return: 成功SUCCESS
 */
int resistance_mesure(double *d)
{
	int i, n;
	int ret;
	double voltage = 0;
	ad_value_t vals[N_CODE];
	n = get_ad_nval(ad7124_handler, vals, N_CODE);
	for(i = 0; i < n; i++)
		convert_code2voltage(&vals[i], &ins_res_param);
	ret = average_voltage(vals, n, &voltage);	
	if(ret != SUCCESS){
		INFO("agerage voltage failed!\n");
		return -1;
	}
	if(check_double_zero(voltage))
		return -1;
	//计算绝缘电阻值
    voltage /= 251;
    double resis = (0.6712)/(voltage*voltage) + 6.24/voltage + 0.3276 ; //MoM
	DEBUG("resistance voltage: %e resis:%f\n", voltage, resis);
	*d = resis;
	return SUCCESS;
}

/*
 *停止AD通道
 * return:成功：0，失败：-1。
 */
int resistance_stop()
{
	return stop_ad_convert(ad7124_handler, &ins_res_param);
}

/*****************************************************************************
 * EEPROM 操作
 *****************************************************************************/
/*
 * 配置GPIO，启动EEPROM
 * return:成功：0，失败：-1。
 */
int start_eeprom(void)
{
	int ret;
	ret = set_gpio_value(GPIO3, 1);
	if(ret < 0){
		INFO("Set GPIO3 to 1 failed!\n");
		return -1;
	}

	ret = set_gpio_value(GPIO6, 0);
	if(ret < 0){
		INFO("Set GPIO6 to 0 failed!\n");
		set_gpio_value(GPIO3, 0);
		return -1;
	}
    usleep(100000);  //us
	return 0;
}

/*
 * 配置GPIO，停止EEPROM
 * return:成功：0，失败：-1。
 */
int stop_eeprom(void)
{
	int ret1, ret2;

	ret2 = set_gpio_value(GPIO6, 1);
	if(ret2 < 0){
		INFO("Set GPIO6 to 1 failed!\n");
	}

	ret1 = set_gpio_value(GPIO3, 0);
	if(ret1 < 0){
		INFO("Set GPIO3 to 0 failed!\n");
	}


	return ret1 | ret2;
}

/*
 * 读EEPROM
 * fname: eeprom文件名
 * buf: 接收数据
 * buf_len: buf大小
 * offset: eeprom偏移
 * return: 返回写了多少字节
 */
int read_eeprom(const char *fname, char *buf, size_t buf_len, size_t offset)
{
	int fd;
	int ret;
    int rbs = 0;
	fd = open(fname, O_RDONLY);
	if(fd < 0){
		INFO("Open faile eeprom file %s failed:%s\n", fname, strerror(errno));
		return fd;
	}
	if(lseek(fd, offset, SEEK_SET) < 0){
		INFO("Seek file %s failed:%s\n", fname, strerror(errno));
		close(fd);
		return -1;
	}
    while(buf_len > 0){
        ret = read(fd, buf + rbs, buf_len);
        if(ret <= 0)
            break;
        buf_len -= ret;
        rbs += ret;
    }
	close(fd);
	return ret;
}

/*
 * 写EEPROM
 * fname: eeprom文件名
 * buf: 要写的数据
 * buf_len: 数据长度
 * offset: eeprom偏移
 * return: 返回写了多少字节
 */
int write_eeprom(const char *fname, const char *buf, size_t buf_len, size_t offset)
{
	int fd;
	int ret;
    int wbs;
    fd = open(fname, O_WRONLY|O_CREAT, 0666);
	if(fd < 0){
		INFO("Open faile eeprom file %s failed:%s\n", fname, strerror(errno));
		return fd;
	}
	if(lseek(fd, offset, SEEK_SET) < 0){
		INFO("Seek file %s failed:%s\n", fname, strerror(errno));
		close(fd);
		return -1;
	}
    while(buf_len > 0){
        ret = write(fd, buf + wbs, buf_len);
        if(ret < 0)
            break;
        buf_len -= ret;
        wbs += ret;
    }
	close(fd);
	return ret;
}

/*
 * 从EEPROM中读取发动机编号、压力参数pk,pb.
 * fname:EEPROM文件名
 * val:输出读取的值
 * return:成功0，失败-1.
 */
int read_eeprom_data(const char *fname, EEPROM_DATA_t *val)
{
    if(read_eeprom(fname, val, sizeof(EEPROM_DATA_t), 0) != sizeof(EEPROM_DATA_t))
        return -1;
    else
        return 0;
}

/*
 * 写入EEPROM发动机编号、压力参数pk,pb.
 * fname:EEPROM文件名
 * val:要写入的值
 * return:成功0，失败-1.
 */
int write_eeprom_data(const char *fname, EEPROM_DATA_t *val)
{
    if(write_eeprom(fname, val, sizeof(EEPROM_DATA_t), 0) != sizeof(EEPROM_DATA_t))
        return -1;
    else
        return 0;
}
/**************************************************************************************
 * 温度测量
 **************************************************************************************/
/*
 * 获取温度对照表
 */
temperature_tab_t *get_temp_tab()
{
	return tabs;
}

/*
 * 获取温度对照表大小
 */
int get_ntemp()
{
	return ntemp;
}

/**
 * 加载温度对照表
 * filename: 温度对照表所在文件。
 * return:成功：0，失败：-1。
 */
int load_temp(const char *filename)
{
	int i = 0;
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		INFO("open temp file %s failed:%s\n", filename, strerror(errno));
		return -1;
	}
	if(fscanf(fp,"%d", &ntemp) != 1){
		INFO("Temperature table file format error, first line must be ntemp\n");
		fclose(fp);
		return -1;
	}
	if(ntemp > N_TEMP){
		INFO("warnning:ntemp too large!\n");
	}
	ntemp = ntemp < N_TEMP ? ntemp : N_TEMP;
	while(i < ntemp && fscanf(fp, "%lf %lf", &tabs[i].res, &tabs[i].temp) == 2)
		i++;
	if(i != ntemp){
		ntemp = i;
		INFO("warnning: Terperature table file may not not have %d items!\n", ntemp);
	}
	fclose(fp);
	return 0;
}

/*
 * 启动AD通道
 * return:成功：0，失败：-1。
 */
int temperature_start()
{
	int ret;
	ret = set_gpio_value(GPIO2, 1);
	if(ret < 0){
		INFO("set gpio2 to 1 faile,start temperature failed.\n");
		return -1;
	}
	ret = set_gpio_value(GPIO0, 1);
	if(ret < 0){
		set_gpio_value(GPIO2, 0);
		INFO("set gpio0 to  1 faile, start temperature failed.\n");
		return -1;
	}
	usleep(100000);
	ret =  start_ad_convert(ad7124_handler, &temperature_param);
	return ret;
}

/*
 * 获得温度值
 * u1 电压3.13
 * tabs:温度对照表
 * n:温度对照表大小
 * d: 用于返回温度值
 * return: 成功SUCCESS
 */
int temperature_mesure(double u1, temperature_tab_t *tabs, int n, double *d)
{
	int i, nval;
	int ret;
	double voltage = 0;
	ad_value_t vals[N_CODE];
	double R1, R2, R4, Rx;
	R1 = 3.9;
	R2 = 3.9;
	R4 = 0.1;
	
	nval = get_ad_nval(ad7124_handler, vals, N_CODE);

	for(i = 0; i < nval; i++)
		convert_code2voltage(&vals[i], &temperature_param);
	ret = average_voltage(vals, nval, &voltage);	
	if(ret != SUCCESS){
		INFO("agerage voltage failed!\n");
        //return -1;
	}

    double u0 = voltage/67.6;
	Rx = (u0 * R2 * (R1 + R4) + u1 * R2 * R4) / (u1 * R1 - u0 * (R1 + R4));
	//计算温度值
	for(i = 0; i < n; i++){
        if(Rx < tabs[i].res)
			break;
	}
	if(i == 0){
		*d = tabs[i].temp;
	}else if(i == n){
		*d = tabs[n-1].temp;
	}else{
		//(y3-y1)/(x3 - x1) == (y2-y1)/(x2-x1)
		*d = (tabs[i].temp - tabs[i-1].temp)/ (tabs[i].res - tabs[i-1].res) * (Rx - tabs[i-1].res) + tabs[i-1].temp;
	}
	
	DEBUG("temperature voltage: %e temp:%f Rx:%f\n", voltage, *d, Rx);
    return ret;
}

/*
 *停止AD通道
 * return:成功：0，失败：-1。
 */
int temperature_stop()
{
	int ret1 = 0;
	int ret2 = 0;
	int ret3 = 0;
	ret1 = stop_ad_convert(ad7124_handler, &temperature_param);
	if(ret1 < 0){
		INFO("Stop ad conver failed!\n");
	}
	ret2 = set_gpio_value(GPIO2, 0);
	if(ret2 < 0){
		INFO("set gpio2 to 2 failed, Stop temperature sensor failed!\n");
	}

	ret3 = set_gpio_value(GPIO0, 0);
	if(ret3 < 0){
		INFO("set gpio0 to 0 failed,Stop temperature sensor failed!\n");
	}
	usleep(100000);
	return ret1 | ret2 | ret3;
}


/**************************************************************************************
 * 压力测量
 **************************************************************************************/
/*
 * 启动AD通道
 * return:成功：0，失败：-1。
 */
int pressure_start()
{
	int ret = 0;
	ret = set_gpio_value(GPIO4, 1);
	if(ret < 0){
		INFO("start temperature failed.\n");
		return -1;
	}
	usleep(100000);
	ret =  start_ad_convert(ad7124_handler, &pressure_param1);
	if(ret < 0){
		INFO("start ad  pressure param1 failed!\n");
        goto failed;
	}
	ret =  start_ad_convert(ad7124_handler, &pressure_param2);
	if(ret < 0){
		INFO("start ad  pressure param2 failed!\n");
        goto failed2;
	}
	return ret;
failed2:
    stop_ad_convert(ad7124_handler, &pressure_param1);
failed:
    set_gpio_value(GPIO4, 0);
    return -1;
}

/*
 * 获得压力值
 * d: 用于返回压力值
 * pk: 压力参数pk
 * pb: 压力参数pb
 * return: 成功SUCCESS
 */
int pressure_mesure(double pk, double pb, double *d)
{
	int i;
	double voltage1 = 0;
	double voltage2 = 0;
	int n1 = 0;
	int n2 = 0;
	int n = 0;
	ad_value_t vals[N_CODE*2];
	n = get_ad_nval(ad7124_handler, vals, N_CODE*2);
	if(n < 2){
		INFO("get ad nval failed!\n");
		return -1;
	}
	for(i = 0; i < n; i++){
		if(vals[i].ch_num == pressure_param1.ad_channel){
			convert_code2voltage(&vals[i], &pressure_param1);
			voltage1 += vals[i].voltage;
			n1++;
		}else{
			convert_code2voltage(&vals[i], &pressure_param2);
			voltage2 += vals[i].voltage;
			n2++;
		}
	}
	if(n1 == 0){
		INFO("failed:param1 n1 is 0\n");
		return -1;
	}
	if(n2 == 0){
		INFO("failed:param2 n2 is 0\n");
		return -1;
	}

	voltage1 /= n1;
	voltage2 /= n2;

    voltage1 /= 279*6.1;
    voltage2 /= 279;
    *d = 100;
	//计算温压力
	DEBUG("pressure voltage1: %f voltage2:%f\n", voltage1, voltage2);
	return SUCCESS;
}

/*
 * 转换压力到-10度下的压力
 * PT：当前压力
 * T：当前温度
 * PTm10:输出-10度时的压力
 * return: success
 */
int PT2PTm10(double PT, double T, double *PTm10)
{
	*PTm10 = 263.15 * (PT / (T + 273.15));
	return SUCCESS;
}
/*
 * 停止AD通道
 * return:成功：0，失败：-1。
 */
int pressure_stop()
{
	int ret1 = 0;
	int ret2 = 0;
	int ret3 = 0;

	ret1 = stop_ad_convert(ad7124_handler, &pressure_param1);
	if(ret1 < 0){
		INFO("Stop ad convter pressure_param1 failed!\n");
	}

	ret2 = stop_ad_convert(ad7124_handler, &pressure_param2);
	if(ret2 < 0){
		INFO("Stop ad convter pressure_param2 failed!\n");
	}

	ret3 = set_gpio_value(GPIO4, 0);
	if(ret3 < 0){
		INFO("Stop pressure sensor failed!\n");
	}

	return ret1 | ret2 | ret3;
}


/**********************************************************
 * 输入输出阻抗测量
 **********************************************************/
void in_out_stop_all_gpio(void)
{

	set_gpio_value(GPIO0, 0);
	set_gpio_value(GPIO1, 0);
	set_gpio_value(GPIO4, 0);
	set_gpio_value(GPIO5, 0);
}

/*
 *开始输入输出阻抗测量
 */
int in_out_resis_start(void)
{
	int ret = 0;
	ret = set_gpio_value(GPIO5, 1);
	if(ret < 0){
		INFO("GPIO5,inout resis start failed.\n");
		goto failed1;
	}

	ret = set_gpio_value(GPIO1, 1);
	if(ret < 0){
		INFO("GPIO1,inout resis start failed.\n");
		goto failed1;
	}
	usleep(20000);

	ret = set_gpio_value(GPIO0, 1);
	if(ret < 0){
		INFO("GPIO0,inout resis start failed.\n");
		goto failed1;
	}
	usleep(100000);

	ret =  start_ad_convert(ad7124_handler, &in_out_resis_param);
	if(ret < 0){
		INFO("start ad,inout resis start failed.\n");
		goto failed1;
	}
	DEBUG("In out resistance start successfully.\n");
	return 0;
failed1:
	in_out_stop_all_gpio();
	return -1;
}

/*
 *测量输入输出阻抗
 * u1:端电压
 * in_resis:输入阻抗
 * out_resis:输出阻抗
 * return:成功：0，失败：-1
 */
int in_out_resis_mesure(double u1, double *in_resis, double *out_resis)
{
	int i, n;
	int ret;
	double voltage_in = 0;
	double voltage_out = 0;
	ad_value_t vals[N_CODE];

	n = get_ad_nval(ad7124_handler, vals, N_CODE);
	for(i = 0; i < n; i++)
		convert_code2voltage(&vals[i], &in_out_resis_param);

	ret = average_voltage(vals, n, &voltage_in);	
	if(ret != SUCCESS){
		INFO("agerage voltage failed!\n");
		return -1;
	}

	ret = set_gpio_value(GPIO4, 1);	
	if(ret < 0){
		INFO("Failed set gpio4 to 1.\n");
		return -1;
	}

	n = get_ad_nval(ad7124_handler, vals, N_CODE);
	for(i = 0; i < n; i++)
		convert_code2voltage(&vals[i], &in_out_resis_param);

	ret = average_voltage(vals, n, &voltage_out);	
	if(ret != SUCCESS){
		INFO("agerage voltage failed!\n");
		return -1;
	}
	DEBUG("InVoltage:%e, OutVoltage:%e\n", voltage_in, voltage_out);
	ret = set_gpio_value(GPIO4, 0);	
	if(ret < 0){
		INFO("Failed set gpio4 to 0.\n");
		return -1;
	}
	if(check_double_zero(voltage_in)){
		INFO("failed:input resis:volatge is 0.\n");
		return -1;
	}

	if(check_double_zero(voltage_out)){
		INFO("failed:output resis:volatge is 0.\n");
		return -1;
	}

	*in_resis = u1 / voltage_in - 1.5;
	*out_resis = u1 / voltage_out - 1.5;
	return 0;
}
/*
 *停止输入输出电阻测量
 */
int in_out_resis_stop(void)
{
	in_out_stop_all_gpio();
	DEBUG("stop in out messure.\n");
	return stop_ad_convert(ad7124_handler, &in_out_resis_param);
}

/***************************************************************
 * 自检
 ***************************************************************/
/*
 * 自检开始
 * return: 成功返回0，失败返回-1
 */
int self_test_start()
{
    int ret = 0;

    ret = set_gpio_value(GPIO0, 1);
    if(ret < 0){
        INFO("GPIO0,self_test start failed.\n");
        goto failed1;
    }
    usleep(100000);

    ret =  start_ad_convert(ad7124_handler, &in_out_resis_param);
    if(ret < 0){
        INFO("start ad,self_test start failed.\n");
        goto failed1;
    }
    DEBUG("self_test start successfully.\n");
    return 0;
failed1:
    set_gpio_value(GPIO0, 0);
    return -1;
}

/*
 * 获得自检电压
 * U0,返回自检电压
 * return: 成功返回0，失败返回-1
 */
int self_test_messure(double *U0)
{
    int i, n;
    int ret;
    double voltage = 0;
    ad_value_t vals[N_CODE];

    n = get_ad_nval(ad7124_handler, vals, N_CODE);
    for(i = 0; i < n; i++)
        convert_code2voltage(&vals[i], &in_out_resis_param);

    ret = average_voltage(vals, n, &voltage);
    if(ret != SUCCESS){
        INFO("agerage voltage failed!\n");
        return -1;
    }
    *U0 = voltage *2;
    DEBUG("voltage:%e voltage*2:%e\n", voltage, *U0);
    return 0;
}

/*
 * 自检停止
 * return: 成功返回0，失败返回-1
 */
int self_test_stop()
{
    DEBUG("self_test stop...\n");
    set_gpio_value(GPIO0, 0);
    return stop_ad_convert(ad7124_handler, &in_out_resis_param);
}

/**************************************************************************************
 * 零点电压
 **************************************************************************************/
/*
 * 启动AD通道
 * return:成功：0，失败：-1。
 */
int zero_voltage_start()
{
    DEBUG("Zero voltage start...\n");
    int ret = 0;
    ret = set_gpio_value(GPIO0, 1);
    if(ret < 0){
        INFO("start zero voltage failed.\n");
        return -1;
    }
    usleep(100000);
    ret =  start_ad_convert(ad7124_handler, &pressure_param2);
    if(ret < 0){
        INFO("start ad  zero voltage param1 failed!\n");
        goto failed;
    }
    return ret;
failed:
    set_gpio_value(GPIO0, 0);
}

/*
 * 获得零点电压
 * d: 用于返回零点电压
 * return: 成功SUCCESS
 */
int zero_voltage_messure(double *d)
{
    int i, n;
    int ret;
    double voltage = 0;
    ad_value_t vals[N_CODE];

    n = get_ad_nval(ad7124_handler, vals, N_CODE);
    for(i = 0; i < n; i++)
        convert_code2voltage(&vals[i], &pressure_param2);

    ret = average_voltage(vals, n, &voltage);
    if(ret != SUCCESS){
        INFO("average voltage failed!\n");
        return -1;
    }
    *d = voltage/(279*6.1);
    DEBUG("voltage:%e\n", *d);
    return SUCCESS;
}

/*
 * 自检零点电压测量
 * return: 成功返回0，失败返回-1
 */
int zero_voltage_stop()
{
    DEBUG("Zero voltage stop...\n");
    set_gpio_value(GPIO0, 0);
    return stop_ad_convert(ad7124_handler, &pressure_param2);
}
