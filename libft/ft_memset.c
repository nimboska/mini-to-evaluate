/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:38:19 by mokon             #+#    #+#             */
/*   Updated: 2025/01/05 18:42:58 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t num)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)ptr;
	while (i < num)
	{
		str[i] = value;
		i++;
	}
	return (ptr);
}
