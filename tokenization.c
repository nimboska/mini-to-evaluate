/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 21:14:06 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/11/10 20:51:09 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static void	add_redir(t_cmd *cmd, const char *path, int append)
{
	t_outredir	*new_out;

	new_out = (t_outredir *)malloc(sizeof(t_outredir) * (cmd->outs_len + 1));
	if (!new_out)
		return ;
	if (cmd->outs_len > 0)
		ft_memcpy(new_out, cmd->outs, sizeof(t_outredir)
			* cmd->outs_len);
	new_out[cmd->outs_len].path = ft_strdup(path);
	if (!new_out[cmd->outs_len].path)
	{
		free(new_out);
		return ;
	}
	new_out[cmd->outs_len].append = append;
	free(cmd->outs);
	cmd->outs = new_out;
	cmd->outs_len++;
}

void	handle_redirects(t_cmd *command, t_token *tokens)
{
	if (!tokens->next || tokens->next->type != TOKEN_WORD)
	{
		write(2, "Error: Wrong use of redirections :(\n", 36);
		return ;
	}
	if (tokens->type == TOKEN_REDIRECT_IN)
	{
		if (command->infile)
			free(command->infile);
		command->infile = ft_strdup(tokens->next->value);
		command->last_in_type = 1;
	}
	else if (tokens->type == TOKEN_REDIRECT_OUT)
		add_redir(command, tokens->next->value, 0);
	else if (tokens->type == TOKEN_REDIRECT_APPEND)
		add_redir(command, tokens->next->value, 1);
	tokens = tokens->next;
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*list;
	t_token	*curr;
	int		i;

	list = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == ' ' || input[i] == '\t')
		{
			i++;
			continue ;
		}
		else if (input[i] == '\'' || input[i] == '"')
			curr = handle_quote(input, &i, shell);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			curr = append_operator(input, &i);
		else
			curr = append_word(input, &i, shell);
		if (!curr)
			return (NULL);
		append_to_list(&list, curr);
	}
	return (list);
}

void	append_to_list(t_token **head, t_token *new_token)
{
	t_token	*last;

	if (!*head)
		*head = new_token;
	else
	{
		last = find_last(*head, offsetof(t_token, next));
		last->next = new_token;
	}
}

void	*find_last(void *stack, size_t offset)
{
	if (!stack)
		return (NULL);
	while (*(void **)(stack + offset) != NULL)
		stack = *(void **)(stack + offset);
	return (stack);
}
