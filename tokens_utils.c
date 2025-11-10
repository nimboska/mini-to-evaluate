/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:39:03 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/30 16:56:12 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_token	*append_operator(char *input, int *i)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (input[*i] == '|')
	{
		new_token->type = TOKEN_PIPE;
		new_token->value = ft_strdup("|");
		(*i)++;
	}
	else if (input[*i] == '>' || input[*i] == '<')
	{
		helper_redir(input, *i, new_token);
		if (input[*i + 1] == '>' || input[*i + 1] == '<')
			(*i) += 2;
		else
			(*i)++;
	}
	else
		return (free(new_token), NULL);
	new_token->next = NULL;
	return (new_token);
}

void	helper_redir(char *input, int i, t_token *new_token)
{
	if (input[i] == '>' && input[i + 1] == '>')
	{
		new_token->type = TOKEN_REDIRECT_APPEND;
		new_token->value = ft_strdup(">>");
	}
	else if (input[i] == '>')
	{
		new_token->type = TOKEN_REDIRECT_OUT;
		new_token->value = ft_strdup(">");
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		new_token->type = TOKEN_HEREDOC;
		new_token->value = ft_strdup("<<");
	}
	else if (input[i] == '<')
	{
		new_token->type = TOKEN_REDIRECT_IN;
		new_token->value = ft_strdup("<");
	}
	if (!new_token->value)
	{
		free(new_token);
		new_token = NULL;
	}
}

char	*expand_var(char *input, int *i, t_shell *shell)
{
	char	*key;
	char	*word;
	int		start;

	(*i)++;
	start = *i;
	if (input[*i] == '?')
	{
		(*i)++;
		return (get_var_value("?", shell));
	}
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup("$"));
	key = ft_substr(input, start, *i - start);
	if (!key)
		return (NULL);
	word = get_var_value(key, shell);
	free(key);
	return (word);
}

t_token	*append_word(char *input, int *i, t_shell *shell)
{
	t_token	*new_token;
	char	*word;
	char	*tmp;

	word = ft_strdup("");
	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	while (input[*i] && !(is_operator(input[*i])))
	{
		if (input[*i] == '$')
		{
			tmp = expand_var(input, i, shell);
			word = join_free(&word, tmp);
			free(tmp);
		}
		else
		{
			word = join_char(&word, input[*i]);
			(*i)++;
		}
	}
	new_token->type = TOKEN_WORD;
	new_token->value = word;
	return (new_token);
}
