#include <gtk/gtk.h>

typedef struct{
	GtkWidget *label_cqw1;
	GtkWidget *label_vs1;
	GtkWidget *label_lsw1;
	GtkWidget *btn0;
	GtkWidget *btn1;
	GtkWidget *btn2;
}MYWID;

void deal_button(GtkWidget *btn, gpointer user_data);

int main(int argc, char *argv[])
{
	srand(time(NULL));

	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "window");
	gtk_window_set_default_size(GTK_WINDOW(win), 520, 520);	
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *table = gtk_table_new(4, 3, TRUE);
	gtk_container_add(GTK_CONTAINER(win), table);
	
	GtkWidget *title_label = gtk_label_new("石头剪刀布");
	gtk_table_attach_defaults(GTK_TABLE(table), title_label, 0, 3, 0, 1);
	
	GtkWidget *label1_cqw = gtk_label_new("CQW");
	gtk_table_attach_defaults(GTK_TABLE(table), label1_cqw, 0, 1, 1, 2);
	GtkWidget *label_vs = gtk_label_new("VS");
	gtk_table_attach_defaults(GTK_TABLE(table), label_vs, 1, 2, 1, 2);
	GtkWidget *label_lsw = gtk_label_new("LSW");
	gtk_table_attach_defaults(GTK_TABLE(table), label_lsw, 2, 3, 1, 2);
	
	MYWID mywid;
	
	mywid.label_cqw1 = gtk_label_new("cqw");
	gtk_table_attach_defaults(GTK_TABLE(table), mywid.label_cqw1, 0, 1, 2, 3);
	mywid.label_vs1 = gtk_label_new("result");
	gtk_table_attach_defaults(GTK_TABLE(table), mywid.label_vs1, 1, 2, 2, 3);
	mywid.label_lsw1 = gtk_label_new("lsw");
	gtk_table_attach_defaults(GTK_TABLE(table), mywid.label_lsw1, 2, 3, 2, 3);
	
	mywid.btn0 = gtk_button_new_with_label("石头");
	gtk_table_attach_defaults(GTK_TABLE(table), mywid.btn0, 0, 1, 3, 4);
	mywid.btn1 = gtk_button_new_with_label("剪刀");
	gtk_table_attach_defaults(GTK_TABLE(table), mywid.btn1, 1, 2, 3, 4);
	mywid.btn2 = gtk_button_new_with_label("布");
	gtk_table_attach_defaults(GTK_TABLE(table), mywid.btn2, 2, 3, 3, 4);
	
	g_signal_connect(mywid.btn0, "clicked", G_CALLBACK(deal_button), &mywid);
	g_signal_connect(mywid.btn1, "clicked", G_CALLBACK(deal_button), &mywid);
	g_signal_connect(mywid.btn2, "clicked", G_CALLBACK(deal_button), &mywid);
	
	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

void deal_button(GtkWidget *button, gpointer user_data)
{
	MYWID *p = (MYWID *)user_data;
	
	int lsw = 0;
	if(button == p->btn0) {
		gtk_label_set_text(GTK_LABEL(p->label_lsw1), "石头");
		lsw = 0;
	}
	else if(button == p->btn1) {
		gtk_label_set_text(GTK_LABEL(p->label_lsw1), "剪刀");
		lsw = 1;
	}
	else if(button == p->btn2) {
		gtk_label_set_text(GTK_LABEL(p->label_lsw1), "布");
		lsw = 2;
	}
	
	int cqw = rand() % 3;
	if(cqw == 0) {
		gtk_label_set_text(GTK_LABEL(p->label_cqw1), "石头");
	}
	else if(cqw == 1) {
		gtk_label_set_text(GTK_LABEL(p->label_cqw1), "剪刀");
	}
	else if(cqw == 2) {
		gtk_label_set_text(GTK_LABEL(p->label_cqw1), "布");
	}
	
	if(cqw == lsw) {
		gtk_label_set_text(GTK_LABEL(p->label_vs1), "=");
	}
	else if((cqw + 1) % 3 == lsw) {
		gtk_label_set_text(GTK_LABEL(p->label_vs1), ">");
	}
	else {
		gtk_label_set_text(GTK_LABEL(p->label_vs1), "<");
	}

}
