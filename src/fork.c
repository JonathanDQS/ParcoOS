#include "mm.h"
#include "sched.h"
#include "entry.h"

//Copy a process given function and argument
int copy_process(unsigned long fn, unsigned long arg)
{
	//Disable preemption - don't reschedule while in the middel of this task
	preempt_disable();
	//Create a pointer to this structure - process
	struct task_struct *p;

	//Get a free page to store the data of this process /* expression */when we switch - own stack
	p = (struct task_struct *) get_free_page();
	if (!p)
		return 1;
	p->priority = current->priority; //Set the same priority as the task running
	p->state = TASK_RUNNING;				 //Set state of this task, task ready to run
	p->counter = p->priority;				 //Counter (time running) same as priority
	p->preempt_count = 1; 					 /*Disable preemtion of this process
	until schedule_tail*/

	p->cpu_context.x19 = fn;				//Set arguments to be restored
	p->cpu_context.x20 = arg;
	p->cpu_context.pc = (unsigned long)ret_from_fork;   /*Look ret_from_fork*/
	p->cpu_context.sp = (unsigned long)p + THREAD_SIZE; //Set to allocated memory
	//Now modify number of tasks in queue
	int pid = nr_tasks++;
	//And add this process to the array of processes
	task[pid] = p;
	//Now this process can be preempted and let other take control
	preempt_enable();
	return 0;
}
