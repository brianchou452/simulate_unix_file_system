// #include "inode_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inode.h"
#include "logger.h"

inode *root, *cwd;  // root and CWD pointers

void initialize() {
  root = malloc(sizeof(inode));
  root->type = 'D';
  strcpy(root->name, "/");
  root->parent = root;
  root->child = NULL;
  root->sibling = NULL;
  cwd = root;
}

inode *createNode(inode *parent, char *name, char type) {
  inode *node = malloc(sizeof(inode));
  strcpy(node->name, name);
  node->type = type;
  node->parent = parent;
  node->child = NULL;
  node->sibling = NULL;

  if (parent->child == NULL) {
    parent->child = node;
  } else {
    inode *temp = parent->child;
    while (temp->sibling != NULL) {
      if (strcmp(temp->name, name) == 0) {
        asprintf(&log_formate_string, "%s already exists", name);
        log_error("createNode", log_formate_string);
        return NULL;
      }
      temp = temp->sibling;
    }
    // 如果沒有進入while loop，
    // 就要檢查最後一個node的名字是否與要新增的node名字相同
    if (strcmp(temp->name, name) == 0) {
      asprintf(&log_formate_string, "%s already exists", name);
      log_error("createNode", log_formate_string);
      return NULL;
    }
    temp->sibling = node;
  }

  return node;
}

void removeNode(inode *parent, inode *node) {
  if (parent->child == node) {
    parent->child = node->sibling;
  } else {
    inode *temp = parent->child;
    while (temp->sibling != node) {
      temp = temp->sibling;
    }
    temp->sibling = node->sibling;
  }
  free(node);
}

void printFileTree(inode *node, int depth) {
  printf("\033[0;32;34m");
  if (node == NULL) return;
  for (int i = 0; i < depth; i++) {
    printf("   ");
  }
  printf("├─ %c: %s\n", node->type, node->name);
  printFileTree(node->child, depth + 1);
  printFileTree(node->sibling, depth);
  printf("\033[0m");
}