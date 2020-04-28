#include <cstdio>   // fprintf()
#include <cstdlib>  // exit()
#include <cstring>  // strdup()
#include <unistd.h> // getopt()

int main(int argc, char *argv[]) {
    int opt;
    char *sopath = strdup("./sandbox.so");
    char *basedir = strdup(".");

    while ((opt = getopt(argc, argv, "p:d:-")) != -1) {
        switch (opt) {
            case 'p':
                free(sopath);
                sopath = strdup(optarg);
                break;
            case 'd':
                free(basedir);
                basedir = strdup(optarg);
                break;
            case '-':
                printf("--\n");
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

    execvp(argv[0], argv);

    /*
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = %s\n", optind + i, argv[i]);
    }

    printf("%s %s\n", sopath, basedir);
     */

    free(sopath);
    free(basedir);
    return 0;
}