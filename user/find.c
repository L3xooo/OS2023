#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(int fd, char *dir, char *name) {
  struct dirent de;
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    struct stat st;
    char path[512];
   // catdir(dir, de.name, path);
    memcpy(path,dir,strlen(dir)+1);
    memcpy(path+strlen(path),"/",2);
    memcpy(path+strlen(path),de.name,strlen(de.name)+1);
    if(de.inum == 0)
        continue;
    if(stat(path, &st) < 0){
        printf("find: cannot stat %s\n", path);
        continue;
    }
    if (st.type == T_FILE && strcmp(de.name,name)==0) {
      printf("%s\n", path);
    } else if (st.type == T_DIR) {
      int subfd;
      if (strcmp(de.name,name) == 0) {
        printf("%s\n",path);
      }
      if((subfd = open(path, 0)) < 0){
        printf("find: cannot open %s\n", path);
        continue;
      }
      find(subfd, path, name);
    }

  }
}


int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "Usage: find dir name\n");
    exit(1);
  }

  char dir[DIRSIZ + 1];
  char name[DIRSIZ + 1];

  if (strlen(argv[1]) > DIRSIZ || strlen(argv[2]) > DIRSIZ) {
    fprintf(2, "dir or name too long...\n");
    exit(1);
  }

  memcpy(dir, argv[1], strlen(argv[1]));
  memcpy(name, argv[2], strlen(argv[2]));

  int fd;
  struct stat st;

  if((fd = open(dir, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", dir);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", dir);
    close(fd);
    exit(1);
  }

  if (st.type != T_DIR) {
    printf("%s is not a dir\n", dir);
  } else {
    find(fd, dir, name);
  }
  
  exit(0);
}
