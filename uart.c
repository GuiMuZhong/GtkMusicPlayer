#include "uart.h"

static struct termios option_old;

/*************************************************************
* 功能：	串口初始化程序
* 参数：	串口设备文件序号
* 返回值：	串口设备文件描述符
**************************************************************/
int uart_init(char *devname)
{
	int uart_fd;
	struct termios option_new;
	
	uart_fd = open(devname, O_RDWR);
	if(uart_fd < 0)
	{
		perror("open_dev");
		_exit(-1);
	}
	tcgetattr(uart_fd, &option_old);		//保存串口属性
	tcgetattr(uart_fd, &option_new);

	cfsetispeed(&option_new, B9600);		//波特率为9600
	cfsetospeed(&option_new, B9600);		//波特率为9600
	option_new.c_cflag &= ~CSIZE;			//设置数据位时先关闭历史设置
	option_new.c_cflag |= CS8;				//数据位为8位
	option_new.c_cflag &= ~CSTOPB;			//1位停止位
	option_new.c_cflag &= ~PARENB;			//无奇偶校验位
	option_new.c_lflag &= ~(ICANON);		//非标准模式
	option_new.c_lflag &= ~ECHO;			//关回显，在使用GPRS模组时需关回显
	//option_new.c_lflag |= ECHO;				//开回显
	tcsetattr(uart_fd, TCSANOW, &option_new);
	return uart_fd;
}

/*************************************************************
* 功能：	串口反初始化程序
* 参数：	串口设备文件描述符
* 返回值：	无
**************************************************************/
void uart_uninit(int uart_fd)
{
	/*还原串口属性*/
	tcsetattr(uart_fd, TCSANOW, &option_old);
	
	/*关闭串口*/
	close(uart_fd);
}

/*************************************************************
* 功能：	串口发送字符串
* 参数：	uart_fd：串口设备文件描述符
			str：待发送的字符
* 返回值：	无
**************************************************************/
void uart_send_str(int uart_fd, char *str)
{
	int ret;
	
	ret = write(uart_fd, str, strlen(str));
	if(ret < 0)
	{
		perror("write");
	}
}

/*************************************************************
* 功能：	串口读一行字符串，在设定的时间内读不到数据则函数返回
* 参数：	uart_fd：串口设备文件描述符
			buffer：存放数据的内存的首地址
			len：存放数据的内存空间的大小
			timeout_ms：超时时间(单位：ms)
* 返回值：	
			成功：实际读到的字符数
			失败：-1
**************************************************************/
int uart_readline(int uart_fd, char *buffer, int len, int timeout_ms)
{
	char c = '\0';
	fd_set fds;
	struct timeval tv;
	int i;
	int ret;

	memset(buffer, 0, len);
	for(i=0; i<len && c != '\n'; i++){
		tv.tv_sec = 0;
		tv.tv_usec = timeout_ms*1000;
		FD_ZERO(&fds);
		FD_SET(uart_fd, &fds);
		ret = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
		if(ret < 0){
			perror("seclect");
			return -1;
		}else if(ret > 0){
			ret = read(uart_fd, &c, 1);
			if(ret < 0)
			{
				perror("read");
			}
		}else{
			return -1;
		}
		*buffer++ = c;
		//printf("c=%c\n", c);
	}
	return i;
}
