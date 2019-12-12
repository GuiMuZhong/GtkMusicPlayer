#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

GtkWidget *label_music_name;

void user_ui(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	
	pid_t pid = fork();
	
	if(pid < 0) {
		perror("启动失败");
		_exit(-1);
	}
	
	if(pid > 0) {
		user_ui(argc, argv);
	}
	else {
		//execlp("mplayer", "mplayer", "-slave", "-quiet", "twotigers.mp3", NULL);
	}
	
	return 0;
}

void user_ui(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "CQW-Music");
	gtk_window_set_default_size(GTK_WINDOW(win), 520, 520);	
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	// 网格布局
	GtkWidget *table = gtk_table_new(4, 3, TRUE);
	gtk_container_add(GTK_CONTAINER(win), table);

	// 显示
	label_music_name = gtk_label_new("两只老虎");
	gtk_table_attach_defaults(GTK_TABLE(table), label_music_name, 0, 3, 0, 1);

	// 进度条
	GtkWidget *progress_speed = gtk_progress_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table), progress_speed, 0, 3, 1, 2);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_speed), 0.5);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_speed), "SPEED");

	// 声音
	GtkWidget *progress_voice = gtk_progress_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table), progress_voice, 0, 2, 2, 3);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_voice), 0.5);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_voice), "VOICE");
	GtkWidget *btn_voice = gtk_button_new_with_label("Mute");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_voice, 2, 3, 2, 3);

	// 上一首 开始 下一首
	GtkWidget *btn_last = gtk_button_new_with_label("Last");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_last, 0, 1, 3, 4);
	GtkWidget *btn_start = gtk_button_new_with_label("Start");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_start, 1, 2, 3, 4);
	GtkWidget *btn_next = gtk_button_new_with_label("Next");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_next, 2, 3, 3, 4);
	
	gtk_widget_show_all(win);

	gtk_main();
}