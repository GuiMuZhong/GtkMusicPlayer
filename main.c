#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "uart.h"

int fd_ui, fd[2];
int flag = 0, flag_start = 0;
GtkWidget *label_music_name;
GtkWidget *progress_voice;
GtkWidget *progress_speed;
GtkWidget *btn_start;
int idx_music = 0;
char *music_list[20] = {"./music/1.mp3", "./music/2.mp3", "./music/3.mp3", "./music/4.mp3", "./music/5.mp3"};

void deal_quit(GtkWidget *win, gpointer user_data);
void user_ui(int argc, char *argv[], int kid_pid);
void deal_button(GtkWidget *button, gpointer user_data);
void music_player();
void* progress_speed_fun1(void * arg);
void* progress_speed_fun2(void * arg);
void chang_background(GtkWidget *widget, int w, int h, const gchar *path);
void load_image(GtkWidget *image, const char *file_path, const int w, const int h );

int main(int argc, char *argv[])
{
	mkfifo("fifo_music_cmd", 0777);

	pipe(fd);
	pid_t pid = fork();
	
	if(pid < 0) {
		perror("启动失败");
		_exit(-1);
	}
	
	if(pid > 0) {
		user_ui(argc, argv, pid);

	}
	else {
		music_player();
	}
	
	return 0;
}

void* progress_speed_fun1(void * arg)
{
	while(1) {
		while(!flag);
		write(fd_ui, "get_percent_pos\n", strlen("get_percent_pos\n"));
		sleep(1);
	}
	
}

void* progress_speed_fun2(void * arg)
{
	while(1) {
		char buffer[128] = "";
		read(fd[0], buffer, sizeof(buffer));
		if(strlen(buffer) > 21) {
			if(buffer[0] == 'A' && buffer[19] == 'N') {		// ANS_PERCENT_POSITION=1
				int pbar = 0;
				for(int i = 21; buffer[i] >= '0' && buffer[i] <= '9'; i++) {
					pbar = pbar * 10 + buffer[i] - '0';
				}
				gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_speed), pbar / 100.0);
			}
		}
	}
	
}

void* voice_control_fun(void *arg)
{
	int fd1 = uart_init("/dev/ttyUSB0");
	while(1) {
		char text[128] = "";
		uart_readline(fd1, text, sizeof(text), 10000);
		printf("cqw *** I am the voice_control_fun\n");
		if(text[0] != 0) {
			printf("cqw >>> %s\n", text);
			if(text[0] == 'z') {
				if(flag == 1) {
					write(fd_ui, "pause\n", strlen("pause\n"));
					flag = 0;
				}
			}
			else if(text[0] == 'b') {
				if(flag_start == 0) {
					write(fd_ui, "loadfile ./music/1.mp3\n", strlen("loadfile ./music/1.mp3\n"));
					gtk_button_set_label(GTK_BUTTON(btn_start), "Pause");
					gtk_label_set_text(GTK_LABEL(label_music_name), "1.mp3");
					flag = 1;
					flag_start = 1;
				}
				else if(flag == 0) {
					write(fd_ui, "pause\n", strlen("pause\n"));
					flag = 1;
				}
			}
			else if(text[0] == 's') {
				idx_music = (idx_music - 1 + 5) % 5;
				char str_cmd[50];
				sprintf(str_cmd, "loadfile %s\n", music_list[idx_music]);
				write(fd_ui, str_cmd, strlen(str_cmd));

				gtk_label_set_text(GTK_LABEL(label_music_name), music_list[idx_music] + 8);

				flag = 1;
			}
			else if(text[0] == 'x') {
				idx_music = (idx_music + 1) % 5;
				char str_cmd[50];
				sprintf(str_cmd, "loadfile %s\n", music_list[idx_music]);
				write(fd_ui, str_cmd, strlen(str_cmd));

				gtk_label_set_text(GTK_LABEL(label_music_name), music_list[idx_music] + 8);

				flag = 1;
			}
		}

	}
}

void user_ui(int argc, char *argv[], int kid_pid)
{
	fd_ui = open("fifo_music_cmd", O_WRONLY);
	write(fd_ui, "volume 50 1\n", strlen("volume 50 1\n"));

	char str_kid_ped[10];
	sprintf(str_kid_ped, "%d", kid_pid);

	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "CQW-Music");
	gtk_window_set_default_size(GTK_WINDOW(win), 620, 520);	
	g_signal_connect(win, "destroy", G_CALLBACK(deal_quit), str_kid_ped);
	chang_background(win, 620, 520, "./image/1.jpg");

	// 网格布局
	GtkWidget *table = gtk_table_new(20, 3, TRUE);
	gtk_container_add(GTK_CONTAINER(win), table);

	// 显示
	label_music_name = gtk_label_new("");
	gtk_table_attach_defaults(GTK_TABLE(table), label_music_name, 0, 3, 0, 16);

	// 进度条
	progress_speed = gtk_progress_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table), progress_speed, 0, 3, 16, 17);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_speed), 0);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_speed), "SPEED");

	// 声音
	progress_voice = gtk_progress_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table), progress_voice, 0, 3, 17, 18);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), 0.5);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_voice), "VOICE");
	GtkWidget *btn_voice = gtk_button_new_with_label("Mute");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_voice, 1, 2, 18, 19);
	GtkWidget *btn_up = gtk_button_new_with_label("Up");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_up, 2, 3, 18, 19);
	GtkWidget *btn_down = gtk_button_new_with_label("Down");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_down, 0, 1, 18, 19);

	// 上一首 开始 下一首
	GtkWidget *btn_last = gtk_button_new_with_label("Last");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_last, 0, 1, 19, 20);
	GtkWidget *btn_start = gtk_button_new_with_label("Start");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_start, 1, 2, 19, 20);
	GtkWidget *btn_next = gtk_button_new_with_label("Next");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_next, 2, 3, 19, 20);

	// 进度条询问进程
	pthread_t tid_progress_speed1;
	pthread_create(&tid_progress_speed1, NULL, progress_speed_fun1, NULL);
	pthread_detach(tid_progress_speed1);

	// 进度条修改进程
	pthread_t tid_progress_speed2;
	pthread_create(&tid_progress_speed2, NULL, progress_speed_fun2, NULL);
	pthread_detach(tid_progress_speed2);

	// 语音进程
	// pthread_t tid_voice;
	// pthread_create(&tid_voice, NULL, voice_control_fun, NULL);
	// pthread_detach(tid_voice);

	g_signal_connect(btn_voice, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_last, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_start, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_next, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_up, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_down, "clicked", G_CALLBACK(deal_button), NULL);
	
	gtk_widget_show_all(win);

	gtk_main();
}

void deal_quit(GtkWidget *win, gpointer user_data)
{
	char kill_cmd[50];
	sprintf(kill_cmd, "kill -9 %s", (char *)user_data);
	system(kill_cmd);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void deal_button(GtkWidget *button, gpointer user_data)
{
	fd_ui = open("fifo_music_cmd", O_WRONLY);

	char *strbtn = (char *)gtk_button_get_label(GTK_BUTTON(button));

	if(strbtn[0] == 'M') {
		gtk_button_set_label(GTK_BUTTON(button), "Voice");
		write(fd_ui, "mute 1\n", strlen("mute 1\n"));
		if(flag == 0) {
			write(fd_ui, "pause\n", strlen("pause\n"));
		}
	}
	else if(strbtn[0] == 'V') {
		gtk_button_set_label(GTK_BUTTON(button), "Mute");
		write(fd_ui, "mute 0\n", strlen("mute 0\n"));
		if(flag == 0) {
			write(fd_ui, "pause\n", strlen("pause\n"));
		}
	}
	else if(strbtn[0] == 'S') {
		write(fd_ui, "loadfile ./music/1.mp3\n", strlen("loadfile ./music/1.mp3\n"));
		gtk_button_set_label(GTK_BUTTON(button), "Pause");
		gtk_label_set_text(GTK_LABEL(label_music_name), "1.mp3");
		flag = 1;
		flag_start = 1;
	}
	else if(strbtn[0] == 'P') {
		write(fd_ui, "pause\n", strlen("pause\n"));
		flag = !flag;
	}
	else if(strbtn[0] == 'L') {
		idx_music = (idx_music - 1 + 5) % 5;
		char str_cmd[50];
		sprintf(str_cmd, "loadfile %s\n", music_list[idx_music]);
		write(fd_ui, str_cmd, strlen(str_cmd));

		gtk_label_set_text(GTK_LABEL(label_music_name), music_list[idx_music] + 8);

		flag = 1;
	}
	else if(strbtn[0] == 'N') {
		idx_music = (idx_music + 1) % 5;
		char str_cmd[50];
		sprintf(str_cmd, "loadfile %s\n", music_list[idx_music]);
		write(fd_ui, str_cmd, strlen(str_cmd));

		gtk_label_set_text(GTK_LABEL(label_music_name), music_list[idx_music] + 8);

		flag = 1;
	}
	else if(strbtn[0] == 'U') {
		double voice = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_voice));
		int du = voice * 100 + 5;
		if(du > 100) du = 10;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), du / 100.0 );
		char str_cmd[50];
		sprintf(str_cmd, "volume %d 1\n", du);
		write(fd_ui, str_cmd, strlen(str_cmd));

		if(flag == 0) {
			write(fd_ui, "pause\n", strlen("pause\n"));
		}
	}
	else if(strbtn[0] == 'D') {
		double voice = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_voice));
		int du = voice * 100 - 5;
		if(du < 0) du = 0;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), du / 100.0 );
		char str_cmd[50];
		sprintf(str_cmd, "volume %d 1\n", du);
		write(fd_ui, str_cmd, strlen(str_cmd));

		if(flag == 0) {
			write(fd_ui, "pause\n", strlen("pause\n"));
		}
	}
}

void music_player()
{
	dup2(fd[1], 1);
	execlp("mplayer", 
				"mplayer", 
				"-slave", "-quiet", "-idle", 
				"-input", 
				"file=./fifo_music_cmd", NULL);
}

void chang_background(GtkWidget *widget, int w, int h, const gchar *path)
{
	gtk_widget_set_app_paintable(widget, TRUE); //允许窗口可以绘图
	gtk_widget_realize(widget);
	/* 更改背景图时，图片会重叠
	* 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。
	*/
	gtk_widget_queue_draw(widget);
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(path, NULL); // 创建图片资源对象
	// w, h是指定图片的宽度和高度
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h,GDK_INTERP_BILINEAR);
	GdkPixmap *pixmap = NULL;
	/* 创建pixmap图像;
	步骤3：设置按钮图片（函数直接调用load_image）
	参数介绍：
	* NULL：不需要蒙版;
	* 123： 0~255，透明到不透明
	*/
	gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 128);
	// 通过pixmap给widget设置一张背景图，最后一个参数必须为: FASLE
	gdk_window_set_back_pixmap(widget->window, pixmap, FALSE);
	// 释放资源
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	g_object_unref(pixmap);
}

void load_image(GtkWidget *image, const char *file_path, const int w, const int h )
{
	gtk_image_clear( GTK_IMAGE(image) ); // 清除图像
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(file_path, NULL); // 创建图片资源
	GdkPixbuf *dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR); // 指定大小
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dest_pixbuf); // 图片控件重新设置一张图片(pixbuf)
	g_object_unref(src_pixbuf); // 释放资源
	g_object_unref(dest_pixbuf); // 释放资源
}