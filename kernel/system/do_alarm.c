/**
 * Syscall in this file: alarm
 * Input:   m1_i1: seconds
 *
 * Return:  m1_i1: previous timer seconds
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

//alarm syscall
//input     m.m1_i1    seconds
//output    m.reply_res    previous timeout
int do_alarm(struct proc *who, struct message *m){
    clock_t seconds;
    struct timer *alarm;
    clock_t prev_timeout;

    if(m->m1_i1 < 0)
        return EINVAL;

    seconds = (clock_t )m->m1_i1; 
    alarm = &who->alarm;
    prev_timeout = alarm->time_out; //return previous alarm

    if(alarm->flags & TIMER_INUSE){
        remove_timer(alarm);
    }
    
    //if seconds is 0, any pending alarm is canceled
    if(seconds != 0){
        alarm->proc_nr = who->proc_nr;
        alarm->time_out = get_uptime() + seconds * get_hz();
        alarm->handler = &deliver_alarm;
        insert_timer(alarm);
    }
    return prev_timeout;
}
