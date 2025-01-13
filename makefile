all: client server
client: basic_client.o pipe_networking.o
	gcc -Wall -o client basic_client.o pipe_networking.o

server: forking_server.o pipe_networking.o
	gcc -Wall -o server forking_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	gcc -Wall -c basic_client.c

persistent_server.o: persistent_server.c pipe_networking.h
	gcc -c persistent_server.c

forking_server.o: forking_server.c pipe_networking.h
	gcc -c forking_server.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c basic_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~
