#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_sys.h"
#include "inode_util.h"

extern inode *root, *cwd;                        /* root and CWD pointers */
char line[128];                                  /* user input line */
char command[16], pathname[MAX_PATHNAME_LENGTH]; /* user inputs */

int (*fptr[])(char *) = {(int (*)())menu, mkdir,  rmdir, ls,   cd,
                         (int (*)())pwd,  create, rm,    save, reload,
                         (int (*)())quit};

int findCmd(char *command) {
  char *cmd[] = {"menu",   "mkdir", "rmdir", "ls",     "cd",  "pwd",
                 "create", "rm",    "save",  "reload", "quit"};
  int i;
  for (i = 0; i < 11; i++) {
    if (strcmp(command, cmd[i]) == 0) return i;
  }
  return -1;
}

int main(void) {
  int id;
  initialize();
  printf("Enter menu for help menu\n");
  printf("%s > ", cwd->name);
  int stop = 0;
  while (!(stop > 0)) {
    fgets(line, 128, stdin);
    command[0] = pathname[0] = 0;
    sscanf(line, "%s %s", command, pathname);
    if (command[0] != 0) {
      id = findCmd(command);
      if (id < 0) {
        printf("invalid command, enter menu for help menu\n");
        printf("%s > ", cwd->name);
        continue;
      }

      stop = fptr[id](pathname);

      printFileTree(root, 0);  // 每次指令結束都印出 file tree

      printf("%s > ", cwd->name);
    }
  }
  return 0;
}
