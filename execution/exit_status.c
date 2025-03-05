#include "../minishell.h"

t_exit_status gexitstatus = SUCCESS;

void exit_here(t_exit_status order)
{
	gexitstatus = order;
}
