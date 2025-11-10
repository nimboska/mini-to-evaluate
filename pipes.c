#include "mini.h"

int	create_pipes(t_execctx *x)
{
	int	i;

	if (x->n <= 1)
		return (0);
	x->pipes = (int *)safe_malloc(sizeof(int) * (2 * (x->n - 1)));
	i = 0;
	while (i < x->n - 1)
	{
		if (pipe(&x->pipes[i * 2]) == -1)
		{
			perror("pipe");
			return (-1);
		}
		i++;
	}
	return (0);
}

void	close_all_pipes(t_execctx *x)
{
	int	i;

	if (x->n <= 1 || !x->pipes)
		return ;
	i = 0;
	while (i < x->n - 1)
	{
		close(x->pipes[i * 2]);
		close(x->pipes[i * 2 + 1]);
		i++;
	}
}

int	pipe_read_fd(t_execctx *x, int idx)
{
	return (x->pipes[idx * 2]);
}

int	pipe_write_fd(t_execctx *x, int idx)
{
	return (x->pipes[idx * 2 + 1]);
}

void	wire_child_pipes(t_execctx *x, int i)
{
	if (x->n > 1 && i > 0)
		dup2(pipe_read_fd(x, i - 1), STDIN_FILENO);
	if (x->n > 1 && i < x->n - 1)
		dup2(pipe_write_fd(x, i), STDOUT_FILENO);
}
