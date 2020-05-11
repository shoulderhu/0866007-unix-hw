#define _GNU_SOURCE

#include <stdio.h>     // fprintf(), realpath()
#include <stdlib.h>    // exit(), getenv()
#include <stdbool.h>   // bool
#include <string.h>    // strcmp(), strncmp()
#include <errno.h>     // errno
#include <dlfcn.h>     // dlopen(), dlsym(), dlerror(), dlclose()
#include <dirent.h>    // DIR
#include <err.h>       // errx()
#include <sys/stat.h>  // struct stat

#ifdef DEBUG
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

char pattern[1024], prefix[] = "sandbox";

void *handle;
static int (*chdir_original)(const char *);
static int (*chmod_original)(const char *, mode_t);
static int (*chown_original)(const char *, uid_t, gid_t);
static int (*creat_original)(const char *, mode_t);
static FILE *(*fopen_original)(const char *, const char *);
static int (*link_original)(const char *, const char *);
static int (*mkdir_original)(const char *, mode_t);
static int (*open_original)(const char *, int);
static int (*openat_original)(int, const char *pathname, int);
static DIR *(*opendir_original)(const char *);
static ssize_t (*readlink_original)(const char *, char *buf, size_t);
static int (*remove_original)(const char *);
static int (*rename_original)(const char *, const char*);
static int (*rmdir_original)(const char *);
static int (*__xstat_original)(int, const char *, struct stat *);
static int (*symlink_original)(const char*, const char*);
static int (*unlink_original)(const char*);

__attribute__((constructor)) static void constructor() {
    if ((handle = dlopen("libc.so.6", RTLD_LAZY)) == NULL) {
        errx(EXIT_FAILURE, "%s: %s", "dlopen()", dlerror());
    }

    if ((__xstat_original = dlsym(handle, "__xstat")) == NULL) {
        errx(EXIT_FAILURE, "%s: %s", "__xstat()", dlerror());
    }

    // printf("%s\n", getenv("BASEDIR"));

    realpath(getenv("BASEDIR"), pattern);
    if (strcmp(pattern, "/") != 0) {
        pattern[strlen(pattern)] = '/';
    }

    // printf("%s\n", pattern);
}

__attribute__((destructor)) static void destructor() {
    // TODO: No execution
    dlclose(handle);
}

bool get_access(const char *func, const char *name) {
    if (strlen(pattern) == 0) {
        return true;
    }

    struct stat s;
    __xstat_original(1, name, &s);

    char buf[1024];
    realpath(name, buf);

    if (S_ISDIR(s.st_mode) && strcmp(buf, "/") != 0) {
        buf[strlen(buf)] = '/';
    }

    if (strncmp(pattern, buf, strlen(pattern)) != 0) {
        fprintf(stderr, "[%s] %s: access to %s is not allowed\n", prefix, func, name);
        errno = EACCES;
        return false;
    }

    return true;
}

int chdir(const char *path) { // Done
    // TEST: ./sandbox ./test /etc/passwd
    // chdir(argv[1]);

    if (chdir_original == NULL) {
        if ((chdir_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, path)) {
        return -1;
    }

    DEBUG("[%s] %s %s\n", prefix, __func__, path);
    return chdir_original(path);
}

int chmod(const char *pathname, mode_t mode) { // Done
    // TEST: ./sandbox ./test /etc/passwd
    // chmod(argv[1], 0777);

    if (chmod_original == NULL) {
        if ((chmod_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s\n", prefix, __func__, pathname);
    return chmod_original(pathname, mode);
}

int chown(const char *pathname, uid_t owner, gid_t group) { // Done
    // TEST: ./sandbox ./test /etc/passwd
    // chown(argv[1], 1000, 1000);

    if (chown_original == NULL) {
        if ((chown_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s %d %d\n", prefix, __func__, pathname, owner, group);
    return chown_original(pathname, owner, group);
}

int creat(const char *pathname, mode_t mode) { // Done
    // TEST: ./sandbox ./test /tmp/creat.txt
    // creat(argv[1], 0644);

    if (creat_original == NULL) {
        if ((creat_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s : %s", "creat()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s %d\n", prefix, __func__, pathname, mode);
    return creat_original(pathname, mode);
}

FILE *fopen(const char *pathname, const char *mode) { // Done
    // TEST: ./sandbox ./test /tmp/fopen.txt
    // fopen(argv[1], "r+");

    if (fopen_original == NULL) {
        if ((fopen_original = dlsym(RTLD_NEXT, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s : %s", "fopen()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return NULL;
    }

    DEBUG("[%s] %s %s %s\n", prefix, __func__, pathname, mode);
    return fopen_original(pathname, mode);
}

int link(const char *oldpath, const char *newpath) { // Done
    // TEST: ./sandbox ./test /tmp/123 /tmp/234
    // link(argv[1], argv[2]);

    if (link_original == NULL) {
        if ((link_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s : %s", "link()", dlerror());
        }
    }

    if (!get_access(__func__, oldpath) ||
        !get_access(__func__, newpath)) {
        return -1;
    }

    DEBUG("[%s] %s %s %s\n", prefix, __func__, oldpath, newpath);
    return link_original(oldpath, newpath);
}

int mkdir(const char *pathname, mode_t mode) { // Done
    // TEST: ./sandbox ./test /tmp/mkdir
    // mkdir(argv[1], 0755);

    if (mkdir_original == NULL) {
        if ((mkdir_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s : %s", "mkdir()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s %d\n", prefix, __func__, pathname, mode);
    return mkdir_original(pathname, mode);
}

int open(const char *pathname, int flags) {
    // TEST: ./sandbox ./test /tmp/open.txt
    // open(argv[1], O_CREAT | O_TRUNC | O_RDWR);

    if (open_original == NULL) {
        if ((open_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "open()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s %d\n", prefix, __func__, pathname, flags);
    return open_original(pathname, flags);
}

int openat(int dirfd, const char *pathname, int flags) { // Done
    // TEST: ./sandbox ./test /tmp/openat.txt
    // openat(-1, argv[1], O_CREAT | O_TRUNC | O_RDWR);

    if (openat_original == NULL) {
        if ((openat_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "openat()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %d %s %d\n", prefix, __func__, dirfd, pathname, flags);
    return openat_original(dirfd, pathname, flags);
}

DIR *opendir(const char *name) { // Done
    // TEST: ./sandbox ls /

    if (opendir_original == NULL) {
        if ((opendir_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, name)) {
        return NULL;
    }

    DEBUG("[%s] %s %s\n", prefix, __func__, name);
    return opendir_original(name);
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz) { // Done
    // TEST: ./sandbox -- ls -l /lib/

    if (readlink_original == NULL) {
        if ((readlink_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s %s %zu\n", prefix, __func__, pathname, buf, bufsiz);
    return readlink_original(pathname, buf, bufsiz);
}

int remove(const char *pathname) { // Done
    // TEST: ./sandbox ./test /etc/passwd
    // remove(argv[1]);

    if (remove_original == NULL) {
        if ((remove_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s\n", prefix, __func__, pathname);
    return remove_original(pathname);
}

int rename(const char *oldpath, const char *newpath) { // Done
    // TEST: ./sandbox mv /tmp/c.txt /tmp/d.txt

    if (rename_original == NULL) {
        if ((rename_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, oldpath) ||
        !get_access(__func__, newpath)) {
        return -1;
    }

    DEBUG("[%s] %s %s %s\n", prefix, __func__, oldpath, newpath);
    return rename_original(oldpath, newpath);
}

int rmdir(const char *pathname) { // Done
    // TEST: ./sandbox rmdir /tmp

    if (rmdir_original == NULL) {
        if ((rmdir_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s\n", prefix, __func__, pathname);
    return rmdir_original(pathname);
}

int __xstat(int ver, const char *path, struct stat *stat_buf) {
    // TEST: ./sandbox ./test /etc/passwd
    // __xstat(1, argv[1], &stat);

    if (__xstat_original == NULL) {
        if ((__xstat_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "__xstat()", dlerror());
        }
    }

    if (!get_access(__func__, path)) {
        return -1;
    }

    DEBUG("[%s] %s %d %s\n", prefix, __func__, ver, path);
    return __xstat_original(ver, path, stat_buf);
}

int symlink(const char *target, const char *linkpath) { // Done
    // TEST: ./sandbox ./test /etc/passwd
    // symlink(argv[1], "pass");

    if (symlink_original == NULL) {
        if ((symlink_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, target) ||
        !get_access(__func__, linkpath)) {
        return -1;
    }

    DEBUG("[%s] %s %s %s\n", prefix, __func__, target, linkpath);
    return symlink_original(target, linkpath);
}

int unlink(const char *pathname) { // Done
    // TEST: ./sandbox unlink /etc/passwd

    if (unlink_original == NULL) {
        if ((unlink_original = dlsym(handle, __func__)) == NULL) {
            errx(EXIT_FAILURE, "%s: %s", "dlsym()", dlerror());
        }
    }

    if (!get_access(__func__, pathname)) {
        return -1;
    }

    DEBUG("[%s] %s %s\n", prefix, __func__, pathname);
    return unlink_original(pathname);
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
