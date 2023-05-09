#ifndef FILESYS_H
#define FILESYS_H

extern char path_tokens[64][64];
extern char dirname[64], basename[64]; /* string holders */
typedef enum { Parent_dir, Dir } path_type;

extern int menu();

extern int mkdir(char *pathname);

extern int rmdir(char *pathname);

extern int ls(char *pathname);

extern int cd(char *pathname);

extern int pwd();

extern int create(char *pathname);

extern int rm(char *pathname);

extern int save(char *pathname);

extern int reload(char *pathname);

extern int quit();

#endif
