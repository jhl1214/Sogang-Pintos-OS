#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

	void
syscall_init (void) 
{
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

	static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	uint32_t num;
	num = *(uint32_t *)(f->esp);

	switch(num){
		case SYS_HALT:
			syscall_halt();
			break;
		case SYS_EXIT:
			syscall_exit(*(int *)(f->esp+4));
			break;
		case SYS_EXEC:
			break;
		case SYS_WAIT:
			f->eax = syscall_wait(*(tid_t *)(f->esp+4));
			break;
		case SYS_CREATE:
			break;
		case SYS_REMOVE:
			break;
		case SYS_OPEN:
			break;
		case SYS_FILESIZE:
			break;
		case SYS_READ:
			break;
		case SYS_WRITE:
			f->eax = syscall_write(*(int *)(f->esp+4), *(int **)(f->esp+8), *(int *)(f->esp+12));
			break;
		case SYS_SEEK:
			break;
		case SYS_TELL:
			break;
		case SYS_CLOSE:
			break;
		case SYS_FIBO:
			f->eax = syscall_fibonacci(*(int *)(f->esp+4));
			break;
		case SYS_SUM:
			f->eax = syscall_sum_of_four_integers(*(int *)(f->esp+4), *(int *)(f->esp+8), *(int *)(f->esp+12), *(int *)(f->esp+16));
			break;
		default:
			printf("default\n");
	}
}

void syscall_halt(void){
	shutdown_power_off();
}

void syscall_exit(int status){
	struct thread *cur = thread_current();
	char *token[128], *save_ptr;
	int i;

	strlcpy(token, cur->name, strlen(cur->name)+1);
	strtok_r(token, " \0\n", &save_ptr);

	printf("%s: exit(%d)\n", token, status);

	thread_exit();
}

tid_t syscall_exec(const char *cmd_line){
	tid_t id;

	id = process_execute(cmd_line);
	if(id == -1)
		return -1;

	return id;
}

int syscall_wait(tid_t id){
	return (int)process_wait(id);
}

int syscall_write(int fd, const void *buffer, unsigned size){
	int i = 0;

	if(fd == 1){
		for(i=0;i<(int)size;i++){
			if(*(const char *)(buffer + i) == 0)
				break;
		}
		putbuf((const char *)buffer, i);
		return i;
	}

	return -1;
}

int syscall_fibonacci(int n){
	return n;
}

int syscall_sum_of_four_integers(int a, int b, int c, int d){
	return a + b + c + d;
}
