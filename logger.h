#ifndef MY_LOGGER_H
#define MY_LOGGER_H

#define LOG_MSG_SIZE 200

void logMsg(const char *name, const char *msg, int option, int facility, int level);

#endif /* end MY_LOGGER_H */