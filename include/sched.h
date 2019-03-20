#ifndef _SCHED_H
#define _SCHED_H

#define THREAD_CPU_CONTEXT				 	 0		//Offset of cpu_context in task_struct

#ifndef __ASSEMBLER__

#define THREAD_SIZE								4096

#define NR_TASKS										64

#define FIRST_TASK 	task[0]
#define LAST_TASK 	task[NR_TASKS-1]

#define TASK_RUNNING								 0

//Hold current task - task running
extern struct task_struct *current;
//Array of tasks
extern struct task_struct * task[NR_TASKS];
//Number of tasks that are scheduled
extern int nr_tasks;

/*This context only changed when we call cpu_switch_to
Accordingly to ARM calling conventions registers x0 - x18 can be overwritten by
the called function so caller can't assume those values will be kept*/
struct cpu_context
{
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
};

//This struct represents a process
struct task_struct
{
	struct cpu_context cpu_context;	//Registers which must be kept in switch
	long state;											//State of currently running task
	long counter;										//Check how much time process has been running
	long priority;									//Use it to regulate how much time it runs
	long preempt_count;							/*Task is running a critical section can't be
switched out, if it has a non zero value can't be switched out*/
};

extern void sched_init(void);
extern void schedule(void);
extern void timer_tick(void);
extern void preempt_disable(void);
extern void preempt_enable(void);
extern void switch_to(struct task_struct* next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);

//Initial task - kernel_main
#define INIT_TASK \
{
/*cpu_context*/		{0,0,0,0,0,0,0,0,0,0,0,0,0},	\
/* state*/				0,	\
/*counter*/ 			0,	\
/*priority*/			1,	\
/*preempt_count*/	0		\
}

#endif
#endif
