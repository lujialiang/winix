/**
 * 
 * WINIX System Task.
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


#ifndef _K_SYSTEM_H_
#define _K_SYSTEM_H_ 1

#include <winix/syscall_ctx.h>

/**
 * Entry point for the system task.
 **/
void system_main();
struct message *curr_mesg();
void intr_syscall();
struct syscall_ctx* interrupted_syscall_ctx();

void syscall_region_begin();
void syscall_region_end();
void intr_syscall();
struct message *curr_mesg();
struct syscall_ctx *interrupted_syscall_ctx();
void kprint_sysinfo();

#endif
