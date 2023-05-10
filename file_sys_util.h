#ifndef FILESYSUTIL_H
#define FILESYSUTIL_H

#include "file_sys.h"
#include "inode.h"

extern int tokenize(char *pathname);

extern inode *path_to_node(char *pathname, path_type type);

#endif