/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:05:09 by mokon             #+#    #+#             */
/*   Updated: 2025/02/28 14:37:50 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (*str)
	{
		++i;
		++str;
	}
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int				i;
	int				j;
	char			*dest;
	unsigned char	*d;
	unsigned char	*p;

	p = (unsigned char *)s1;
	d = (unsigned char *)s2;
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	dest = malloc(i + j + 1);
	if (dest == NULL)
		return (NULL);
	i = -1;
	while (p[++i])
		dest[i] = p[i];
	j = -1;
	while (d[++j])
		dest[i + j] = d[j];
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	if (c == '\0')
		return ((char *)s + i);
	return (NULL);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned long	i;
	char			*str;

	i = 0;
	str = (char *)s;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;

	if (size != 0 && nmemb != 0)
		if (SIZE_MAX / nmemb <= size)
			return (NULL);
	p = malloc(size * nmemb);
	if (p == NULL)
		return (p);
	ft_bzero(p, size * nmemb);
	return (p);
}
