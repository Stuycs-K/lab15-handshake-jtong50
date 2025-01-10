#include "pipe_networking.h"

static void sighandler(int signo){
    if (signo == SIGINT){
        remove(WKP); 
        exit(0);
    }
}

int main() {
  int to_client;
  int from_client;
  while(1){
    from_client = server_handshake( &to_client );
    if (close(from_client) == -1){
        perror("failed to close from_client"); 
    }
    if (close(to_client) == -1){
        perror("failed to close to_client");
    }
    signal(SIGINT, sighandler); 
  }
  
}