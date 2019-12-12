#include <unistd.h>

// exec������ 
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
	 if(pid == 0) // �ӽ���
	 {	
		// ����grep���������豸0�ж�ȡ��Ϣ
		// ������Ҫ���ļ�������1�ض���fd[0],ʹgrep�������ܵ��ж�ȡ���
		dup2(fd[0], 0);
		
		// ִ��grep����
		execlp("grep", "grep", "ps", NULL);	
	 }
	 else if (pid > 0) // ������,���ص�pid���ӽ��̵�id
	 {
		// ����ps��������豸1��д���
		// ������Ҫ���ļ�������1�ض���fd[1],ʹps�������ܵ���������
		dup2(fd[1], 1);
		
		// ִ��ps����
		execlp("ps","ps", "-elf", NULL);
	 }
	
	return 0;
}