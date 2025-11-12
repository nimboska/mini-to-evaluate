/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:14:48 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:14:50 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	set_path(t_shell *shell)
{
	char	*current_path;

	if (!shell)
		return ;
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("minishell: getcwd");
		free(current_path);
		shell->last_status = 1;
		return ;
	}
	if (shell->path)
		free(shell->path);
	shell->path = current_path;
	update_env_val(&shell->env, "PWD", shell->path);
	free(current_path);
	shell->last_status = 0;
}

void	value_init(t_shell *shell)
{
	if (!shell)
		return ;
	shell->path = NULL;
	shell->last_status = 0;
	shell->env = NULL;
	shell->count_cmds = 0;
	shell->cmds = NULL;
	shell->line = NULL;
	signals_things();
}
