// #include "inode_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inode.h"

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
  return node;
}

void printFileTree(inode *node, int depth) {
  // printf("printFileTree function\n");
  if (node == NULL) return;
  for (int i = 0; i < depth; i++) {
    printf("   ");
  }
  printf("├─ %c: %s\n", node->type, node->name);
  printFileTree(node->child, depth + 1);
  printFileTree(node->sibling, depth);
}