/*************************************************************************
    > File Name: gpio_ctl.h
    > Author: wyg
    > Mail: wyg_0802@126.com 
    > Created Time: 2017年06月18日 星期日 19时05分54秒
 ************************************************************************/
#ifndef __GPIO_CTL_H__
#define __GPIO_CTL_H__

#ifdef __cplusplus

extern "C" {

#endif

#define GPIO_NR 16
/* GPIO  bank NR IMX_GPIO_NR(x,y) == 32*(x-1)+y
 * GPIO0 3 16	80
 * GPIO1 3 17	81
 * GPIO2 3 18	82
 * GPIO3 3 19	83
 *
 * GPIO4 7 1	193
 * GPIO5 7 0	192
 * GPIO6 6 18	178
 * GPIO7 6 17  177
 * GPIO8 7 8   200
 * GPIO9 2 1	33
 * GPIO10 2 2  34
 *
 * GPIO11 2 3  35
 * GPIO12 2 4	36
 * GPIO13 2 5	37
 * GPIO14 2 6	38
 * GPIO15 2 7	39
 */
#define GPIO0	0
#define GPIO1	1
#define GPIO2	2
#define GPIO3	3
#define GPIO4	4
#define GPIO5	5
#define GPIO6	6
#define GPIO7	7
#define GPIO8	8
#define GPIO9	9
#define GPIO10	10
#define GPIO11	11
#define GPIO12	12
#define GPIO13	13
#define GPIO14	14
#define GPIO15	15
int set_gpio_value(int gpio, int value);

#ifdef __cplusplus

}

#endif

#endif
