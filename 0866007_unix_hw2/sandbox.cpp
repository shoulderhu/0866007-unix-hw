#include <cstdio>   // fprintf()
#include <cstdlib>  // exit(), setenv(), free()
#include <cstring>  // strdup()
#include <unistd.h> // getopt(), execvp()
#include <err.h>    // err()

int main(int argc, char *argv[]) {
    int opt;
    char *sopath = strdup("./sandbox.so");
    char *basedir = strdup(".");

    while ((opt = getopt(argc, argv, "p:d:")) != -1) {
        switch (opt) {
            case 'p':
                free(sopath);
                sopath = strdup(optarg);
                break;
            case 'd':
                free(basedir);
                basedir = strdup(optarg);
                break;
            case '?':
            case ':':
            default: // show help message ...
                    fprintf(stderr, "usage: %s [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n", argv[0]);
                    fprintf(stderr, "        -p: set the path to sandbox.so, default = ./sandbox.so\n");
                    fprintf(stderr, "        -d: the base directory that is allowed to access, default = .\n");
                    fprintf(stderr, "        --: separate the arguments for sandbox and for the executed command\n");
                    exit(EXIT_FAILURE);
        }
    }

    argc -= optind;
    argv += optind;

    if (argc == 0) { // default message
        fprintf(stderr, "no command given.\n");
        exit(EXIT_FAILURE);
    }

    setenv("LD_PRELOAD", sopath, 1);
    free(sopath);

    setenv("BASEDIR", basedir, 1);
    free(basedir);


    if (execvp(argv[0], argv) == -1) {
        err(EXIT_FAILURE, "execvp()");
    }

    return 0;
}