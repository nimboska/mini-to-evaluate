/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 22:38:33 by mokon             #+#    #+#             */
/*   Updated: 2025/10/22 22:38:33 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static void	free_execctx(t_execctx *x)
{
	if (x->pipes)
		free(x->pipes);
	if (x->pidv)
		free(x->pidv);
}

static int	wait_for_all(t_execctx *x)
{
	int		i;
	int		status;
	int		last;

	last = 0;
	i = 0;
	while (i < x->n)
	{
		if (waitpid(x->pidv[i], &status, 0) == x->last_pid)
			last = status;
		i++;
	}
	if (WIFEXITED(last))
		return (WEXITSTATUS(last));
	else if (WIFSIGNALED(last))
		return (128 + WTERMSIG(last));
	return (1);
}

static int	create_fork(t_shell *shell, t_execctx *x, int i)
{
	x->pidv[i] = fork();
	if (x->pidv[i] == -1)
	{
		perror("fork");
		close_all_pipes(x);
		free_execctx(x);
		shell->last_status = 1;
		return (1);
	}
	if (x->pidv[i] == 0)
	{
		child(shell, x, i);
	}
	x->last_pid = x->pidv[i];
	if (x->n > 1 && i > 0)
	{
		close(pipe_read_fd(x, i - 1));
		close(pipe_write_fd(x, i - 1));
	}
	return (0);
}

int	run_pipeline_or_external(t_shell *shell)
{
	t_execctx	x;
	int			i;

	x.n = shell->count_cmds;
	x.pipes = NULL;
	x.pidv = (pid_t *)safe_malloc(sizeof(pid_t) * (x.n));
	x.last_pid = -1;
	if (create_pipes(&x) == -1)
	{
		free_execctx(&x);
		shell->last_status = 1;
		return (1);
	}
	i = 0;
	while (i < x.n)
	{
		if (create_fork(shell, &x, i))
			return (shell->last_status);
		i++;
	}
	close_all_pipes(&x);
	shell->last_status = wait_for_all(&x);
	free_execctx(&x);
	cmds_free(shell);
	return (shell->last_status);
}
