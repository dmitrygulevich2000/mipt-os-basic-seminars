#include <stddef.h>

#include <err.h>
#include <linux/audit.h>
#include <linux/bpf_common.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/signal.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <unistd.h>

static int install_filter() {
    struct sock_filter filter[] = {

        // load syscall number
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, nr)),

        // jump to allow if not write
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 0, 2),

        // load 1st argument (fd)
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, args)),

        // jump to kill if stdout
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, STDOUT_FILENO, 1, 0),

        // ret allow
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

        // ret kill
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS),
    };

    struct sock_fprog prog = {
        .len = sizeof(filter) / sizeof(filter[0]),
        .filter = filter,
    };
    if (syscall(SYS_seccomp, SECCOMP_SET_MODE_FILTER, 0, &prog) != 0) {
        err(1, "seccomp");
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        errx(1, "usage: %s command args...", argv[0]);
    }

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) {
        err(1, "prctl");
    }

    install_filter();

    // tests
    // printf("this is printf string\n");
    // const char write_string[] = "this is write string\n";
    // write(STDOUT_FILENO, write_string, sizeof(write_string));

    execvp(argv[1], argv + 1);
    err(1, "execvp");
}