#include <fifo/fifo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

static pid_t child_make(int i, int listenfd);
static void sig_int(int signo); 

int idle_num;
pid_t * pids;

int main(int argc, char * argv[]) {
  int listenfd;
  int num = 10;
  int i;
  idle_num = 10;

  listenfd = init_server();
  pids = (pid_t *)(malloc(sizeof(pid_t) * num));
  for (i = 0; i < num; i++) {
    pids[i] = child_make(i, listenfd);
  }

  signal_wrapper(SIGINT, sig_int);
  close_wrapper(listenfd);
  while(true)
    pause();
  return 0;
}

static pid_t child_make(int i, int listenfd) {
  pid_t pid;
  if ((pid = fork_wrapper()) > 0)
    return pid;
  while(true) {
    char buf[32];
    wait_client(listenfd);
    fscanf(stdin, "%s", buf);
    fprintf(stdout, "sendback\n");
    fflush(stdout);
    fprintf(stderr, "gotit %s\n", buf);
  }
  exit(0);
}


static void sig_int(int signo) {
  int i;
  for (i = 0; i < idle_num; i++) {
    kill(pids[i], SIGTERM);
  }
  while (wait(NULL) > 0); 
  free(pids);
  exit(0); 
}
