all: client server
client: basic_client.o pipe_networking.o
	gcc -o client basic_client.o pipe_networking.o

server: persistent_server.o pipe_networking.o
	gcc -o server persistent_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	gcc -c basic_client.c

persistent_server.o: persistent_server.c pipe_networking.h
	gcc -c persistent_server.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c basic_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~
