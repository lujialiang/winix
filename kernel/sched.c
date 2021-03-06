/**
 * 
 * Winix Scheduling module
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
#include <kernel/sched.h>

PRIVATE struct timer sched_timer;

void init_sched(){
    memset(&sched_timer, 0, sizeof(struct timer));
    new_timer(&sched_timer, REBALANCE_TIMEOUT, rebalance_queues);
}

/**
 * This method is called every 12 timer interrupts
 * It effectly moves every processes in the ready queues
 * to the top priority ready queue, refer to Multi-fedback
 * queue scheduling for more details
 *  
 **/
void rebalance_queues(int proc_nr, clock_t time){
    struct proc* curr;
    for_each_proc_except_idle(curr)
    {
        if(curr->i_flags & IN_USE){
            curr->priority = MAX_PRIORITY;
        }
    }
    //Idle process always remain the lowest queue
    new_timer(&sched_timer, REBALANCE_TIMEOUT, rebalance_queues);
}

/**
 * Chooses a process to run.
 *
 * Returns:
 *   The process that is runnable with the highest priority.
 *   NULL if no processes are runnable (should never happen).
 *
 * Side Effects:
 *   A proc is removed from a ready_q.
 **/
struct proc *pick_proc() {
    int i;
    struct proc* mp;

    //Find the highest-priority non-empty queue
    for (i = 0; i < NUM_QUEUES; i++){
        if(mp = dequeue(ready_q[i])){
            if(get_debug_sched_count()){
                kprintf("|| %d || ", mp->proc_nr);
            }
            return mp;
        }
    }

    PANIC("No procs left");
    return NULL;
}


/**
 * The Scheduler.
 *
 * Notes:
 *   Context of current_proc must already be saved.
 *   If successful, this function does not return.
 *
 * Side Effects:
 *   current_proc has its accounting fields updated, and is reinserted to ready_q.
 *   current_proc is updated to point to the next runnable process.
 *   Context of the new proc is loaded.
 **/
void sched() {
    //irq count is increased for each exception being called, and cleared on exiting
    //exception
    reset_irq_count();

#ifdef _DEBUG
    //Check exception stack
    if(*(get_exception_stack_top()) != STACK_MAGIC)
        PANIC("Exception stack overflow\n");
#endif

    if (current_proc != NULL && !current_proc->s_flags) {

        if (current_proc->ticks_left > 0) {
            enqueue_head(ready_q[current_proc->priority], current_proc);
        }
        else { 
            //move the proc down to the lower ready queue, unless this proc
            //if already at the lowest ready queue, for every 100 timer interrupts
            //rebalance_queue is called which bumps every processes in the top
            //ready queue
            if(current_proc->priority < NUM_QUEUES - 1){
                current_proc->priority++;
            }
            enqueue_tail(ready_q[current_proc->priority], current_proc);
        }
    }

    current_proc = pick_proc();    

    //Reset quantum if needed
    if (current_proc->ticks_left <= 0) {
        current_proc->ticks_left = current_proc->quantum;
    }

    //Load context and run
    wramp_load_context();
}

