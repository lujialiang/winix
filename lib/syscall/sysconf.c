#include <lib.h>


long sysconf(int name){
    struct message m;
    m.m1_i1 = name;
    return _syscall(SYSCALL_SYSCONF, &m);
}

