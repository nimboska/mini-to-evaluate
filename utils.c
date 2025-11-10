/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:38:47 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/13 20:38:47 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	*safe_malloc(size_t bytes)
{
	void	*value;

	value = malloc(bytes);
	if (value == NULL)
		error_exit("Memory allocation problem!");
	return (value);
}

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "env") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	return (0);
}

char	*join_free(char **str1, char *str2)
{
	char	*temp;

	temp = ft_strjoin(*str1, str2);
	if (!temp)
		return (NULL);
	free(*str1);
	*str1 = temp;
	return (*str1);
}

char	*join_char(char **str1, char c)
{
	char	*res;
	size_t	len;
	size_t	i;

	i = 0;
	if (!str1 || !*str1)
		len = 0;
	else
		len = ft_strlen(*str1);
	res = (char *)malloc(len + 2);
	if (!res)
		return (NULL);
	while (i < len)
	{
		res[i] = (*str1)[i];
		i++;
	}
	res[len] = c;
	res[len + 1] = '\0';
	free(*str1);
	return (res);
}

int	is_operator(char c)
{
	if (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '\'' || c == '\"')
		return (1);
	return (0);
}
