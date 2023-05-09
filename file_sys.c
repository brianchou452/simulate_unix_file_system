#include <stdio.h>
#include <string.h>

#include "file_sys_util.h"
#include "inode.h"
#include "inode_util.h"

char path_tokens[64][64];
char dirname[64], basename[64]; /* string holders */

int menu() {
  printf("menu function\n");
  return 0;
}
int mkdir(char *pathname) {
  log_debug("mkdir", "start mkdir function");
  // tokenize(pathname);
  inode *parent = path_to_node(pathname, Parent_dir);
  if (parent == NULL) {
    printf("mkdir: cannot create directory '%s': No such file or directory\n",
           pathname);
    return -1;
  }
  inode *node = createNode(parent, basename, 'D');
  if (parent->child == NULL) {
    parent->child = node;
  } else {
    inode *temp = parent->child;
    while (temp->sibling != NULL) {
      temp = temp->sibling;
    }
    temp->sibling = node;
  }

  return 0;
}
int rmdir(char *pathname) {
  printf("rmdir function %s\n", pathname);
  return 0;
}
int ls(char *pathname) {
  asprintf(&log_formate_string, "start ls function %s", pathname);
  log_debug("ls", log_formate_string);
  // tokenize(pathname);
  inode *node = pathname[0] == '\0' ? cwd : path_to_node(pathname, Dir);
  if (node == NULL) {
    asprintf(&log_formate_string,
             "cannot access '%s': No such file or directory", pathname);
    log_debug("ls", log_formate_string);
    return -1;
  }
  if (node->child == NULL) {
    printf("Directory is empty\n");
    return 0;
  }
  inode *temp = node->child;
  while (temp != NULL) {
    printf("%c\t%s\n", temp->type, temp->name);
    temp = temp->sibling;
  }
  return 0;
}
int cd(char *pathname) {
  printf("cd function %s\n", pathname);
  return 0;
}
int pwd() {
  printf("pwd function\n");
  return 0;
}
int create(char *pathname) {
  printf("create function %s\n", pathname);
  return 0;
}
int rm(char *pathname) {
  printf("rm function %s\n", pathname);
  return 0;
}
int save(char *pathname) {
  printf("save function %s\n", pathname);
  return 0;
}
int reload(char *pathname) {
  printf("reload function %s\n", pathname);
  return 0;
}

int quit() {
  printf("quit function\n");
  return 0;
}