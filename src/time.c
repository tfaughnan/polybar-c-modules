/* time.c */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMESIZ 100

int main(int argc, char *argv[])
{
    char *fmt;
    if (argc == 2)
    {
        fmt = argv[1];
    }
    else
    {
        fmt = "%Y-%m-%d %H:%M:%S";
    }

    time_t unix_time = time(NULL);
    struct tm *time = localtime(&unix_time);

    char fmt_time[TIMESIZ];
    strftime(fmt_time, TIMESIZ, fmt, time);

    printf("%s\n", fmt_time);

    return EXIT_SUCCESS;
}
