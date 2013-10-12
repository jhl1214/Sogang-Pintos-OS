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
			//syscall_exit(a);
			break;
		case SYS_EXEC:
			//syscall_exec(a);
			break;
		case SYS_WAIT:
			//syscall_wait(a);
			break;
		case SYS_CREATE:
			//syscall_create(a, b);
			break;
		case SYS_REMOVE:
			//syscall_remove(a);
			break;
		case SYS_OPEN:
			//syscall_open(a);
			break;
		case SYS_FILESIZE:
			//syscall_filesize(a);
			break;
		case SYS_READ:
			//syscall_read(a, b, c);
			break;
		case SYS_WRITE:
			//syscall_write(a, b, c);
			break;
		case SYS_SEEK:
			//syscall_seek(a, b);
			break;
		case SYS_TELL:
			//syscall_tell(a);
			break;
		case SYS_CLOSE:
			//syscall_close(a);
			break;
		default:
			printf("default\n");
	}
	printf("This is Sparata!!!!!!\nnum is %d\n", num);
	thread_exit();
}

void syscall_halt(void){
	shutdown_power_off();
}

void syscall_exit(int status){
}

int syscall_exec(const char *cmd_line){
}

int syscall_wait(int pid){
}

bool syscall_create(const char *file, unsigned initial_size){
}

bool syscall_remove(const char *file){
}

int syscall_open(const char *file){
}

int syscall_filesize(int fd){
}

int syscall_read(int fd, void *buffer, unsigned size){
}

int syscall_write(int fd, const void *buffer, unsigned size){
}

void syscall_seek(int fd, unsigned position){
}

unsigned syscall_tell(int fd){
}

void syscall_close(int fd){
}
