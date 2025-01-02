#include "pipe_networking.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  char * fifoname = "wkp"; 
  mkfifo(fifoname, 0666); 
  int from_client = open(fifoname, O_RDONLY, 0);
  unlink(fifoname);
  return from_client;
}

/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup(); 
  char buffer[256];
  read(from_client, buffer, 255); 

  int pid = getpid(); 
  char privpipe[30]; 
  sprintf(privpipe, "%d", pid); 
  *to_client = open(privpipe, O_WRONLY, 0666);
  srand(time(NULL)); 
  int * num; 
  *num = rand();
  write(*to_client, num, sizeof(int));

  char name[256]; 
  read(*to_client, name, 255); 
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char privpipe[30];
  sprintf(privpipe, "%d", getpid()); 
  mkfifo(privpipe, 0666); 
  *to_server = open("wkp", O_WRONLY, 0); 
  write(*to_server, privpipe, 29); 
  int from_server = open(privpipe, O_RDONLY, 0);
  unlink(privpipe);
  int * num; 
  read(*to_server, num, sizeof(num));
  num++; 
  write(*to_server, num, sizeof(num)); 
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}


