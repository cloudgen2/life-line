#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

/**
 * @file log-message.h
 * @brief Write a message to the log file with timestamp and process ID
 *
 * @author Cloudgen Wong
 * @date 2023-06-03
 */

#include <libgen.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

char* logDirname(const char *data_log, const char* app);
char* prepareLogDir(const char *data_log, const char* app);
char* logFilenameWithTimeStamp(int debug_mode, char *log_dir, char* app);
void logMessageWithLogName(int useVersion, char *logFile, int pid, char *app, char *appName, const char *thread, const char *msg);
void logMessage(int useVersion, int debug_mode, char *app, char *appName, const char *thread, const char *msg);
void logMessageWithPid(int useVersion, int debug_mode, int pid, char *app, char *appName, const char *thread, const char *msg) ;
int init_log(const char* thread_name);
void log_message(const char *msg);
void simple_log(char* app, char* msg);
void simple_log_with_pid(int pid, char* app, char* msg);
void log_message_w_thread(const char *thread, const char *msg);
void debug_log_message(int debug_mode, const char *msg);
void debug_log_message_w_thread(int debug_mode, const char *thread, const char *msg);

#endif /* LOG_MESSAGE_H */
