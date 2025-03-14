#include "minishell.h"

void ft_free_args(char **args)
{
	int i = 0;

	while(args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
