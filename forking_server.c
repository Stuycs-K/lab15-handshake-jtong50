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
    from_client = server_setup();
    pid_t p1 = fork(); 
    if (p1 < 0){
        perror("forkfail"); 
        exit(1);
    }
    else if (p1 == 0){
        server_handshake_half(&to_client, from_client);
        if (close(from_client) == -1){
            perror("failed to close from_client"); 
        }
        if (close(to_client) == -1){
            perror("failed to close to_client");
        }
        signal(SIGINT, sighandler); 
        exit(0);
    }
  }
  
}