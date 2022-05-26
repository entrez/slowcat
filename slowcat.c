#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>

#define ERRLEN 80

useconds_t delay_time = 50000;
extern int errno;

static const char *const usage[] = {
    "Usage: slowcat -d<usec> <filename>",
    "",
    "Prints a file with a delay between each character",
    "",
    "Options:",
    "    -d<usec>    Microseconds of delay used (default 50000)",
    0
};

int
slowcat(const char *fn)
{
    FILE *fp;
    char c;
    if (!(fp = fopen(fn, "r")))
        return errno;
    while (fread(&c, 1, 1, fp) == 1) {
        printf("%c", c);
        fflush(stdout);
        usleep(delay_time);
    }
    fclose(fp);
    return 0;
}

int
main(int argc, char *argv[])
{
    int argnum = 1;
    if (argc < 2) {
        int i;
        for (i = 0; usage[i]; i++) {
            printf("%s\n", usage[i]);
        }
        return 0;
    }

    if (!strncmp("-d", argv[1], 2)) {
        char *delay_ptr = argv[1] + 2;
        delay_time = atoi(delay_ptr);
        argnum++;
    }

    for (; argnum < argc; argnum++) {
        const char *arg = argv[argnum];
        if (slowcat(arg)) {
            char name[ERRLEN]; 
            snprintf(name, ERRLEN, "slowcat: %s", arg);
            perror(name);
        }
    }
}
