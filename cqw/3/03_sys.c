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
	
	int fifd = open("1607094249", O_RDONLY);
	
	char buf[128] = "";
	
	read(fifd, buf, sizeof(buf));
	
	printf(">>>> %s\n", buf);

	close(fifd);
	
	
	return 0;
}