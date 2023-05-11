#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_sys.h"
#include "inode.h"
#include "inode_util.h"
#include "logger.h"

int num_tokens = 0;

int tokenize(char *pathname) {
  char path_tmp[MAX_PATHNAME_LENGTH];
  strcpy(path_tmp, pathname);
  char *token;
  token = strtok(path_tmp, "/");
  char *last_token = token;
  int i = 0;
  while (token != NULL) {
    last_token = token;
    strcpy(path_tokens[i], token);
    token = strtok(NULL, "/");
    i++;
  }
  if (i == 0) {
    return 0;  // root 的特殊判斷
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
  log_debug("path_to_node", "start path_to_node function");
  if (pathname == NULL || strlen(pathname) == 0) {
    asprintf(&log_formate_string, "pathname is NULL or empty");
    log_error("path_to_node", log_formate_string);
    return NULL;
  }
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

char *node_to_path(inode *node) {
  char *path = malloc(sizeof(char) * MAX_PATHNAME_LENGTH);
  char path_tokens[MAX_PATHNAME_LENGTH][MAX_PATHNAME_LENGTH];
  int i = 0;
  while (node != root) {
    strcpy(path_tokens[i], node->name);
    node = node->parent;
    i++;
  }
  strcat(path, "/");
  for (int j = i - 1; j >= 0; j--) {
    strcat(path, path_tokens[j]);
    if (j != 0) {
      strcat(path, "/");
    }
  }
  return path;
}

void save_to_file(inode *node, FILE *fp) {
  if (node == NULL || fp == NULL) {
    return;
  }

  asprintf(&log_formate_string, "%c %s", node->type, node_to_path(node));
  log_debug("save_to_file", log_formate_string);

  fprintf(fp, "%c %s\n", node->type, node_to_path(node));
  save_to_file(node->child, fp);
  save_to_file(node->sibling, fp);
}

void load_from_file(FILE *fp) {
  if (fp == NULL) {
    return;
  }
  char line[MAX_PATHNAME_LENGTH];
  while (fgets(line, MAX_PATHNAME_LENGTH, fp) != NULL) {
    char type = line[0];
    char *pathname = line + 2;
    pathname[strlen(pathname) - 1] = '\0';
    asprintf(&log_formate_string, "%c %s", type, pathname);
    log_debug("load_from_file", log_formate_string);
    if (type == 'D') {
      mkdir(pathname);
    } else if (type == 'F') {
      create(pathname);
    }
  }
}
