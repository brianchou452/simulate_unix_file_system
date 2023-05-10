#ifndef LOGGER_H
#define LOGGER_H
extern char *log_formate_string;
extern void log_debug(char *function_name, char *msg);
extern void log_error(char *function_name, char *msg);
#endif