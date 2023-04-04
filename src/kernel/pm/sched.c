/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <signal.h>
#include <nanvix/klib.h>

/**
 * @brief Schedules a process to execution.
 *
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 *
 * @param proc Process to be resumed.
 *
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void)
{
	struct process *p;	  /* Working process.     */
	struct process *next; /* Next process to run. */

	/* Name */
	int size = kstrlen(curr_proc->name);
	char name[26];
	kstrcpy(name, curr_proc->name);
	int len = 20 - size;
	int i = 0;
	for (i = size; i < len + size - 1; i++)
		*(name + i) = ' ';

	*(name + i) = '\0';

	kprintf("kstrncmp %d, nome %s\n", kstrncmp(name, "test", 4), name);
	if (kstrncmp(name, "test", 4) == 32)
	{
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			if (p->state == PROC_READY)
				kprintf("%d, ", p->pid);
		}
		kprintf("\n");
		/* Finishing process. */

		/* Remember this process. */
		last_proc = curr_proc;

		/* Check alarm. */
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			/* Skip invalid processes. */
			if (!IS_VALID(p))
				continue;

			/* Alarm has expired. */
			if ((p->alarm) && (p->alarm < ticks))
				p->alarm = 0, sndsig(p, SIGALRM);
		}
		// kprintf("Opa hehe\n");
		// kprintf("Status do joao: %d e name %s\n", curr_proc->status, name);

		
		/* Choose a process to run next. */
		next = IDLE;
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			// kprintf("PID no for: %d\n", p->pid);
			if (p->status == PROC_READY)
			{
				// kprintf("Status do joao: %d\n", p->status);
				//  kprintf("PID: %d\n", p->pid);
				next = p;
				break;
			}
		}
	}
	else
	{
		/* Re-schedule process for execution. */
		if (curr_proc->state == PROC_RUNNING)
			sched(curr_proc);

		/* Remember this process. */
		last_proc = curr_proc;

		/* Check alarm. */
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			/* Skip invalid processes. */
			if (!IS_VALID(p))
				continue;

			/* Alarm has expired. */
			if ((p->alarm) && (p->alarm < ticks))
				p->alarm = 0, sndsig(p, SIGALRM);
		}

		/* Choose a process to run next. */
		next = IDLE;
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			/* Skip non-ready process. */
			if (p->state != PROC_READY)
				continue;

			/*
			 * Process with higher
			 * waiting time found.
			 */
			if (p->counter > next->counter)
			{
				next->counter++;
				next = p;
			}

			/*
			 * Increment waiting
			 * time of process.
			 */
			else
				p->counter++;
		}

		next->counter = PROC_QUANTUM;
	}

	/* Switch to next process. */
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	if (curr_proc != next)
	{
		switch_to(next);
		// finisherF(curr_proc);
	}
}
