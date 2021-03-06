/**
 * 
 * Winix system task for processing system calls
 *
 * @author Bruce Tan
 * @email brucetansh@gmail.com
 * 
 * @author Paul Monigatti
 * @email paulmoni@waikato.ac.nz
 * 
 * @create date 2016-09-19
 * 
*/

#include "winix.h"

PRIVATE struct message m;
PRIVATE int who_proc_nr;
PRIVATE struct proc *who;

PRIVATE ucontext_t recv_ctx;
PRIVATE struct syscall_ctx syscall_context;

/**
 * Entry point for system task.
 **/
void system_main() {
    int reply;
    syscall_handler_t handler;
    struct message* mesg = &m;
    kprint_sysinfo();
    getcontext(&recv_ctx);

    //Receive message, do work, repeat.
    while(true) {
        
        //get a message
        winix_receive(mesg);
        who_proc_nr = mesg->src;
        who = get_proc(who_proc_nr);

        syscall_region_begin();

        if(mesg->type >= 1 && mesg->type <= _NSYSCALL)
            handler = syscall_table[mesg->type];
        else
            handler = no_syscall;
        
        reply = handler(who,mesg);

        switch(reply){
            case SUSPEND:
            case DONTREPLY:
                break;
            default:
                syscall_reply(reply, who_proc_nr, mesg);
        }

        syscall_region_end();
        
    }
}

/**
 * print sys info of text, data and bss segment size
 */
 void kprint_sysinfo(){
    int free_mem_begin, mem_end;
    free_mem_begin = peek_next_free_page() * PAGE_LEN;
    mem_end = peek_last_free_page() * PAGE_LEN;
    kprintf("\r\nWINIX v%d.%d\r\n", MAJOR_VERSION, MINOR_VERSION);
    kprintf("Text Segment: 0x%08x - 0x%08x\r\n", &TEXT_BEGIN, &TEXT_END);
    kprintf("Data Segment: 0x%08x - 0x%08x\r\n", &DATA_BEGIN, &DATA_END);
    kprintf("BSS Segment:  0x%08x - 0x%08x\r\n", &BSS_BEGIN, &BSS_END);
    kprintf("Unallocated:  0x%08x - 0x%08x\r\n", free_mem_begin, mem_end);
    kprintf("%d kWords Free\r\n", 
    ((unsigned int)(mem_end - free_mem_begin + PAGE_LEN)) / PAGE_LEN); //inclusive
}

void syscall_region_begin(){

    //Bill the user proc's sys_used_time while executing syscall
    //on behalf of the user process
    set_bill_ptr(who);
    get_proc(SYSTEM)->i_flags |= BILLABLE;

    //Make sure system doesn't send a message to itself
    ASSERT(who != NULL && who_proc_nr != SYSTEM); 
}

void syscall_region_end(){
    get_proc(SYSTEM)->i_flags &= ~BILLABLE;
    //reset messages
    memset(&m, 0, sizeof(struct message));
    who_proc_nr = 0;
}

/**
 * interrupt the current executing system call
 * it saves the current syscall context into struct syscall_ctx
 * and start receiving syscalls again
 */
void intr_syscall(){
    struct syscall_ctx *ctx = &syscall_context;
    if(who_proc_nr){
        ctx->m = m;
        ctx->who = who;
        ctx->interruptted = true;
        setcontext(&recv_ctx);
    }
}

int no_syscall(struct proc* who, struct message* m){
    KPRINT_DEBUG(("Process \"%s (%d)\" performed unknown system call %d\r\n", 
        who->name, who->proc_nr, m->type));
    return ENOSYS;
}

struct message *curr_mesg(){
    return &m;
}

struct syscall_ctx *interrupted_syscall_ctx(){
    return &syscall_context;
}




