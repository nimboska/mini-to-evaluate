/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:07:44 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/30 16:42:11 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	init_cmd_defaults(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->argc = 0;
	cmd->infile = NULL;
	cmd->in_fd = -1;
	cmd->last_in_type = 0;
	cmd->last_heredoc_idx = -1;
	cmd->outs = NULL;
	cmd->outs_len = 0;
	cmd->out_fd = -1;
	cmd->heredocs = NULL;
	cmd->heredoc_cnt = 0;
	cmd->next = NULL;
}

t_cmd	*handle_pipe(t_cmd *node, t_token *tokens, t_shell *shell)
{
	t_cmd	*new_cmd;

	if (!node || tokens->next == NULL)
	{
		write(2, "Error: Wrong use of pipes :(\n", 29);
		return (NULL);
	}
	new_cmd = command_init(&shell, &shell->cmds);
	if (!new_cmd)
		return (NULL);
	return (new_cmd);
}

char	*get_var_value(char *name, t_shell *shell)
{
	t_env	*node;

	if (!name || !shell)
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(shell->last_status));
	node = find_env(shell->env, name);
	if (!node || !node->value)
		return (ft_strdup(""));
	return (ft_strdup(node->value));
}

int	handle_status(int status, char *path)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(path);
		return (130);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (WEXITSTATUS(status));
	return (0);
}
