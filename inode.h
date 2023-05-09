#ifndef INODE_H
#define INODE_H

typedef struct node {
  char name[64];         // name string of file or directory
  char type;             // 'D' for directory, 'F' for file
  struct node *parent;   // pointer to parent inode
  struct node *child;    // pointer to child inode
  struct node *sibling;  // pointer to sibling inode
} inode;

#endif