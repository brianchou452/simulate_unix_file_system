#ifndef FILESYS_H
#define FILESYS_H

#define MAX_PATHNAME_LENGTH 64

extern char path_tokens[MAX_PATHNAME_LENGTH][MAX_PATHNAME_LENGTH];
extern char dirname[MAX_PATHNAME_LENGTH],
    basename[MAX_PATHNAME_LENGTH]; /* string holders */
typedef enum { Parent_dir, Dir } path_type;

extern int menu();

extern int mkdir(char *pathname);

extern int rmdir(char *pathname);

extern int ls(char *pathname);

extern int cd(char *pathname);

extern int pwd();

extern int create(char *pathname);

extern int rm(char *pathname);

extern int save();

extern int reload(char *pathname);

extern int quit();

#endif
