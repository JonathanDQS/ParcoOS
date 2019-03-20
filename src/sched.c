#include "sched.h"
#include "irq.h"
#include "printf.h"

//Scheduler
//Initial task status
static struct task_struct init_task = INIT_TASK;
//Current task
struct task_struct *current = &(init_task);
//Array of tasks in scheduler
struct task_struct * task[NR_TASKS] = {&(init_task), };
//Number of tasks in queue
int nr_tasks = 1;

//Careful with these 2 one call in the wrong place and everything crashes

/*Preempt_count different from 0 so we don't get interrupted in the middle of
this process*/
void preempt_disable(void)
{
	current->preempt_count++;
}

//Preempt_count now 0 again so can be taken out
void preempt_enable(void)
{
	current->preempt_count--;
}

//Scheduling algorithm
void _schedule(void)
{
	//Disable preemtion of this process, don't want to switch in the middle
	preempt_disable();

	//Variable needed
	int next,c;
	struct task_struct * p;

	//Loop to check which task to run now
	while (1)
	{
		c = -1;
		next = 0;
		//Find task with greatest counter - time to run - priority
		for (int i = 0; i < NR_TASKS; i++)
		{
			//Set the pointer to that process
			p = task[i];
			//Given that it is ready - shoud rename to ready - and it is greater
			if (p && p->state == TASK_RUNNING && p->counter > c)
			{
				//To keep comparing
				c = p->counter;
				next = i;
			}
		}
		//If such task found break while
		if (c)
		{
			break;
		}
		//Else for each task
		for (int i = 0; i < NR_TASKS; i++)
		{
			p = task[i];
			if (p)
			{
				//Switch what's left in the counter and add the priority to check again
				p->counter = (p->counter >> 1) + p->priority;
			}
		}
	}
	//Switch to given task
	switch_to(task[next]);
	//And enable preemtion in case
	preempt_enable();
}

/*Function called by a process which cannot be preemted once it has finished
and also called regularly by a timer interrupt, time shared processing time*/
void schedule(void)
{
	current->counter = 0;
	_schedule();
}

//Switch to a given process
void switch_to(struct task_struct * next)
{
	//If chosen task is the same return to it
	if (current == next)
		return;

	//Else replace task
	struct task_struct * prev = current;
	current = next;
	//And change registers
	cpu_switch_to(prev, next);
}

//Enable preemtion in current task
void schedule_tail(void)
{
	preempt_enable();
}

//Called from timer interrupt
void timer_tick()
{
	//Decrease current task counter
	--current->counter;
	//If current task counter is more than 0, or can't be preemted
	if (current->counter > 0 || current->preempt_count > 0)
	{
		//Return to task
		return;
	}
	//Otherwise set counter of current to 0
	current->counter = 0;
	//Enable interrupts and call the algorithm for scheduling
	//Why? Because in theory all tasks could be waiting for an interrupt
	enable_irq();
	_schedule();
	disable_irq();
}
