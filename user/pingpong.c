#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
  int p[2];
  int d[2];

  if(argc > 1) {
    fprintf(2,"Pingpong nepotrebuje argumenty\n");
    exit(1);
  }
  pipe(p);
  pipe(d);
  write(p[1],"a",1);
  close(p[1]);
  if(fork() == 0) {
    close(d[0]);
    read(p[0],"",1);
    close(p[0]);
    fprintf(1,"%d: received ping\n",getpid());
    write(d[1],"",1);
    close(d[1]);
  } else {
    wait(0);
    close(d[1]);
    read(d[0],"",1);
    close(d[0]);
    fprintf(1,"%d: received pong\n",getpid());
  }
  exit(0);
}

