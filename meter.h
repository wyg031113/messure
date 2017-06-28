/*************************************************************************
    > File Name: meter.h
    > Author: wyg
    > Mail: wyg_0802@126.com 
    > Created Time: 2017年06月19日 星期一 14时14分30秒
 ************************************************************************/
#ifndef __METER_H__
#define __METER_H__

#ifdef __cplusplus

extern "C" {

#endif

typedef struct ad_value
{
	int ch_num;
	double voltage;
	int code;
}ad_value_t;

typedef struct temperature_tab
{
	double res;
	double temp;
}temperature_tab_t;

#define SNO_SIZE 16
struct EEPROM_DATA
{
    char sno[SNO_SIZE];
    double pk;
    double pb;
}__attribute__((packed));
typedef struct EEPROM_DATA EEPROM_DATA_t;

/*
 *初始化AD7124 打开SPI，关闭默认已经开启的通道0
 */
int setup_ad7124(const char *spidev);

/*
 * 配置参考电压
 * ref_low  负参考电压
 * ref_high 正参考电压
 */
void config_ref_voltage(double ref_low, double ref_high);

/*
 * GPIO上电初始化。GPIO0～GPIO4 输出0；
 * GPIO5输出1.
 * 陪则成功返回 SUCCESS
 */
int gpio_init(void);

/*
 *初始化AD7124 打开SPI，关闭默认已经开启单通道0
 */
int setup_ad7124(const char *spidev);


/***************************************************************************
 *绝缘电阻测量
 ***************************************************************************/
/*
 * 启动AD通道
 */
int resistance_start();

/*
 *获得绝缘电阻值
 * d: 用于返回电阻值
 * return: 成功SUCCESS
 */
int resistance_mesure(double *d);


/*
 *停止AD通道
 */
int resistance_stop();


/*****************************************************************************
 * EEPROM 操作
 *****************************************************************************/
/*
 * 配置GPIO，启动EEPROM
 */
int start_eeprom(void);

/*
 * 配置GPIO，停止EEPROM
 */
int stop_eeprom(void);

/*
 * 读EEPROM
 * fname: eeprom文件名
 * buf: 接收数据
 * buf_len: buf大小
 * offset: eeprom偏移
 * return: 返回写了多少字节
 */
int read_eeprom(const char *fname, char *buf, size_t buf_len, size_t offset);

/*
 * 写EEPROM
 * fname: eeprom文件名
 * buf: 要写的数据
 * buf_len: 数据长度
 * offset: eeprom偏移
 * return: 返回写了多少字节
 */
int write_eeprom(const char *fname, const char *buf, size_t buf_len, size_t offset);

/*
 * 从EEPROM中读取发动机编号、压力参数pk,pb.
 * fname:EEPROM文件名
 * val:输出读取的值
 * return:成功0，失败-1.
 */
int read_eeprom_data(const char *fname, EEPROM_DATA_t *val);

/*
 * 写入EEPROM发动机编号、压力参数pk,pb.
 * fname:EEPROM文件名
 * val:要写入的值
 * return:成功0，失败-1.
 */
int write_eeprom_data(const char *fname, EEPROM_DATA_t *val);
/**************************************************************************************
 * 温度测量
 **************************************************************************************/
/*
 * 获取温度对照表
 */
temperature_tab_t *get_temp_tab();

/*
 * 获取温度对照表大小
 */
int get_ntemp();

/**
 * 加载温度对照表
 * filename: 温度对照表所在文件。
 * return:成功：0，失败：-1。
 */
int load_temp(const char *filename);

/*
 * 启动AD通道
 */
int temperature_start();

/*
 * 获得温度值
 * v1 电压3.13
 * tabs:温度对照表
 * n:温度对照表大小
 * d: 用于返回温度值
 * return: 成功SUCCESS
 */
int temperature_mesure(double v1, temperature_tab_t *tabs, int n, double *d);

/*
 *停止AD通道
 */
int temperature_stop();

/**************************************************************************************
 * 压力测量
 **************************************************************************************/
/*
 * 启动AD通道
 */
int pressure_start();

/*
 * 获得压力值
 * d: 用于返回压力值
 * return: 成功SUCCESS
 */
int pressure_mesure(double pk, double pb, double *d);

/*
 *停止AD通道
 */
int pressure_stop();

/*
 * 转换压力到-10度下的压力
 * PT：当前压力
 * T：当前温度
 * PTm10:输出-10度时的压力
 * return: success
 */
int PT2PTm10(double PT, double T, double *PTm10);

/**********************************************************
 * 输入输出阻抗测量
 **********************************************************/

/*
 *开始输入输出阻抗测量
 */
int in_out_resis_start(void);
/*
 *停止输入输出电阻测量
 */
int in_out_resis_stop(void);


/*
 *测量输入输出阻抗
 * u1:端电压
 * in_resis:输入阻抗
 * out_resis:输出阻抗
 * return:成功：0，失败：-1
 */
int in_out_resis_mesure(double u1, double *in_resis, double *out_resis);

/***************************************************************
 * 自检
 ***************************************************************/
/*
 * 自检开始
 * return: 成功返回0，失败返回-1
 */
int self_test_start();


/*
 * 获得自检电压
 * U0,返回自检电压
 * return: 成功返回0，失败返回-1
 */
int self_test_messure(double *U0);

/*
 * 自检停止
 * return: 成功返回0，失败返回-1
 */
int self_test_stop();


/**************************************************************************************
 * 零点电压
 **************************************************************************************/
/*
 * 启动AD通道
 * return:成功：0，失败：-1。
 */
int zero_voltage_start();

/*
 * 获得零点电压
 * d: 用于返回零点电压
 * return: 成功SUCCESS
 */
int zero_voltage_messure(double *d);

/*
 * 自检零点电压测量
 * return: 成功返回0，失败返回-1
 */
int zero_voltage_stop();

#ifdef __cplusplus

}

#endif

#endif /*_METER_H__*/
