/*
 * Copyright (C) 2011-2013 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * setpgrp.c - setpgrp() system call implementation.
 */

#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <sys/types.h>

/*
 * Sets the process group ID.
 */
PUBLIC pid_t sys_setpgrp()
{
	/* Set process group ID. */
	if (curr_proc->pid != curr_proc->pgrp)
		curr_proc->pgrp = curr_proc->pid;
	
	return (curr_proc->pgrp);
}


