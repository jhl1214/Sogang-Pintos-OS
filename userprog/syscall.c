#include "userprog/syscall.h"
#include "threads/vaddr.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

struct file_item{
	struct file *f;
	struct list_elem elem;
	int descripter;
};

static void syscall_handler (struct intr_frame *);
void syscall_halt (void);
void syscall_exit (int);
tid_t syscall_exec (const char *);
int syscall_wait (tid_t);
int syscall_write (int, const void *, unsigned);
int syscall_fibonacci (int);
int syscall_sum_of_four_integers (int, int, int, int);
bool syscall_create(const char *, unsigned);
bool syscall_remove(const char *);
int syscall_open(const char *);
int syscall_filesize(int);
int syscall_read(int, void *, unsigned);
void syscall_seek(int, unsigned);
unsigned syscall_tell(int);
void syscall_close(int);

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
			f->eax = syscall_exec(*(char **)(f->esp+4));
			break;
		case SYS_WAIT:
			f->eax = syscall_wait(*(tid_t *)(f->esp+4));
			break;
		case SYS_CREATE:
			f->eax = syscall_create(*(char **)(f->esp+4), *(int *)(f->esp+8));
			break;
		case SYS_REMOVE:
			f->eax = syscall_remove(*(char **)(f->esp+4));
			break;
		case SYS_OPEN:
			f->eax = syscall_open(*(char **)(f->esp+4));
			break;
		case SYS_FILESIZE:
			f->eax = syscall_filesize(*(int *)(f->esp+4));
			break;
		case SYS_READ:
			f->eax = syscall_read(*(int *)(f->esp+4), *(int **)(f->esp+8), *(int *)(f->esp+12));
			break;
		case SYS_WRITE:
			f->eax = syscall_write(*(int *)(f->esp+4), *(int **)(f->esp+8), *(int *)(f->esp+12));
			break;
		case SYS_SEEK:
			syscall_seek(*(int *)(f->esp+4), *(int *)(f->esp+8));
			break;
		case SYS_TELL:
			f->eax = syscall_tell(*(int *)(f->esp+4));
			break;
		case SYS_CLOSE:
			syscall_close(*(int *)(f->esp+4));
			break;
		case SYS_FIBO:
			f->eax = syscall_fibonacci(*(int *)(f->esp+4));
			break;
		case SYS_SUM:
			f->eax = syscall_sum_of_four_integers(*(int *)(f->esp+4), *(int *)(f->esp+8), *(int *)(f->esp+12), *(int *)(f->esp+16));
			break;
	}
}

void syscall_halt(void){
	shutdown_power_off();
}

void syscall_exit(int status){
	struct thread *cur = thread_current();
	char *token[128], *save_ptr;

	cur->ret_value = status;
	strlcpy(token, cur->name, strlen(cur->name)+1);
	strtok_r(token, " \0\n", &save_ptr);

	if(status < -1)
		status = -1;

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

int syscall_fibonacci(int n){//1 1 2 3 5 8 12
	int i;
	int a=1, b=1, sum=0;

	if(n<=0)
		return -1;

	if(n>0 && n<3)
		return 1;

	for(i=2;i<n;i++){
		sum = a + b;
		a = b;
		b = sum;
	}
	printf("fibonacci(%d) = %d\n", n, sum);
	return sum;
}

int syscall_sum_of_four_integers(int a, int b, int c, int d){
	printf("%d + %d + %d + %d = %d\n",a,b,c,d,a+b+c+d);
	return a + b + c + d;
}

/* Project 2-2 new system call functions */
/* Since 2013.10.30 */

bool syscall_create(const char *file, unsigned initial_size){
	if(file == NULL)
		syscall_exit(-1);

	return filesys_create(file, initial_size);
}

bool syscall_remove(const char *file){
	if(file == NULL)
		syscall_exit(-1);

	return filesys_remove(file);
}

int syscall_open(const char *file){
	struct thread *cur = thread_current();
	struct list *file_list = &cur->file_list;
	struct file_item *file_item = (struct file_item *)malloc(sizeof(struct file_item));

	if(file == NULL)
		return -1;

	file_item->f = filesys_open(file);
	if(file_item->f == NULL || !strcmp(file, ""))
		return -1;

	cur->fd_total = cur->fd_total + 1;
	file_item->descripter = cur->fd_total + 1;
	list_push_back(file_list, &(file_item->elem));

	return file_item->descripter;
}

int syscall_filesize(int fd){
	int size;
	struct list_elem *e;
	struct thread *cur = thread_current();
	struct list *file_list = &cur->file_list;

	for(e=list_begin(file_list);e!=list_end(file_list);e=list_next(e)){
		struct file_item *f = list_entry(e, struct file_item, elem);
		if(f->descripter == fd){
			size = file_length(f->f);
		}
	}

	return size;
}

int syscall_read(int fd, void *buffer, unsigned size){
	struct list_elem *e;
	struct thread *cur = thread_current();
	struct list *file_list = &cur->file_list;

	if(buffer > PHYS_BASE)
		syscall_exit(-1);

	for(e=list_begin(file_list);e!=list_end(file_list);e=list_next(e)){
		struct file_item *f = list_entry(e, struct file_item, elem);
		if(f->descripter == fd){
			return file_read(f->f, buffer, size);
		}
	}
}

void syscall_seek(int fd, unsigned position){
}

unsigned syscall_tell(int fd){
}

void syscall_close(int fd){
}
