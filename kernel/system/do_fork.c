/**
 * Syscall in this file: fork
 * Input:    
 *
 * Return:     reply_res: child's pid to parent,
 *                 and 0 to child
 * 
 * @author Bruce Tan
 * @email brucetansh@gmail.com
 * 
 * @author Paul Monigatti
 * @email paulmoni@waikato.ac.nz
 * 
 * @create date 2017-08-23 06:08:30
 * 
*/
#include "../winix.h"

/**
 * copy the pcb struct from parent to child
 * @param  parent 
 * @param  child  
 * @return        
 */
int copy_pcb(struct proc* parent, struct proc* child){
    int pbak;
    pid_t pidbak;
    pbak = child->proc_nr;
    pidbak = child->pid;
    *child = *parent;
    child->proc_nr = pbak;
    child->pid = pidbak;
    child->ptable = child->protection_table;
    return OK;
}

/**
 * allocate new virtual address space for chlid, and 
 * copy the whole process image from parent to child
 * @param  parent 
 * @param  child  
 * @return        
 */
int copy_mm(struct proc* parent, struct proc* child){
    ptr_t *src, *dest;
    int j;

    bitmap_clear(child->ptable, PTABLE_LEN);
    child->rbase = dup_vm(parent,child);
    if(child->rbase == NULL)
        return ERR;

    if(parent->i_flags & DISABLE_FIRST_PAGE){
        proc_memctl(child, NULL, PROC_NO_ACCESS);
    }
    src = (ptr_t *)parent->rbase;
    dest = (ptr_t *)child->rbase;
    while(src < parent->heap_bottom){
        copy_page(dest, src);
        src += PAGE_LEN;
        dest += PAGE_LEN;
    }
    return OK;
}


/**
 * copy physical registers. Physical registers are internally used by the kernel
 * Unlike virtual registers like $pc, $ra, some kernel space pointers point to the
 * actually physical memory for fast access
 * @param  parent 
 * @param  child  
 * @return        
 */
int copy_pregs(struct proc* parent, struct proc* child){
    ptr_t *sp;
    sp = get_physical_addr(parent->sp,parent);
    child->message = (struct message *)get_physical_addr(*( sp + 2 ), child);
    child->heap_break = get_physical_addr(get_virtual_addr(parent->heap_break, parent), child);
    child->heap_bottom = get_physical_addr(get_virtual_addr(parent->heap_bottom, parent), child);
    child->stack_top = get_physical_addr(get_virtual_addr(parent->stack_top, parent), child);
    return OK;
}




/**
 * syscall for fork
 * @param  parent 
 * @return        pid of the child, or -1 if forking is failed
 */
int sys_fork(struct proc *parent) {
    struct proc *child;
    // int tdb_page_len, sp_heap_page_len, page;

    if (child = get_free_proc_slot()) {
        copy_pcb(parent,child);

        if(copy_mm(parent,child) == ERR){
            free_slot(child);
            return ERR;
        }

        copy_pregs(parent,child);
        
        //Divide the quantum size between the parent and child
        //if quantum size is 1, quantum size is not changed
        if(parent->quantum != 1){
            //child get an extra quantum if the quantum size is odd
            child->quantum = (child->quantum + 1) / 2; 
            parent->quantum /= 2;
        }

        child->time_used = child->sys_time_used = 0;

        child->parent = parent->proc_nr;
        return child->proc_nr;
    }
    return ERR;
}

int do_fork(struct proc *who, struct message *m){
    int child_pr;
    child_pr = sys_fork(who);
    
    if(child_pr == ERR)
        return EINVAL;
    
    //send 0 to child
    syscall_reply(0, child_pr, m);

    //send the child pid to parent
    return child_pr;
}


