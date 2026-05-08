#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>

inline std::string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             1900 + ltm->tm_year,
             1 + ltm->tm_mon,
             ltm->tm_mday);
    return std::string(buf);
}

#endif