#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

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
    return 0;
}

int openat(int dirfd, const char *pathname, int flags) {
    return 0;
}

DIR *opendir(const char *name) {
    return NULL;
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz) {
    return 0;
}

int remove(const char *pathname) {
    return 0;
}

int rename(const char *oldpath, const char *newpath) {
    return 0;
}

int rmdir(const char *pathname) {
    return 0;
}

int stat(const char *pathname, struct stat *statbuf) {
    return 0;
}

int symlink(const char *target, const char *linkpath) {
    return 0;
}

int unlink(const char *pathname) {
    return 0;
}

int execl(const char *pathname, const char *arg, ...) {
    return 0;
}

int execle(const char *pathname, const char *arg, ...) {
    return 0;
}

int execlp(const char *file, const char *arg, ...) {
    return 0;
}

int execv(const char *pathanme, char *const argv[]) {
    return 0;
}

int execve(const char *pathname, char *const argv[], char *const envp[]) {
    return 0;
}

int execvp(const char *file, char *const argv[]) {
    return 0;
}

int system(const char *command) {
    return 0;
}

/*
uid_t getuid(void) {
    fprintf(stderr, "injected getuid, always return 0\n");
    return 0;
}
 */
