#include <stdio.h>     // fprintf(), realpath()
#include <stdlib.h>    // exit(), getenv()
#include <stdbool.h>   // bool
#include <string.h>    //
#include <errno.h>     // errno
#include <regex.h>     // regexcomp(), regexec()
#include <stdarg.h>
#include <dlfcn.h>     // dlopen(), dlsym(), dlerror(), dlclose()
#include <dirent.h>
#include <sys/types.h> //
#include <sys/stat.h>  // struct stat

regex_t regex;
char pattern[1024], prefix[] = "sandbox";

void *handle;
static int (*open_original)(const char *, int);
static DIR *(*opendir_original)(const char *);
static ssize_t (*readlink_original)(const char *, char *buf, size_t);
static int (*remove_original)(const char *);
static int (*__xstat_original)(int, const char *, struct stat *);

__attribute__((constructor)) static void constructor() {
    if ((handle = dlopen("libc.so.6", RTLD_LAZY)) == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // printf("%s\n", getenv("BASEDIR"));
    if (realpath(getenv("BASEDIR"), pattern + 1) == NULL) {
        perror("realpath()");
        exit(EXIT_FAILURE);
    }

    pattern[0] = '^';
    // pattern[strlen(pattern)] = '/';

    if (regcomp(&regex, pattern, 0) != 0) {
        fprintf(stderr, "regcomp()\n");
        exit(EXIT_FAILURE);
    }

    // printf("%s\n", pattern);
}

__attribute__((destructor)) static void destructor() {
    // TODO: No execution
    dlclose(handle);
}

bool get_access(const char *func, const char *name) {
    char buf[1024];
    realpath(name, buf);

    if (regexec(&regex, buf, 0, NULL, 0) != 0) {
        fprintf(stderr, "[%s] %s: access to %s is not allowed\n", prefix, func, name);
        errno = EACCES;
        return false;
    }

    return true;
}

int chdir(const char *path) {
    return 0;
}

int chmod(const char *pathname, mode_t mode) {
    return 0;
}

int chown(const char *pathname, uid_t owner, gid_t group) {
    return 0;
}

int creat(const char *pathname, mode_t mode) {
    return 0;
}

FILE *fopen(const char *pathname, const char *mode) {

    return NULL;
}

int link(const char *oldpath, const char *newpath) {
    return 0;
}

int mkdir(const char *pathname, mode_t mode) {
    return 0;
}

int open(const char *pathname, int flags) {
    if (open_original == NULL) {
        if ((open_original = dlsym(handle, __func__)) == NULL) {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
        }
    }

    if (regexec(&regex, pathname, 0, NULL, 0) != 0) {
        fprintf(stderr, "[%s] %s: access to %s is not allowed\n",
                prefix, __func__, pathname);
        return -1;
    }

    printf("[sandbox] open\n");
    return open_original(pathname, flags);
}

int openat(int dirfd, const char *pathname, int flags) {
    return 0;
}

DIR *opendir(const char *name) { // Done
    // TEST: ./sandbox ls /

    if (opendir_original == NULL) {
        if ((opendir_original = dlsym(handle, __func__)) == NULL) {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
        }
    }

    if (!get_access(__func__, name)) {
        return NULL;
    }

    // printf("[%s] %s\n", prefix, __func__);
    return opendir_original(name);
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz) { // Done
    // TEST: ./sandbox -- ls -l /lib/

    if (readlink_original == NULL) {
        if ((readlink_original = dlsym(handle, __func__)) == NULL) {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
        }
    }

    if (!get_access(__func__, pathname)) {
        return 0;
    }

    // printf("[%s] %s\n", prefix, __func__);
    return readlink_original(pathname, buf, bufsiz);
}

int remove(const char *pathname) { // Done
    // TEST: ./sandbox ./test /etc/passwd
    // remove(argv[1]);

    if (remove_original == NULL) {
        if ((remove_original = dlsym(handle, __func__)) == NULL) {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    // printf("[%s] %s %s\n", prefix, __func__, pathname);
    return remove_original(pathname);
}

int rename(const char *oldpath, const char *newpath) {
    return 0;
}

int rmdir(const char *pathname) {
    return 0;
}

int __xstat(int ver, const char *path, struct stat *stat_buf) {

    return 0;
}

int symlink(const char *target, const char *linkpath) {
    return 0;
}

int unlink(const char *pathname) {
    return 0;
}

int exec(const char *func, const char *name) { // Done
    fprintf(stderr, "[%s] %s(%s): not allowed\n", prefix, func, name);
    errno = EACCES; // Permission denied
    return -1;
}

int execl(const char *pathname, const char *arg, ...) { // Done
    return exec(__func__, pathname);
}

int execle(const char *pathname, const char *arg, ...) { // Done
    return exec(__func__, pathname);
}

int execlp(const char *file, const char *arg, ...) { // Done
    return exec(__func__, file);
}

int execv(const char *pathname, char *const argv[]) { // Done
    return exec(__func__, pathname);
}

int execve(const char *pathname, char *const argv[], char *const envp[]) { // Done
    return exec(__func__, pathname);
}

int execvp(const char *file, char *const argv[]) { // Done
    return exec(__func__, file);
}

int system(const char *command) { // Done
    return exec(__func__, command);
}
