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
  createNode(parent, basename, 'D');

  return 0;
}
int rmdir(char *pathname) {
  // TODO: 不能移除root
  printf("rmdir function %s\n", pathname);
  inode *node = path_to_node(pathname, Dir);
  if (node == NULL) {
    asprintf(&log_formate_string,
             "failed to remove '%s': No such file or directory", pathname);
    log_error("rmdir", log_formate_string);
    return -1;
  }
  if (node->child != NULL) {
    asprintf(&log_formate_string, "failed to remove '%s': Directory not empty",
             pathname);
    log_error("rmdir", log_formate_string);
    return -1;
  }
  if (node->type == 'F') {
    asprintf(&log_formate_string, "cannot remove '%s': Not a directory",
             pathname);
    log_error("rmdir", log_formate_string);
  }

  inode *parent = path_to_node(pathname, Parent_dir);
  removeNode(parent, node);

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
    log_error("ls", log_formate_string);
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
  inode *node = path_to_node(pathname, Dir);
  if (node == NULL) {
    asprintf(&log_formate_string, "%s: No such file or directory", pathname);
    log_error("cd", log_formate_string);
    return -1;
  } else if (node->type == 'F') {
    asprintf(&log_formate_string, "%s: Not a directory", pathname);
    log_error("cd", log_formate_string);
    return -1;
  } else {
    cwd = node;
  }
  return 0;
}
int pwd() {
  printf("pwd function\n");
  char path[64][64];
  int i = 0;
  inode *node = cwd;
  while (node != root) {
    strcpy(path[i], node->name);
    node = node->parent;
    i++;
  }
  printf("/");
  for (int j = i - 1; j >= 0; j--) {
    printf("%s/", path[j]);
  }
  printf("\n");
  return 0;
}
int create(char *pathname) {
  log_debug("create", "start create function");
  inode *parent = path_to_node(pathname, Parent_dir);
  if (parent == NULL) {
    asprintf(&log_formate_string,
             "cannot create file '%s': No such file or directory", pathname);
    log_error("create", log_formate_string);
    return -1;
  }
  createNode(parent, basename, 'F');
  return 0;
}
int rm(char *pathname) {
  inode *node = path_to_node(pathname, Dir);
  if (node == NULL) {
    asprintf(&log_formate_string,
             "failed to remove '%s': No such file or directory", pathname);
    log_error("rm", log_formate_string);
    return -1;
  }
  if (node->type == 'D') {
    asprintf(&log_formate_string, "failed to remove '%s': Is a directory",
             pathname);
    log_error("rm", log_formate_string);
    return -1;
  }
  inode *parent = path_to_node(pathname, Parent_dir);
  removeNode(parent, node);

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