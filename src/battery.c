#include <sys/types.h>
#include <dirent.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *POWER_PATH = "/sys/class/power_supply";
char *BAT = "BAT";

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        BAT = argv[1];
    }

    DIR *d = opendir(POWER_PATH);
    if (!d)
    {
        fprintf(stderr, "opendir failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char cap_path[PATH_MAX];
    struct dirent *e;
    while ((e = readdir(d)))
    {
        if (strncmp(e->d_name, BAT, strlen(BAT)) == 0)
        {
            snprintf(cap_path, PATH_MAX, "%s/%s/capacity", POWER_PATH, e->d_name);
            break;
        }
    }
    closedir(d);

    FILE *cap_fp = fopen(cap_path, "r");
    if (!cap_fp)
    {
        fprintf(stderr, "fopen failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    int cap;
    fscanf(cap_fp, "%d", &cap);
    fclose(cap_fp);

    fprintf(stdout, "%d%%\n", cap);

    return EXIT_SUCCESS;
}
