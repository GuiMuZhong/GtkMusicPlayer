#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define N 50

void game_help(void);
void create_str(char s[]);
char mygetch(void);
void play_game(char *src_str);

int main(int argc, char *argv[])
{
	while(1) {
		game_help();
		char src_str[N+1] = {0};
		create_str(src_str);
		mygetch();
		printf("%s\n", src_str);
		play_game(src_str);
		while(1) {
			char ch = mygetch();
			if(ch == '') {
				return;
			}
			else if(ch == ' ') {
				break;
			}
		}
		system("clear");
	}

	return 0;
}

void game_help(void)
{
	printf("**************************************\n");
	printf("*输入过程中无法退出                  *\n");
	printf("*按所给的字母敲击键盘                *\n");
	printf("*按下任意字母开始，按下首字母开始计时*\n");
	printf("*输入错误以_表示                     *\n");
	printf("**************************************\n");
	return;
}

void create_str(char s[])
{
	srand(time(NULL));
	int i = 0;
	for(i = 0; i < N; i++) {
		s[i] = rand() % 26 + 'a';
	}
	return;
}

char mygetch(void) 
{
    struct termios oldt, newt;
    char ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

void play_game(char *src_str)
{
	int i = 0;
	int cnt = 0;
	char c = 0;
	int start_time = 0;
	int end_time = 0;
	for(i = 0; i < N; i++) {
		c = mygetch();
		
		if(i == 0) {
			start_time = time(NULL);
		}
		
		if(c == src_str[i]) {
			cnt++;
			putchar(c);
		}
		else {
			putchar('_');
		}
	}
	
	end_time = time(NULL);
	
	printf("\n完成输入");
	
	printf("\n所耗时间为：%ds", end_time - start_time);
	
	printf("\n正确率: %.2f%%\n", (double)cnt / N * 100);
	
	printf("按ESC退出，按空格继续\n");
}