/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 20:12:50 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/11/12 16:59:58 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	open_file(t_outredir *redir)
{
	int	fd;

	if (redir->append == 0)
		fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror("error opening file");
	return (fd);
}

static void	set_output(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->outs_len)
	{
		fd = open_file(&cmd->outs[i]);
		if (fd != -1)
		{
			if (i == cmd->outs_len - 1)
				dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		i++;
	}
}

static void	set_input(t_cmd *cmd)
{
	int		fd;
	char	*path;

	path = NULL;
	if (cmd->last_in_type == 2 && cmd->last_heredoc_idx >= 0)
		path = cmd->heredocs[cmd->last_heredoc_idx].tmp_path;
	else if (cmd->last_in_type == 1 && cmd->infile)
		path = cmd->infile;
	if (!path)
		return ;
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{	
		perror(path);
		exit(127);
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	apply_redirs(t_cmd *cmd)
{
	set_input(cmd);
	set_output(cmd);
}
