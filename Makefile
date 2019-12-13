obj=main.c uart.c
exec=main.out
$(exec):$(obj)
	gcc $^ -o $@ `pkg-config --cflags --libs gtk+-2.0` -lpthread