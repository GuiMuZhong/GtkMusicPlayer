obj=jsq.c
exec=demo
$(exec):$(obj)
	gcc $^ -o $@ `pkg-config --cflags --libs gtk+-2.0`
clean:
	rm $(exec)