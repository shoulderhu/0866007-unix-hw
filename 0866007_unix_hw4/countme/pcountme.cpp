#include <cstdio>       // printf(), fprintf()
#include <cstdlib>      // exit()
#include <unistd.h>     // fork(), execvp()
#include <sys/wait.h>   // waitpid()
#include <sys/ptrace.h> // ptrace()
#include <err.h>        // err()

int main(int argc, char *argv[]) {
    pid_t pid;
    if ((pid = fork()) == -1) {
        err(EXIT_FAILURE, "fork()");
    }

    if (pid == 0) { // child process
        if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
            err(EXIT_FAILURE, "ptrace(PTRACE_TRACEME)");
        }
        if (execlp("./countme", "./countme", nullptr) == -1) {
            err(EXIT_FAILURE, "execlp()");
        }
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            err(EXIT_FAILURE, "waitpid()");
        }

        ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);
        long long counter = 0LL;

        while (WIFSTOPPED(status)) {
            ++counter;
            if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) == -1) {
                err(EXIT_FAILURE, "ptrace(PTRACE_SINGLESTEP)");
            }
            if (waitpid(pid, &status, 0) == -1) {
                err(EXIT_FAILURE, "waitpid()");
            }
        }

        printf("ASM{%lld}\n", counter);
    }

    return 0;
}