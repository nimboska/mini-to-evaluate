/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:14:00 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:14:00 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	is_valid_key(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=' && s[i] != '+')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	if (s[i] == '+' && s[i + 1] != '=')
		return (0);
	return (1);
}

static char	*extract_key(const char *s, int *is_append, int *has_eq)
{
	int		klen;
	char	*key;

	*is_append = 0;
	*has_eq = 0;
	klen = 0;
	while (s[klen] && s[klen] != '=' && s[klen] != '+')
		klen++;
	if (s[klen] == '+')
	{
		*is_append = 1;
		if (s[klen + 1] == '=')
			*has_eq = 1;
	}
	else if (s[klen] == '=')
		*has_eq = 1;
	key = (char *)ft_calloc(klen + 1, 1);
	if (!key)
		return (NULL);
	ft_memcpy(key, s, klen);
	return (key);
}

static int	set_var(t_env **env, const char *key, const char *val, int app)
{
	t_env	*node;
	char	*joined;

	node = find_env(*env, key);
	if (!app)
	{
		if (node)
			update_env_val(env, key, val);
		else
			add_new_env(env, key, val);
		return (0);
	}
	if (!node)
		return (add_new_env(env, key, val), 0);
	if (!node->value)
		return (update_env_val(env, key, val), 0);
	if (val)
		joined = ft_strjoin(node->value, val);
	else
		joined = ft_strjoin(node->value, "");
	if (!joined)
		return (1);
	update_env_val(env, key, joined);
	free(joined);
	return (0);
}

int	handle_arg(t_shell *sh, const char *arg)
{
	int		app;
	int		has_eq;
	char	*key;
	char	*val;

	if (!is_valid_key(arg))
		return (ft_putstr_fd("minishell: export: `", 2),
			ft_putstr_fd((char *)arg, 2),
			ft_putstr_fd("': not a valid identifier\n", 2), 1);
	key = extract_key(arg, &app, &has_eq);
	if (!key)
		return (1);
	val = NULL;
	if (has_eq)
		val = ft_strchr(arg, '=') + 1;
	if (set_var(&sh->env, key, val, app))
		return (free(key), 1);
	return (free(key), 0);
}
