all: client server
compile: basic_client.o forking_server.o pipe_networking.o
	@gcc -Wall -o clientt basic_client.o pipe_networking.o 
	@gcc -Wall -o serverr forking_server.o pipe_networking.o

client: clientt
	@./client

server: serverr
	@./server

basic_client.o: basic_client.c pipe_networking.h
	@gcc -Wall -c basic_client.c

forking_server.o: forking_server.c pipe_networking.h
	@gcc -c forking_server.c

persistent_server.o: persistent_server.c pipe_networking.h
	@gcc -c persistent_server.c

basic_server.o: basic_server.c pipe_networking.h
	@gcc -c basic_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~
