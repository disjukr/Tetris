#include "Time.hh"

#ifdef _WIN32
#include <time.h>
#include <Windows.h>

#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL

void gettimeofday(struct timeval* tv, int* tz) {
    tz;
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    GetSystemTimeAsFileTime(&ft);
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
    tmpres /= 10;
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv -> tv_sec = (long int) (tmpres / 1000000UL);
    tv -> tv_usec = (long int) (tmpres % 1000000UL);
}

#else
#include <cstddef>
#include <sys/time.h>

#endif

long long int Time::msec(void) {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000LL + time.tv_usec / 1000LL;
}
