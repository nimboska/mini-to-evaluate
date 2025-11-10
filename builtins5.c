#include "mini.h"

static void	print_escaped(const char *s)
{
	int	i;

	i = -1;
	while (s && s[++i])
	{
		if (s[i] == '\"' || s[i] == '\\')
			ft_putchar_fd('\\', 1);
		ft_putchar_fd(s[i], 1);
	}
}

static void	sort_env(t_env **a, int n)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(a[i]->key, a[j]->key) > 0)
			{
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_one_export(t_env *e)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(e->key, 1);
	if (e->value)
	{
		ft_putstr_fd("=\"", 1);
		print_escaped(e->value);
		ft_putstr_fd("\"\n", 1);
	}
	else
		ft_putstr_fd("\n", 1);
}

int	print_sorted_export(t_env *env)
{
	int		n;
	int		i;
	t_env	**arr;

	n = env_len(env);
	arr = (t_env **)ft_calloc(n, sizeof(*arr));
	if (!arr)
		return (1);
	i = 0;
	while (i < n)
	{
		arr[i] = env;
		env = env->next;
		i++;
	}
	sort_env(arr, n);
	i = 0;
	while (i < n)
	{
		print_one_export(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

int	ft_export(t_shell *sh, char **argv)
{
	int	i;
	int	code;

	if (!argv || !argv[1])
		return (print_sorted_export(sh->env));
	code = 0;
	i = 0;
	while (argv[++i])
		if (handle_arg(sh, argv[i]) != 0)
			code = 1;
	return (code);
}
