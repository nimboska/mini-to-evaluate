/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:41:51 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/29 20:08:18 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	find_second_quote(char *input, int i, char type_of_quote)
{
	while (input[i])
	{
		if (input[i] == type_of_quote)
			return (i);
		i++;
	}
	return (-1);
}

static char	*extract_quoted_word(char *input, int *i, int j, t_shell *shell)
{
	char	*word;
	char	*tmp;
	char	type;

	word = ft_strdup("");
	type = input[*i];
	(*i)++;
	while (input[*i] && *i < j)
	{
		if (input[*i] == '$' && type == '"')
		{
			tmp = expand_var(input, i, shell);
			word = join_free(&word, tmp);
			free(tmp);
		}
		else
			word = join_char(&word, input[(*i)++]);
	}
	if (input[*i] == type)
		(*i)++;
	return (word);
}

t_token	*handle_quote(char *input, int *i, t_shell *shell)
{
	t_token	*new_token;
	char	type;
	int		j;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	type = input[*i];
	j = find_second_quote(input, *i + 1, type);
	if (j == -1)
		return (write(2, "Error: Unmatched quote\n", 23), NULL);
	new_token->type = TOKEN_WORD;
	new_token->value = extract_quoted_word(input, i, j, shell);
	return (new_token);
}

//t_token	*handle_quote(char *input, int *i, t_shell *shell)
//{
//	t_token	*new_token;
//	char	*word;
//	char	*tmp;
//	char	type_of_quote;
//	int		j;

//	word = ft_strdup("");
//	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
//	if (!new_token)
//		return (NULL);
//	type_of_quote = input[*i];
//	j = find_second_quote(input, *i + 1, type_of_quote);
//	(*i)++;
//	while (input[*i] && *i < j)
//	{
//		if (input[*i] == '$' && type_of_quote == '"')
//		{
//			tmp = expand_var(input, i, shell);
//			word = join_free(&word, tmp);
//			free(tmp);
//		}
//		else
//		{
//			word = join_char(&word, input[*i]);
//			(*i)++;
//		}
//	}
//	if (input[*i] == type_of_quote)
//		(*i)++;
//	return (new_token->type = TOKEN_WORD, new_token->value = word, new_token);
//}
