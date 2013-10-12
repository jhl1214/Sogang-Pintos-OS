#include "threads/thread.h"
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void syscall_halt (void);
void syscall_exit (int);
tid_t syscall_exec (const char *);
int syscall_wait (tid_t);





int syscall_write (int, const void *, unsigned);
int syscall_fibonacci (int);
int syscall_sum_of_four_integers (int, int, int, int);

#endif /* userprog/syscall.h */
