#include <unistd.h>

// exec函数族 
int main(int argc, char *argv[])
{
	int fd[2];
	pipe(fd);
	
	pid_t pid = fork();
	if(pid < 0)
	 {
		perror("fork");
		_exit(-1);
	 }
	 if(pid == 0) // 子进程
	 {	
		// 由于grep仅从输入设备0中读取信息
		// 所以需要将文件描述符1重定向到fd[0],使grep从无名管道中读取结果
		dup2(fd[0], 0);
		
		// 执行grep命令
		execlp("grep", "grep", "ps", NULL);	
	 }
	 else if (pid > 0) // 父进程,返回的pid是子进程的id
	 {
		// 由于ps仅向输出设备1中写结果
		// 所以需要将文件描述符1重定向到fd[1],使ps向无名管道中输出结果
		dup2(fd[1], 1);
		
		// 执行ps命令
		execlp("ps","ps", "-elf", NULL);
	 }
	
	return 0;
}