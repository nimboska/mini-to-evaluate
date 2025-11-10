/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_things.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:37:43 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/30 17:42:41 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static char	**special_env_split(const char *str, char c)
{
	char	**arr;
	size_t	i;
	size_t	len;
	char	*k;

	if (!str)
		return (0);
	i = -1;
	len = 0;
	arr = malloc(sizeof(char *) * 3);
	if (!arr)
		return (0);
	while (str[++i] != c)
		len++;
	i = 0;
	k = ft_strchr(str, c);
	i = ft_strlen(str);
	if (len > 0)
		arr[0] = ft_substr(str, 0, len);
	if (k)
		arr[1] = ft_substr(str, len + 1, (i - len - 1));
	else
		arr[1] = 0;
	arr[2] = 0;
	return (arr);
}

static void	add_next(t_env **stack, t_env *new_node)
{
	t_env	*last_node;

	if (*stack == NULL)
		*stack = new_node;
	else
	{
		last_node = find_last(*stack, offsetof(t_env, next));
		last_node->next = new_node;
	}
}

static void	free_arr(char **str)
{
	if (!str)
		return ;
	free(str[0]);
	free(str[1]);
	free(str);
}

int	split_env(t_env **stack, char **str)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(*new_node));
	if (!new_node)
		return (free_arr(str), 0);
	if (str[0])
	{
		new_node->key = ft_strdup(str[0]);
		if (!new_node->key)
			return (free(new_node), free_arr(str), 0);
	}
	if (str[1])
	{
		new_node->value = ft_strdup(str[1]);
		if (!new_node->value)
		{
			new_node->value = ft_strdup("");
			if (!new_node->value)
				return (free(new_node->key), free(new_node), free_arr(str), 0);
		}
	}
	new_node->next = NULL;
	add_next(stack, new_node);
	free_arr(str);
	return (1);
}

void	create_list_env(t_env **stack, char **env)
{
	char	**str;
	int		i;

	i = -1;
	while (env[++i])
	{
		str = special_env_split(env[i], '=');
		if (!str)
			return ;
		if (!split_env(stack, str))
			return ;
	}
}
