// Student Number: C14339246
#include <syslog.h>

// logs a message with flags set
void logMsg(const char *name, const char *msg, int option, int facility, int level) {
    openlog(name, option, facility);
    syslog(level, msg);
    closelog();
}
