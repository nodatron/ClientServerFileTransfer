#include <syslog.h>

/*
    void logMsg(const char *name, const char *msg, int option, int facility, int level)
        - name - the name given to the entry in the log
        - msg - the message to be placed in the log
        - option - the options for message
        - facility - the facility for the message
        - level - the level of the log message
    return void
    logs the message supplied
*/
void logMsg(const char *name, const char *msg, int option, int facility, int level) {
    openlog(name, option, facility);
    syslog(level, msg);
    closelog();
}