/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:04:56 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/20 11:04:56 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_cmd	*command_init(t_shell **sh, t_cmd **head)
{
	t_cmd	*cmd;
	t_cmd	*last;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	init_cmd_defaults(cmd);
	if (sh && *sh)
		(*sh)->count_cmds++;
	if (!head)
		return (cmd);
	if (!*head)
		*head = cmd;
	else
	{
		last = (t_cmd *)find_last(*head, offsetof(t_cmd, next));
		((t_cmd *)last)->next = cmd;
	}
	return (cmd);
}

void	add_cmd_argv(t_cmd *command, const char *arg)
{
	char	**upd_arg;
	int		i;
	int		j;

	i = 0;
	j = -1;
	if (command->argv)
		while (command->argv[i])
			i++;
	upd_arg = (char **)malloc(sizeof(char *) * (i + 2));
	if (!upd_arg)
		error_exit("Error: malloc error");
	while (++j < i)
		upd_arg[j] = ft_strdup(command->argv[j]);
	upd_arg[i] = ft_strdup(arg);
	upd_arg[i + 1] = NULL;
	if (command->argv)
	{
		i = 0;
		while (command->argv[i])
			free(command->argv[i++]);
		free(command->argv);
	}
	command->argv = upd_arg;
}

static void	handle_token_type(t_cmd *node, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_WORD)
		add_cmd_argv(node, (*tokens)->value);
	else if ((*tokens)->type == TOKEN_REDIRECT_IN
		|| (*tokens)->type == TOKEN_REDIRECT_OUT
		|| (*tokens)->type == TOKEN_REDIRECT_APPEND)
	{
		handle_redirects(node, *tokens);
		if (*tokens && (*tokens)->next)
			*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == TOKEN_HEREDOC)
	{
		add_heredoc(node, *tokens);
		*tokens = (*tokens)->next;
	}
}

static t_cmd	*process_token(t_cmd *node, t_token **tokens, t_shell *shell)
{
	if ((*tokens)->type == TOKEN_PIPE)
	{
		node = handle_pipe(node, *tokens, shell);
		if (!node)
			return (NULL);
	}
	else
		handle_token_type(node, tokens);
	return (node);
}

t_cmd	*adding_command(t_token *tokens, t_shell *shell)
{
	t_cmd	*node;

	if (!tokens)
		return (NULL);
	node = NULL;
	while (tokens)
	{
		if (node == NULL)
			node = command_init(&shell, &shell->cmds);
		if (!node)
			return (NULL);
		if (shell->cmds)
			shell->cmds->argc++;
		node = process_token(node, &tokens, shell);
		if (tokens)
			tokens = tokens->next;
	}
	return (shell->cmds);
}

//WORKING VERSION
//t_cmd	*adding_command(t_token *tokens, t_shell *shell)
//{
//	t_cmd	*node;

//	if (!tokens)
//		return (NULL);
//	node = NULL;
//	while (tokens)
//	{
//		if (node == NULL)
//			node = command_init(&shell, &shell->cmds);
//		if (!node)
//			return (NULL);
//		shell->cmds->argc++;
//		if (tokens->type == TOKEN_WORD)
//			add_cmd_argv(node, tokens->value);
//		else if (tokens->type == TOKEN_REDIRECT_IN
//			|| tokens->type == TOKEN_REDIRECT_OUT
//			|| tokens->type == TOKEN_REDIRECT_APPEND)
//		{
//			handle_redirects(node, tokens);
//			tokens = tokens->next;
//		}
//		else if (tokens->type == TOKEN_HEREDOC)
//		{
//			add_heredoc(node, tokens);
//			tokens = tokens->next;
//		}
//		else if (tokens->type == TOKEN_PIPE)
//		{
//			node = handle_pipe(node, tokens, shell);
//			if (!node)
//				return (NULL);
//		}
//		tokens = tokens->next;
//	}
//	return (shell->cmds);
//}