/*************************************************************************
    > File Name: gpio_ctl.c
    > Author: wyg
    > Mail: wyg_0802@126.com 
    > Created Time: 2017年06月18日 星期日 18时43分55秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "gpio_ctl.h"

static int gpios[GPIO_NR] = {
	80,	81, 82, 83,   193, 192, 178, 177,
    200, 33, 34, 35,  36, 37, 38, 39,
    95
};

int write_cmd(const char *file, const char *cmd)
{
	int ret = 0;
	int fd = 0;
	fd = open(file, O_WRONLY);
	if(fd < 0){
		fprintf(stderr, "open file %s failed:%s\n", file, strerror(errno));
		return -1;
	}
	ret = write(fd, cmd, strlen(cmd));
	if(ret != strlen(cmd)){
		fprintf(stderr, "write cmd: %s failed:%s\n", cmd, strerror(errno));
		ret = -1;
	}
	close(fd);
	return ret;
}
int set_gpio_value(int gpio, int value)
{
	char cmd_add_gpio[16];

	char cmd_dir_file[128];
	
	char cmd_set_val[16];
	char cmd_value_file[128];

	int gpio_imx = -1;
	if(gpio < 0 || gpio >= GPIO_NR){
		fprintf(stderr,"GPIO %d out of range!\n", gpio);
		return -1;
	}
	gpio_imx = gpios[gpio];
	snprintf(cmd_add_gpio, 16, "%d", gpio_imx);	

	snprintf(cmd_dir_file, 128, "/sys/class/gpio/gpio%d/direction", gpio_imx);	
	snprintf(cmd_set_val, 16, "%d", value);
	snprintf(cmd_value_file, 128, "/sys/class/gpio/gpio%d/value", gpio_imx);
	if(access(cmd_value_file, F_OK) < 0)
		if(write_cmd("/sys/class/gpio/export", cmd_add_gpio) < 0){
			fprintf(stderr, "export gpio %d failed\n", gpio);
			return - 1;
		}
	if(write_cmd(cmd_dir_file, "out") > 0){
		if(write_cmd(cmd_value_file, cmd_set_val) > 0){
			return 0;
		}

	}
	fprintf(stderr,"set gpio %d to value %d failed!\n", gpio, value);
	return -1;
}
