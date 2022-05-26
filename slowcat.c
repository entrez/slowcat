#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>

#define ERRLEN 80

int slowcat(const char *);

useconds_t delay_time = 50000;
extern int errno;

static const char *const usage[] = {
    "Usage: slowcat -d<msec> <filename>",
    "",
    "Prints a file with a delay between each character",
    "",
    "Options:",
    "    -d<msec>    Milliseconds of delay used (default 50)",
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
    const char *arg;
    if (argc < 2) {
        int i;
        for (i = 0; usage[i]; i++) {
            fprintf(stderr, "%s\n", usage[i]);
        }
        return 0;
    }

    arg = argv[1];
    if (!strcmp("--", arg)) {
        /* if a filename starts with -d, invoke slowcat like
         *  $ slowcat -- -deep.txt
         * and it will work; otherwise it will think it's the -d option */
        argnum++;
    } else if (!strncmp("-d", arg, 2)) {
        const char *delay_ptr = arg + 2;
        int dtmp;
        if ((dtmp = atoi(delay_ptr)) == 0) {
            if (argc > 2)
                dtmp = atoi(argv[2]);
            argnum++;
        }
        if (dtmp) {
            delay_time = (useconds_t) dtmp * 1000;
            argnum++;
        } else {
            fprintf(stderr, "slowcat: ignoring -d: no time specified\n");
        }
    }

    for (; argnum < argc; argnum++) {
        arg = argv[argnum];
        if (slowcat(arg)) {
            char name[ERRLEN]; 
            snprintf(name, ERRLEN, "slowcat: %s", arg);
            perror(name);
        }
    }
}
