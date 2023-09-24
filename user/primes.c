#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test(int fileDescriptor){
  int number;
  read(fileDescriptor,&number,4);
  fprintf(1,"prime %d\n",number);

  int p[2];
  pipe(p);
  int tmp = -1;
  while(1){
    int readStatus = read(fileDescriptor,&tmp,4);
    if(readStatus <=0){ //v rure nam neostalo cislo, read vratim -1
      break;
    }
    if(tmp % number != 0) { //nedeli zapis cislo do pipe a pokracuj
      write(p[1],&tmp,4);
    } else { // cislo je delitelne cislom number neposielam do pipe
    }
  }
  if (tmp == -1) { //uz nic neni v rure koncim
    close(p[1]);
    close(p[0]);
    close(fileDescriptor);
    return;
  }
  int pid = fork();
  if (pid == 0){
    close(p[1]); // zavri zapisovaci fd rury
    close(fileDescriptor);
    test(p[0]); //zavolaj test s citacim fd rury
    close(p[0]);
  } else {
    close(p[1]);
    close(p[0]);
    close(fileDescriptor);
    wait(0); //cakam kym sa dokonci detsky proces
  }
}



int main(int argc, char argv[]){
  int p[2];
  pipe(p);
  for(int a = 2;a < 35;a++){
    int number = a;
    write(p[1],&number,4);
  }
  close(p[1]);
  test(p[0]);
  exit(0);
}
