

#ifndef PERIODIC_TASK_H
#define PERIODIC_TASK_H


typedef void (*pdCOMMAND_LINE_CALLBACK)(void);

/* The structure that defines command line commands.  A command line command
should be defined by declaring a const structure of this type. */
typedef struct PERIODIC_TASK_DEF
{
	pdCOMMAND_LINE_CALLBACK cmd;
	uint32_t period;
	uint32_t counter;
} periodic_task_t;


/*
 * Register the command passed in using the pxCommandToRegister parameter.
 * Registering a command adds the command to the list of commands that are
 * handled by the command interpreter.  Once a command has been registered it
 * can be executed from the command line.
 */
portBASE_TYPE periodic_task_cmd_register( periodic_task_t* pxCommandToRegister );
void create_periodic_task(uint16_t stack_depth_words,
		unsigned portBASE_TYPE task_priority);


#endif /* PERIODIC_TASK_H */

