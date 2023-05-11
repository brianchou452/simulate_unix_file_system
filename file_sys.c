#include <stdio.h>
#include <string.h>

#include "file_sys_util.h"
#include "inode.h"
#include "inode_util.h"
#include "logger.h"

char path_tokens[MAX_PATHNAME_LENGTH][MAX_PATHNAME_LENGTH];
char dirname[MAX_PATHNAME_LENGTH], basename[MAX_PATHNAME_LENGTH];

int menu() {
  printf("mkdir rmdir ls cd pwd create rm save reload quit\n");
  printf("mkdir:\t make directory\n");
  printf("rmdir:\t remove directory\n");
  printf("ls:\t list directory\n");
  printf("cd:\t change directory\n");
  printf("pwd:\t print working directory\n");
  printf("create:\t create file\n");
  printf("rm:\t remove file\n");
  printf("save:\t save file system tree\n");
  printf("reload:\t reload file system tree\n");
  printf("quit:\t save and quit\n");
  return 0;
}

int mkdir(char *pathname) {
  log_debug("mkdir", "start mkdir function");
  inode *parent = path_to_node(pathname, Parent_dir);
  if (parent == NULL) {
    asprintf(&log_formate_string,
             "failed to create directory '%s': No such file or directory",
             pathname);
    log_error("mkdir", log_formate_string);
    return -1;
  }
  createNode(parent, basename, 'D');

  return 0;
}

int rmdir(char *pathname) {
  if (pathname[0] == '/' && pathname[1] == '\0') {
    asprintf(&log_formate_string, "failed to remove '%s': Is a root directory",
             pathname);
    log_error("rmdir", log_formate_string);
    return -1;
  }
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
  printf("%s\n", node_to_path(cwd));
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
  if (pathname[0] == '/' && pathname[1] == '\0') {
    asprintf(&log_formate_string, "failed to remove '%s': Is a root directory",
             pathname);
    log_error("rm", log_formate_string);
    return -1;
  }
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
  char filename[MAX_PATHNAME_LENGTH];
  strcpy(filename, pathname);
  if (filename[0] == '\0') {
    printf("Enter the name of the file to save to: ");
    scanf("%s", filename);
  }
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    printf("failed to open file %s\n", filename);
    return -1;
  }
  save_to_file(root->child, fp);
  fclose(fp);
  return 0;
}

int reload(char *pathname) {
  if (pathname[0] == '\0') {
    printf("Enter the name of the file to load from: ");
    scanf("%s", pathname);
  }

  FILE *fp = fopen(pathname, "r");
  if (fp == NULL) {
    printf("failed to open file %s\n", pathname);
    return -1;
  }
  load_from_file(fp);
  fclose(fp);

  return 0;
}

int quit() {
  printf("save before quit? (y/n): ");
  char c;
  scanf("%c", &c);
  if (c == 'y') {
    save("\0");
  }
  return 1;
}