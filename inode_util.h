#ifndef INODE_UTIL_H
#define INODE_UTIL_H

#include "inode.h"

extern inode *root, *cwd;  // root and CWD pointers
extern void initialize();

extern inode *createNode(inode *parent, char *name, char type);
extern void removeNode(inode *parent, inode *node);

extern void printFileTree(inode *node, int depth);

#endif