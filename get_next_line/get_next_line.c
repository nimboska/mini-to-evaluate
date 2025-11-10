/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:33:47 by marvin            #+#    #+#             */
/*   Updated: 2025/02/28 14:37:45 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_joinfree(char *buf, char *tmp)
{
	char	*tmp2;

	tmp2 = buf;
	buf = ft_strjoin(buf, tmp);
	free(tmp2);
	tmp2 = NULL;
	return (buf);
}

char	*get_rest_of_file(char *result)
{
	char	*rest_of_file;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (result[i] && result[i] != '\n')
		++i;
	if (!result[i])
	{
		free(result);
		return (NULL);
	}
	rest_of_file = ft_calloc(ft_strlen(result) - i + 1, sizeof(char));
	while (result[i])
	{
		rest_of_file[j] = result[i + 1];
		++i;
		++j;
	}
	free(result);
	result = NULL;
	return (rest_of_file);
}

char	*get_one_line(char *result)
{
	int		i;
	char	*one_line;

	i = 0;
	if (!result[i])
		return (NULL);
	while (result[i] && result[i] != '\n')
		++i;
	one_line = ft_calloc(i + 2, sizeof(char));
	if (!one_line)
		return (NULL);
	i = 0;
	while (result[i] && result[i] != '\n')
	{
		one_line[i] = result[i];
		++i;
	}
	if (result[i] && result[i] == '\n')
		one_line[i] = '\n';
	return (one_line);
}

char	*read_line(int fd, char *result)
{
	char	*temp;
	int		bytes_read;

	temp = malloc(BUFFER_SIZE + 1);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, temp, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(temp);
			free(result);
			return (NULL);
		}
		temp[bytes_read] = 0;
		result = ft_joinfree(result, temp);
		if (ft_strchr(result, '\n'))
			break ;
	}
	free(temp);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*result;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	result = read_line(fd, result);
	if (!result)
		return (NULL);
	line = get_one_line(result);
	result = get_rest_of_file(result);
	return (line);
}
