#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_sys.h"
#include "inode.h"
#include "inode_util.h"
#include "logger.h"

int num_tokens = 0;

int tokenize(char *pathname) {
  char path_tmp[64];
  strcpy(path_tmp, pathname);
  char *token;
  token = strtok(path_tmp, "/");
  char *last_token = token;
  int i = 0;
  while (token != NULL) {
    // printf("%s\n", token);
    last_token = token;
    strcpy(path_tokens[i], token);
    token = strtok(NULL, "/");
    i++;
  }
  strcpy(basename, last_token);

  asprintf(&log_formate_string, "basename: %s", basename);
  log_debug("tokenize", log_formate_string);

  int len_dirname = strlen(pathname) - strlen(basename) - 1;
  if (len_dirname < 0) {
    strcpy(dirname, "\0");
  } else {
    strncpy(dirname, pathname, len_dirname);
  }

  asprintf(&log_formate_string, "dirname: %s", dirname);
  log_debug("tokenize", log_formate_string);
  num_tokens = i;

  return i;
}

inode *path_to_node(char *pathname, path_type type) {
  // log_debug("path_to_node", "start path_to_node function");
  int num_tokens = tokenize(pathname);
  if (type == Parent_dir) {
    num_tokens--;
  }
  asprintf(&log_formate_string, "num_tokens: %d", num_tokens);
  log_debug("path_to_node", log_formate_string);

  inode *node = pathname[0] == '/' ? root : cwd;

  for (int i = 0; i < num_tokens; i++) {
    if (strcmp(path_tokens[i], ".") == 0) {
      continue;
    } else if (strcmp(path_tokens[i], "..") == 0) {
      node = node->parent;
      continue;
    }
    if (node->child == NULL) {
      asprintf(&log_formate_string, "%s does not exist(code:1)", pathname);
      log_error("path_to_node", log_formate_string);
      return NULL;
    }
    node = node->child;
    while (node != NULL) {
      if (strcmp(node->name, path_tokens[i]) == 0) {
        break;
      }
      node = node->sibling;
    }
    if (node == NULL) {
      asprintf(&log_formate_string, "%s does not exist(code:2)", pathname);
      log_error("path_to_node", log_formate_string);
      return NULL;
    }
  }
  asprintf(&log_formate_string, "Found node: %s", node->name);
  log_debug("path_to_node", log_formate_string);
  log_debug("path_to_node", "end of path_to_node function");
  return node;
}
