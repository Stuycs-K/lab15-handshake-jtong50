#include "pipe_networking.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

int err(){
    printf("errno %d\n",errno);
    printf("%s\n",strerror(errno));
    exit(1);
}

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  char * fifoname = WKP; 
  mkfifo(fifoname, 0666); 
  int from_client = open(fifoname, O_RDONLY, 0);
  if (from_client == -1){
    printf("Failed to open WKP: ");
    err(); 
  }
  printf("Server: Opened WKP\n"); 
  int rem_fifo = remove(fifoname);
  if (rem_fifo == -1){
    printf("Failed to remove WKP: ");
    err(); 
  }
  printf("Server: Removed WKP\n");
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
  //reading SYN (PP Name) from WKP
  char privpipe[255];
  int r_bytes = read(from_client, privpipe, 255); 
  if (r_bytes == -1){
    printf("Server: Failed to read from client: "); 
    err(); 
  }
  printf("Server: Read following SYN from client: %s\n", privpipe); 

  //opening PP 
  *to_client = open(privpipe, O_WRONLY, 0666);
  if (*to_client == -1){
    printf("Server: Failed to open private pipe: "); 
    err(); 
  }
  printf("Server: Opened PP\n");

  //writing SYN_ACK to PP/client
  srand(time(NULL)); 
  int x = rand() % 100;
  int w_bytes = write(*to_client, &x, sizeof(int)); 
  if (w_bytes == -1){
    printf("Server: Failed to write to client: "); 
    err(); 
  }
  printf("Server: Wrote SYN_ACK %d to client\n", x);
  sleep(1); 

  //reading ACK from PP/client
  int val; 
  int rr_bytes = read(from_client, &val, sizeof(int)); 
  if (rr_bytes == -1){
    printf("Server: Failed to read ACK from client"); 
    err(); 
  }
  printf("Server: Recieved ACK from client: %d -- SUCCESS!", val);
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
  //opening WKP 
  *to_server = open(WKP, O_WRONLY, 0); 
  if (*to_server == -1){
    printf("Client: Failed to open WKP: "); 
    err(); 
  }
  printf("Client: Opened WKP, unblocked server\n");

  //creating PP
  char privpipe[30];
  sprintf(privpipe, "%d", getpid()); 
  mkfifo(privpipe, 0666); 
  printf("Client: Created PP\n"); 

  //write SYN to WKP
  int w_bytes = write(*to_server, privpipe, 29); 
  if (w_bytes == -1){
    printf("Client: Failed to write private pipe to WKP: ");
    err(); 
  }
  printf("Client: Wrote SYN %d to WKP\n", getpid());

  //open PP
  int from_server = open(privpipe, O_RDONLY, 0);
  if (from_server == -1){
    printf("Client: Failed to open PP"); 
    err(); 
  }
  printf("Client: Opened PP, blocks PP\n");

  //remove PP
  int rem_pipe = remove(privpipe);
  if (rem_pipe == -1){
    printf("Client: Failed to remove pipe: "); 
    err(); 
  }
  printf("Client: Removed PP\n");

  // read SYN_ACK from PP sent by server
  int num; 
  int r_bytes = read(from_server, &num, sizeof(int));
  if (r_bytes == -1){
    printf("Client: Failed to read SYN_ACK: "); 
    err(); 
  }
  printf("Client: Server sent number %d\n", num);

  //write ACK to WKP 
  num++; 
  int ww_bytes = write(*to_server, &num, sizeof(int)); 
  if (ww_bytes == -1){
    printf("Client: Failed to write ACK to server: "); 
    err();
  }
  printf("Client: Wrote ACK %d to server\n", num);

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


