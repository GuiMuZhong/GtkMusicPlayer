#include <gtk/gtk.h>

void deal_button(GtkWidget *button, gpointer user_data);

GtkWidget *entry;

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "window");
	gtk_window_set_default_size(GTK_WINDOW(win), 520, 520);	
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *table = gtk_table_new(6, 4, TRUE);
	gtk_container_add(GTK_CONTAINER(win), table);
	
	entry = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 4, 0, 1);
	
	
	GtkWidget *btn_add = gtk_button_new_with_label("+");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_add, 0, 1, 1, 2);
	GtkWidget *btn_sub = gtk_button_new_with_label("-");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_sub, 1, 2, 1, 2);
	GtkWidget *btn_mul = gtk_button_new_with_label("*");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_mul, 2, 3, 1, 2);
	GtkWidget *btn_div = gtk_button_new_with_label("/");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_div, 3, 4, 1, 2);
	
	GtkWidget *btn7 = gtk_button_new_with_label("7");
	gtk_table_attach_defaults(GTK_TABLE(table), btn7, 0, 1, 2, 3);
	GtkWidget *btn8 = gtk_button_new_with_label("8");
	gtk_table_attach_defaults(GTK_TABLE(table), btn8, 1, 2, 2, 3);
	GtkWidget *btn9 = gtk_button_new_with_label("9");
	gtk_table_attach_defaults(GTK_TABLE(table), btn9, 2, 3, 2, 3);
	
	GtkWidget *btn4 = gtk_button_new_with_label("4");
	gtk_table_attach_defaults(GTK_TABLE(table), btn4, 0, 1, 3, 4);
	GtkWidget *btn5 = gtk_button_new_with_label("5");
	gtk_table_attach_defaults(GTK_TABLE(table), btn5, 1, 2, 3, 4);
	GtkWidget *btn6 = gtk_button_new_with_label("6");
	gtk_table_attach_defaults(GTK_TABLE(table), btn6, 2, 3, 3, 4);
	
	GtkWidget *btn_equ = gtk_button_new_with_label("=");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_equ, 3, 4, 2, 4);
	
	GtkWidget *btn1 = gtk_button_new_with_label("1");
	gtk_table_attach_defaults(GTK_TABLE(table), btn1, 0, 1, 4, 5);
	GtkWidget *btn2 = gtk_button_new_with_label("2");
	gtk_table_attach_defaults(GTK_TABLE(table), btn2, 1, 2, 4, 5);
	GtkWidget *btn3 = gtk_button_new_with_label("3");
	gtk_table_attach_defaults(GTK_TABLE(table), btn3, 2, 3, 4, 5);
	GtkWidget *btn_del = gtk_button_new_with_label("DEL");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_del, 3, 4, 4, 5);
	
	GtkWidget *btn0 = gtk_button_new_with_label("0");
	gtk_table_attach_defaults(GTK_TABLE(table), btn0, 0, 2, 5, 6);
	GtkWidget *btn_dian = gtk_button_new_with_label(".");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_dian, 2, 3, 5, 6);
	GtkWidget *btn_cle = gtk_button_new_with_label("C");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_cle, 3, 4, 5, 6);
	
	g_signal_connect(btn_add, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_sub, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_mul, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_div, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn9, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn8, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn7, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn6, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn5, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn4, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_equ, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn3, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn2, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn1, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn0, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_dian, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_cle, "clicked", G_CALLBACK(deal_button), NULL);
	g_signal_connect(btn_del, "clicked", G_CALLBACK(deal_button), NULL);
	
	
	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

void deal_button(GtkWidget *button, gpointer user_data)
{
	char *s = (char *)gtk_button_get_label(GTK_BUTTON(button));
	char *t = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
	
	if(s[0] == '=') {
	
		if(t[0] < '0' || t[0] > '9') {
			gtk_entry_set_text(GTK_ENTRY(entry), "别闹");
			return;
		}
	
		int i = 0;
		double a = 0;
		while(t[i] >= '0' && t[i] <= '9') {
			a = a * 10 + t[i] - '0';
			i++;
		}
		if(t[i] == '.') {
			double k = 10;
			i++;
			while(t[i] >= '0' && t[i] <= '9') {
				a = a + (t[i] - '0') / k;
				k *= 10;
				i++;
			}
		}
		
		if(t[i] != '+' && t[i] != '-' && t[i] != '*' && t[i] != '/') {
			gtk_entry_set_text(GTK_ENTRY(entry), "别闹");
			return;
		}
		int op = i;
		i++;
		
		double b = 0;
		while(t[i] >= '0' && t[i] <= '9') {
			b = b * 10 + t[i] - '0';
			i++;
		}
		if(t[i] == '.') {
			double k = 10;
			i++;
			while(t[i] >= '0' && t[i] <= '9') {
				b = b + (t[i] - '0') / k;
				k *= 10;
				i++;
			}
		}
		if(t[i] != '\0') {
			gtk_entry_set_text(GTK_ENTRY(entry), "别闹");
			return;
		}
		
		double ans = 0;
		if(t[op] == '+'){
			ans = a + b;
		}
		else if(t[op] == '-') {
			ans = a - b;
		}
		else if(t[op] == '*') {
			ans = a * b;
		}
		else if(t[op] == '/') {
			if(b <= 0.000001) {
				gtk_entry_set_text(GTK_ENTRY(entry), "别闹");
				return;
			}
			ans = a / b;
		}
		
		char res[100];
		sprintf(res, "%.3f", ans);
		gtk_entry_set_text(GTK_ENTRY(entry), res);
		
	}
	else if(s[0] == 'C') {
		gtk_entry_set_text(GTK_ENTRY(entry), "");
	}
	else if(s[0] == 'D') {
		char e[100];
		int i = 0;
		while(t[i + 1]) {
			e[i] = t[i];
			i++;
		}
		e[i] = 0;
		gtk_entry_set_text(GTK_ENTRY(entry), e);
	}
	else {
		char e[100];
		int i = 0;
		while(t[i]) {
			e[i] = t[i];
			i++;
		}
		e[i++] = s[0];
		e[i] = 0;
		gtk_entry_set_text(GTK_ENTRY(entry), e);
	}
}