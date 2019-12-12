#include <gtk/gtk.h>

void deal_button(GtkButton *btn, gpointer data);

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "window");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);	
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *button = gtk_button_new_with_label("start");
	gtk_container_add(GTK_CONTAINER(window), button);
	g_signal_connect(button, "clicked", G_CALLBACK(deal_button), "user_data");
	
	gtk_widget_show_all(window);

	gtk_main();
	

	return 0;
}

void deal_button(GtkButton *btn, gpointer data)
{
	printf("%s\n", (char *)data);
	const gchar *text = gtk_button_get_label(btn);
	printf("%s\n", text);
	gtk_button_set_label(btn, "CQW");
}