#include <cstdio>       // printf(), fprintf()
#include <cstdlib>      // exit()
#include <unistd.h>     // fork(), execvp()
#include <sys/wait.h>   // waitpid()
#include <sys/ptrace.h> // ptrace()
#include <sys/user.h>   // user_regs_struct
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
        if (execlp("./syscall", "./syscall", nullptr) == -1) {
            err(EXIT_FAILURE, "execvp()");
        }
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            err(EXIT_FAILURE, "waitpid()");
        }

        ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL | PTRACE_O_TRACESYSGOOD);
        long long counter = 0LL;
        int enter = 0x01;

        while (WIFSTOPPED(status)) {
            if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {
                err(EXIT_FAILURE, "ptrace(PTRACE_SYSCALL)");
            }
            if (waitpid(pid, &status, 0) == -1) {
                err(EXIT_FAILURE, "waitpid()");
            }

            if (!WIFSTOPPED(status) || !(WSTOPSIG(status) & 0x80)) {
                continue;
            }

            struct user_regs_struct regs;
            if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1) {
                err(EXIT_FAILURE, "ptrace(PTACE_GETREGS)");
            }

            if (enter) { // syscall enter
                ++counter;
                printf("0x%llx: rax=%llx rdi=%llx rsi=%llx rdx=%llx r10=%llx r8=%llx r9=%llx\n",
                       regs.rip - 2, regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9);
            } else { // syscall exit
                printf("0x%llx: ret = 0x%llx\n", regs.rip - 2, regs.rax);
            }

            enter ^= 0x01;
        }

        printf("ASM{%lld}\n", counter);
    }

    return 0;
}