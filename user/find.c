#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
void find(char *fileName,char *path) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  printf("Current file: %s\n",path);
  
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
   
   if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

 
  switch(st.type){
  case T_DIR:   
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';    
    printf("buffer: %s",buf);
    while(read(fd,&de,sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);     
        continue;
      }
      char buffer[strlen(buf)-1];
      strcpy(buffer,buf+2);
      if(strcmp("./.",buf) == 0){
      //  printf("not printgin");
        continue;
      } else if (strcmp("./..",buf) == 0){
    //    printf("not printgin");
        continue;
      } else if (strcmp(fileName,buffer) == 0){
        printf("%s/%s\n",path,fileName);
      } else {
        //printf("strlen:%d  buffer:%s\n",strlen(buffer),buffer);
        char newPath[strlen(path)+strlen(buffer)+1+1+1];
        strcpy(newPath,path);
        strcat(path,buffer);
        printf("strlen:%d",strlen(newPath));
        //strcpy(newPath[strlen(path)],buf)
        find(fileName,newPath);
      }
      //printf("%s \n",buf);
    }
    break;
  }
  close(fd);
}




int main(int argc, char *argv[]){

  if (argc < 2) {
    fprintf(2,"No fileName passed as argument\n");
    exit(1);
  }
  
  if(argc == 2) {
    find (argv[1],".");
  } else {
    find (argv[2],argv[1]);
  }

  //find(argv[1],".");
  exit(0);
}
