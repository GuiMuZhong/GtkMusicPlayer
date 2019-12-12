#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	//mkfifo("1607094249", 0777);
	
	int fifd = open("1607094249", O_WRONLY);
	
	printf("5s后将发送消息\n");
	sleep(5);
	
	write(fifd, "hello", strlen("hello"));

	close(fifd);
	
	
	return 0;
}