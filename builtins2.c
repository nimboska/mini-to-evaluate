#include "mini.h"

static int	is_nflag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] == '\0')
		return (0);
	i = 1;
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	ft_echo(t_shell *sh, char **argv)
{
	int	i;
	int	nflag;
	int	first;

	(void)sh;
	i = 1;
	nflag = 0;
	while (argv[i] && is_nflag(argv[i]))
	{
		nflag = 1;
		i++;
	}
	first = 1;
	while (argv[i])
	{
		if (!first)
			write(1, " ", 1);
		first = 0;
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (!nflag)
		write(1, "\n", 1);
	return (0);
}

int	ft_env(t_shell *sh, char **argv)
{
	t_env	*cur;

	(void)argv;
	cur = sh->env;
	while (cur)
	{
		if (cur->value)
		{
			write(1, cur->key, ft_strlen(cur->key));
			write(1, "=", 1);
			write(1, cur->value, ft_strlen(cur->value));
			write(1, "\n", 1);
		}
		cur = cur->next;
	}
	return (0);
}

static int	is_numeric_str(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_shell *sh, char **argv)
{
	long	code;

	write(1, "exit\n", 5);
	if (!argv[1])
		code = sh->last_status;
	else if (!is_numeric_str(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		code = 2;
		exit((int)code);
	}
	else if (argv[2])
		return (write(2,
				"minishell: exit: too many arguments\n", 36), 1);
	else
		code = (long)ft_atol(argv[1]);
	free_env_list(sh->env);
	cmds_free(sh);
	free(sh);
	exit((int)((unsigned char)code));
}
