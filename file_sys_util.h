#ifndef FILESYSUTIL_H
#define FILESYSUTIL_H

#include "file_sys.h"
#include "inode.h"

extern char *log_formate_string;
extern int tokenize(char *pathname);

extern void log_debug(char *function_name, char *msg);
extern void log_error(char *function_name, char *msg);
extern inode *path_to_node(char *pathname, path_type type);

#endif