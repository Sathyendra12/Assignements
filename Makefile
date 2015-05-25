server: server_r_list.c server_r_open.c server_r_read.c server_r_close.c
	gcc -c server_r_list.c server_r_open.c server_r_read.c server_r_close.c

clean:
	rm *.o


