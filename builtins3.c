/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:13:51 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:13:51 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	valid_ident_unset(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	delete_env_node(t_env **env, t_env *prev, t_env *cur)
{
	if (prev)
		prev->next = cur->next;
	else
		*env = cur->next;
	free(cur->key);
	free(cur->value);
	free(cur);
}

static void	unset_one(t_env **env, const char *key)
{
	t_env	*prev;
	t_env	*cur;

	if (!env || !*env || !key)
		return ;
	prev = NULL;
	cur = *env;
	while (cur)
	{
		if (cur->key && ft_strcmp(cur->key, key) == 0)
		{
			delete_env_node(env, prev, cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	ft_unset(t_shell *sh, char **argv)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (valid_ident_unset(argv[i]))
			unset_one(&sh->env, argv[i]);
		else
		{
			write(2, "minishell: unset: not a valid identifier\n", 42);
			ret = 1;
		}
		i++;
	}
	return (ret);
}
