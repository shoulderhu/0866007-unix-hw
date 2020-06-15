#include <cstdio>       // printf(), fprintf()
#include <cstdlib>      // exit()
#include <cstring>      // strncpy()
#include <unistd.h>     // fork(), execlp(), read()
#include <sys/wait.h>   // waitpid()
#include <sys/ptrace.h> // ptrace()
#include <sys/user.h>   // user_regs_struct
#include <err.h>        // err()
#include <fcntl.h>      // open()

int main(int argc, char *argv[]) {
    pid_t pid;
    if ((pid = fork()) == -1) {
        err(EXIT_FAILURE, "fork()");
    }

    if (pid == 0) { // child process
        if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
            err(EXIT_FAILURE, "ptrace(PTRACE_TRACEME)");
        }
        if (execlp("./no_more_traps", "./no_more_traps", nullptr) == -1) {
            err(EXIT_FAILURE, "execlp()");
        }
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            err(EXIT_FAILURE, "waitpid()");
        }

        int fd;
        if ((fd = open("no_more_traps.txt", O_RDWR)) == -1) {
            err(EXIT_FAILURE, "open()");
        }

        int offset = 0;
        char opcodes[20000];
        if (read(fd, opcodes, sizeof(opcodes)) == -1) {
            err(EXIT_FAILURE, "read()");
        }

        ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);
        ptrace(PTRACE_CONT, pid, 0, 0);

        while (waitpid(pid, &status, 0) > 0) {
            if (!WIFSTOPPED(status)) {
                continue;
            }

            struct user_regs_struct regs;
            if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1) {
                err(EXIT_FAILURE, "ptrace(PTACE_GETREGS)");
            }

            unsigned long code;
            // unsigned char *ptr = (unsigned char *) &code;
            code = ptrace(PTRACE_PEEKTEXT, pid, regs.rip - 1, 0);
            /*printf("0x%llx: %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x\n", regs.rip - 1,
                   ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);*/

            char op[3] = {};
            strncpy(op, opcodes + offset, 2);
            offset += 2;
            unsigned long data = strtol(op, nullptr, 16);

            if (ptrace(PTRACE_POKETEXT, pid, regs.rip - 1, (code & 0xffffffffffffff00) | data) == -1) {
                err(EXIT_FAILURE, "ptrace(PTRACE_POKETEXT)");
            }

            regs.rip = regs.rip - 1;
            if (ptrace(PTRACE_SETREGS, pid, 0, &regs) == -1) {
                err(EXIT_FAILURE, "ptrace(SETREGS)");
            }

            /*code = ptrace(PTRACE_PEEKTEXT, pid, regs.rip, 0);
            printf("0x%llx: %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x\n", regs.rip,
                    ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);*/

            ptrace(PTRACE_CONT, pid, 0, 0);
        }
    }

    return 0;
}