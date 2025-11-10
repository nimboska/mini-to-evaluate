#include "mini.h"

int	apply_in_redir(t_cmd *cmd)
{
	int	tmp;

	if (cmd->in_fd >= 0)
	{
		tmp = cmd->in_fd;
		if (dup2(tmp, STDIN_FILENO) < 0)
		{
			close(tmp);
			cmd->in_fd = -1;
			return (1);
		}
		close(tmp);
		cmd->in_fd = -1;
	}
	return (0);
}

int	apply_out_redir(t_cmd *cmd)
{
	int	tmp;

	if (cmd->out_fd >= 0)
	{
		tmp = cmd->out_fd;
		if (dup2(tmp, STDOUT_FILENO) < 0)
		{
			close(tmp);
			cmd->out_fd = -1;
			return (1);
		}
		close(tmp);
		cmd->out_fd = -1;
	}
	return (0);
}

int	has_slash(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
		if (s[i++] == '/')
			return (1);
	return (0);
}

char	*join_path(const char *dir, const char *bin)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(dir, "/");
	res = ft_strjoin(tmp, bin);
	free(tmp);
	return (res);
}

t_cmd	*child_cmd(t_cmd *head, int i)
{
	while (head && i-- > 0)
		head = head->next;
	return (head);
}
