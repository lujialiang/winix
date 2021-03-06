#ifndef _K_TABLE_H_
#define _K_TABLE_H_

#include <sys/ipc.h>
#include <kernel/proc.h>

struct boot_image{
    char name[PROC_NAME_LEN];
    void (*entry)();
    int proc_nr;
    int quantum;
    int priority;
};

extern struct boot_image boot_table[NUM_TASKS];
extern char *initial_env[];
extern syscall_handler_t syscall_table[];

int no_syscall(struct proc* who, struct message* m);
int do_times(struct proc* who, struct message* m);
int do_exit(struct proc* who, struct message* m);
int do_fork(struct proc* who, struct message* m);
int do_vfork(struct proc* who, struct message* m);
int do_exec(struct proc* who, struct message* m);
int do_brk(struct proc* who, struct message* m);
int do_alarm(struct proc* who, struct message* m);
int do_sigaction(struct proc* who, struct message* m);
int do_sigreturn(struct proc* who, struct message* m);
int do_wait(struct proc* who, struct message* m);
int do_kill(struct proc* who, struct message* m);
int do_getpid(struct proc* who, struct message* m);
int do_winfo(struct proc* who, struct message* m);
int do_getc(struct proc* who, struct message* m);
int do_printf(struct proc* who, struct message* m);
int do_sysconf(struct proc* who, struct message* m);

#endif

