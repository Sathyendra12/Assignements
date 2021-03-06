server: rudi_server.h client_api client
	gcc -pthread -o server server_rpc_r_list.c server_rpc_r_write.c server_rpc_r_read.c server_r_list.c server_r_read.c server_r_write.c server_r_open.c server_r_close.c server_rpc_thread.c server_rpc_main.c -lpthread

client:	rudi_client.h client.o librudi librpc
	gcc -o client client.o -L. -lrudi -lrpc

client.o: rudi_client.h ./Test/client.c
	gcc -c ./Test/client.c

librudi: rudi_client.h client_r_close.o client_r_init.o client_r_list.o client_r_open.o client_r_read.o client_r_write.o
	ar cr librudi.a client_r_close.o client_r_init.o client_r_list.o client_r_open.o client_r_read.o client_r_write.o

librpc:	rudi_client.h client_rpc_r_close.o client_rpc_r_list.o client_rpc_r_open.o client_rpc_r_read.o client_rpc_r_write.o
	ar cr librpc.a client_rpc_r_close.o client_rpc_r_list.o client_rpc_r_open.o client_rpc_r_read.o client_rpc_r_write.o

client_api: rudi_client.h client_r_close.c client_r_init.c client_r_list.c client_r_open.c client_r_read.c client_r_write.c client_rpc_r_close.c client_rpc_r_list.c client_rpc_r_open.c client_rpc_r_read.c client_rpc_r_write.c
	gcc -c client_r_close.c client_r_init.c client_r_list.c client_r_open.c client_r_read.c client_r_write.c client_rpc_r_close.c client_rpc_r_list.c client_rpc_r_open.c client_rpc_r_read.c client_rpc_r_write.c

clean:
	rm -f *.o *.a client server
