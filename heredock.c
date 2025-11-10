/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:30:40 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/28 17:02:45 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	hrdc_path(char **new_path)
{
	static int	i = 0;
	char		*path;
	char		*num;
	int			fd;

	while (1)
	{
		num = ft_itoa(i++);
		if (!num)
			return (-1);
		path = ft_strjoin("/tmp/.hrdc_", num);
		free(num);
		if (!path)
			return (-1);
		fd = open(path, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (fd >= 0)
		{
			*new_path = path;
			return (fd);
		}
		free(path);
		if (errno != EEXIST)
			return (-1);
	}
}

int	read_stdin(const t_heredoc *hd)
{
	int	fd;

	fd = open(hd->tmp_path, O_RDONLY);
	if (fd < 0)
		return (-1);
	unlink(hd->tmp_path);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	read_to_file(t_heredoc *new_hrdc)
{
	int		fd;
	pid_t	pid;
	int		status;

	fd = hrdc_path(&new_hrdc->tmp_path);
	if (fd < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		add_to_file(new_hrdc, fd);
	}
	close(fd);
	if (waitpid(pid, &status, 0) < 0)
		return (-1);
	return (handle_status(status, new_hrdc->tmp_path));
}

void	add_to_file(t_heredoc *new_hrdc, int fd)
{
	char	*line;
	size_t	len;

	while (1)
	{
		line = readline(">");
		if (!line || ft_strcmp(line, new_hrdc->delim) == 0)
		{
			free(line);
			break ;
		}
		len = ft_strlen(line);
		if (write(fd, line, len) != (ssize_t)len || write(fd, "\n", 1) != 1)
		{
			close(fd);
			free(line);
			error_exit("heredock failure\n");
		}
		free(line);
	}
	exit(0);
}

void	add_heredoc(t_cmd *cmd, t_token *delim)
{
	t_heredoc	*new_hrdc;

	if (!delim || !delim->next || !delim->next->value)
		return ;
	new_hrdc = (t_heredoc *)malloc(sizeof(t_heredoc) * (cmd->heredoc_cnt + 1));
	if (!new_hrdc)
		return (perror("malloc fail"));
	if (cmd->heredoc_cnt > 0)
	{
		ft_memcpy(new_hrdc, cmd->heredocs, sizeof(t_heredoc)
			*cmd->heredoc_cnt);
		free(cmd->heredocs);
	}
	new_hrdc[cmd->heredoc_cnt].delim = ft_strdup(delim->next->value);
	if (!new_hrdc[cmd->heredoc_cnt].delim)
	{
		free(new_hrdc);
		return (perror("malloc fail"));
	}
	new_hrdc[cmd->heredoc_cnt].tmp_path = NULL;
	read_to_file(&new_hrdc[cmd->heredoc_cnt]);
	cmd->heredocs = new_hrdc;
	cmd->last_in_type = 2;
	cmd->last_heredoc_idx = cmd->heredoc_cnt;
	cmd->heredoc_cnt++;
}
