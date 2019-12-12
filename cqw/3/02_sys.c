#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
	int num = 10;
	int fd[2];
	pipe(fd);
	
	pid_t pid = fork();
	
	if(pid < 0) {
		perror("fork");
		_exit(-1);
	}
	
	if(pid == 0) {
		printf("子进程：\n");
		char buffer[128] = "";
		read(fd[0], buffer, sizeof(buffer));
		printf("子进程：%s\n", buffer);
	}
	else if(pid > 0) {
		sleep(5);
		write(fd[1], "hello", strlen("hello"));
		printf("父进程：\n");
	}

	
	
	
	return 0;
}