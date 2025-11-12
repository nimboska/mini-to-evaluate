/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:16:26 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:16:30 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	parse_and_execute(char *input, t_shell *shell)
{
	t_token	*list;
	int		st;

	if (!input || !*input)
		return (0);
	cmds_free(shell);
	list = lexer(input, shell);
	if (!input)
		return (shell->last_status = 130);
	shell->cmds = adding_command(list, shell);
	free_token_list(list);
	if (!shell->cmds || shell->count_cmds == 0 || !(&shell->cmds[0]))
		return (cmds_free(shell), 0);
	if (shell->count_cmds == 1 && shell->cmds->argv
		&& is_builtin(shell->cmds->argv[0]))
	{
		st = run_single_builtin(shell);
		shell->last_status = st;
		return (st);
	}
	else
		return (run_pipeline_or_external(shell));
	return (0);
}

int	read_input(t_shell *shell)
{
	char	*input;

	set_path(shell);
	while (true)
	{
		input = readline("\033[38;5;198mminishell$ \033[0m");
		if (!input)
		{
			write(2, "Goodbye! ;*\n", 12);
			free_env_list(shell->env);
			cmds_free(shell);
			free(shell);
			exit(1);
		}
		if (*input)
		{
			add_history(input);
			parse_and_execute(input, shell);
			free(input);
			cmds_free(shell);
		}
	}
	return (0);
}
