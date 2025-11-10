/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:31:09 by mokon             #+#    #+#             */
/*   Updated: 2025/01/06 14:25:41 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *destination, const void *source, size_t num)
{
	size_t	i;	
	char	*dest;
	char	*sour;

	i = 0;
	dest = (char *)destination;
	sour = (char *)source;
	while (num != 0)
	{
		dest[i] = sour[i];
		num--;
		i++;
	}
	return (dest);
}
