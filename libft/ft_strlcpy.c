/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:39:31 by mokon             #+#    #+#             */
/*   Updated: 2025/01/06 14:35:37 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (size == 0 || src == NULL)
		return (src_len);
	while (*src && --size)
	{
		*dst = *src;
		src++;
		dst++;
	}
	*dst = '\0';
	return (src_len);
}
