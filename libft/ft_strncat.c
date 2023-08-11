/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ayad <oel-ayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 20:19:04 by oel-ayad          #+#    #+#             */
/*   Updated: 2023/08/11 22:01:42 by oel-ayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *dest, const char *src, int nb)
{
	int		size;
	int		i;
	char	*result;

	result = dest;
	size = ft_strlen(result);
	i = 0;
	while (src[i] && i < nb)
	{
		result[size] = src[i];
		size++;
		i++;
	}
	result[size] = '\0';
	return (result);
}
