#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
  if(argc == 1) {
    fprintf(2,"Nezadal si argument!\n");
    exit(1);
  }
  if(argc > 2) {
    fprintf(2,"Zadal si vela argumetnov\n");
    exit(1);
  }
  int a = atoi(argv[1]);
  sleep(a);
  exit(0);
}
