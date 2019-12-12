#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int fd_ui;
GtkWidget *label_music_name;
GtkWidget *progress_voice;
int idx_music = 0;
char *music_list[20] = {"./music/1.mp3", "./music/2.mp3", "./music/3.mp3", "./music/4.mp3", "./music/5.mp3"};

void deal_quit(GtkWidget *win, gpointer user_data);
void user_ui(int argc, char *argv[], int kid_pid);
void deal_button(GtkWidget *button, gpointer user_data);
void music_player();

int main(int argc, char *argv[])
{
	mkfifo("fifo_music_cmd", 0777);
	
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

void user_ui(int argc, char *argv[], int kid_pid)
{
	fd_ui = open("fifo_music_cmd", O_WRONLY);
	write(fd_ui, "volume 50 1\n", strlen("volume 50 1\n"));
	write(fd_ui, "pause\n", strlen("pause\n"));
	close(fd_ui);

	char str_kid_ped[10];
	sprintf(str_kid_ped, "%d", kid_pid);

	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "CQW-Music");
	gtk_window_set_default_size(GTK_WINDOW(win), 520, 520);	
	g_signal_connect(win, "destroy", G_CALLBACK(deal_quit), str_kid_ped);
	
	// 网格布局
	GtkWidget *table = gtk_table_new(5, 3, TRUE);
	gtk_container_add(GTK_CONTAINER(win), table);

	// 显示
	label_music_name = gtk_label_new("");
	gtk_table_attach_defaults(GTK_TABLE(table), label_music_name, 0, 3, 0, 1);

	// 进度条
	GtkWidget *progress_speed = gtk_progress_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table), progress_speed, 0, 3, 1, 2);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_speed), 0.5);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_speed), "SPEED");

	// 声音
	progress_voice = gtk_progress_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table), progress_voice, 0, 3, 2, 3);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), 0.5);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_voice), "VOICE");
	GtkWidget *btn_voice = gtk_button_new_with_label("Mute");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_voice, 0, 1, 3, 4);
	GtkWidget *btn_up = gtk_button_new_with_label("Up");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_up, 1, 2, 3, 4);
	GtkWidget *btn_down = gtk_button_new_with_label("Down");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_down, 2, 3, 3, 4);

	// 上一首 开始 下一首
	GtkWidget *btn_last = gtk_button_new_with_label("Last");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_last, 0, 1, 4, 5);
	GtkWidget *btn_start = gtk_button_new_with_label("Start");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_start, 1, 2, 4, 5);
	GtkWidget *btn_next = gtk_button_new_with_label("Next");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_next, 2, 3, 4, 5);

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
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	char kill_cmd[50];
	sprintf(kill_cmd, "kill -9 %s", (char *)user_data);
	system(kill_cmd);
}

void deal_button(GtkWidget *button, gpointer user_data)
{
	fd_ui = open("fifo_music_cmd", O_WRONLY);

	char *strbtn = (char *)gtk_button_get_label(GTK_BUTTON(button));

	if(strbtn[0] == 'M') {
		gtk_button_set_label(GTK_BUTTON(button), "Voice");
		write(fd_ui, "mute 1\n", strlen("mute 1\n"));
	}
	else if(strbtn[0] == 'V') {
		gtk_button_set_label(GTK_BUTTON(button), "Mute");
		write(fd_ui, "mute 0\n", strlen("mute 0\n"));
	}
	else if(strbtn[0] == 'S') {
		write(fd_ui, "loadfile ./music/1.mp3\n", strlen("loadfile ./music/1.mp3\n"));
		gtk_button_set_label(GTK_BUTTON(button), "Pause");
		gtk_label_set_text(GTK_LABEL(label_music_name), "1.mp3");
	}
	else if(strbtn[0] == 'P') {
		write(fd_ui, "pause\n", strlen("pause\n"));
	}
	else if(strbtn[0] == 'L') {
		idx_music = (idx_music - 1 + 5) % 5;
		char str_cmd[50];
		sprintf(str_cmd, "loadfile %s\n", music_list[idx_music]);
		write(fd_ui, str_cmd, strlen(str_cmd));

		gtk_label_set_text(GTK_LABEL(label_music_name), music_list[idx_music] + 8);
	}
	else if(strbtn[0] == 'N') {
		idx_music = (idx_music + 1) % 5;
		char str_cmd[50];
		sprintf(str_cmd, "loadfile %s\n", music_list[idx_music]);
		write(fd_ui, str_cmd, strlen(str_cmd));

		gtk_label_set_text(GTK_LABEL(label_music_name), music_list[idx_music] + 8);
	}
	else if(strbtn[0] == 'U') {
		double voice = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_voice));
		int du = voice * 100 + 5;
		if(du > 100) du = 10;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), du / 100.0 );
		char str_cmd[50];
		sprintf(str_cmd, "volume %d 1\n", du);
		write(fd_ui, str_cmd, strlen(str_cmd));
	}
	else if(strbtn[0] == 'D') {
		double voice = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_voice));
		int du = voice * 100 - 5;
		if(du < 0) du = 0;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), du / 100.0 );
		char str_cmd[50];
		sprintf(str_cmd, "volume %d 1\n", du);
		write(fd_ui, str_cmd, strlen(str_cmd));
	}

	close(fd_ui);
}

void music_player()
{
	execlp("mplayer", 
				"mplayer", 
				"-slave", "-quiet", "-idle", 
				"-input", 
				"file=./fifo_music_cmd", "./music/1.mp3", NULL);
}