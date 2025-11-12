/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:13:42 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:13:42 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static char	*env_val(t_env *env, const char *k)
{
	t_env	*n;

	n = find_env(env, k);
	if (n)
		return (n->value);
	return (NULL);
}

static int	select_target(t_shell *sh, char **argv, char **target, int *print)
{
	*print = 0;
	if (!argv[1] || argv[1][0] == '\0')
	{
		*target = env_val(sh->env, "HOME");
		if (!*target)
			return (write(2, "minishell: cd: HOME not set\n", 28), 1);
		return (0);
	}
	if (ft_strcmp(argv[1], "-") == 0)
	{
		*target = env_val(sh->env, "OLDPWD");
		if (!*target)
			return (write(2, "minishell: cd: OLDPWD not set\n", 30), 1);
		*print = 1;
		return (0);
	}
	*target = argv[1];
	return (0);
}

int	ft_cd(t_shell *sh, char **argv)
{
	char	old[MINI_PATH_MAX];
	char	now[MINI_PATH_MAX];
	char	*target;
	int		print;

	if (argv[1] && argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 7);
	if (!getcwd(old, sizeof(old)))
		old[0] = '\0';
	if (select_target(sh, argv, &target, &print))
		return (1);
	if (chdir(target) != 0)
	{
		write(2, "minishell: cd: ", 15);
		perror(target);
		return (1);
	}
	if (print && getcwd(now, sizeof(now)))
		printf("%s\n", now);
	update_env_val(&sh->env, "OLDPWD", old);
	if (getcwd(now, sizeof(now)))
		update_env_val(&sh->env, "PWD", now);
	return (0);
}

int	ft_pwd(t_shell *sh, char **argv)
{
	char	buf[MINI_PATH_MAX];
	t_env	*node;

	(void)argv;
	if (getcwd(buf, sizeof(buf)))
	{
		write(1, buf, ft_strlen(buf));
		write(1, "\n", 1);
		return (0);
	}
	node = find_env(sh->env, "PWD");
	if (node && node->value)
	{
		write(1, node->value, ft_strlen(node->value));
		write(1, "\n", 1);
		return (0);
	}
	perror("pwd");
	return (1);
}
