/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:14:30 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:14:30 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	call_builtin(t_shell *sh, char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(sh, argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(sh, argv));
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(sh, argv));
	if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(sh, argv));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(sh, argv));
	if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(sh, argv));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit(sh, argv));
	return (0);
}

static int	restore_fds(int saved_stdin, int saved_stdout)
{
	int	flag;

	flag = 0;
	if (dup2(saved_stdin, STDIN_FILENO) < 0)
		flag = 1;
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		flag = 1;
	close(saved_stdin);
	close(saved_stdout);
	return (flag);
}

static int	save_fds(int *in, int *out)
{
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
	if (*in < 0 || *out < 0)
	{
		if (*in >= 0)
			close(*in);
		if (*out >= 0)
			close(*out);
		return (1);
	}
	return (0);
}

int	run_single_builtin(t_shell *shell)
{
	t_cmd	*cmd;
	int		saved_stdin;
	int		saved_stdout;
	int		status;

	cmd = shell->cmds;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (save_fds(&saved_stdin, &saved_stdout))
		return (1);
	apply_redirs(cmd);
	status = call_builtin(shell, cmd->argv);
	if (restore_fds(saved_stdin, saved_stdout) != 0 && status == 0)
		status = 1;
	shell->last_status = status;
	return (status);
}
