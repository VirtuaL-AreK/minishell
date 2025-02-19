/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:05:06 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 17:05:07 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	if (dest > src)
	{
		while (n > 0)
		{
			n--;
			((unsigned char *)dest)[n] = ((unsigned char *)src)[n];
		}
	}
	else
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dest);
}
/* 
#include <stdio.h>

int main()
{
    char dest[] = "Hello";
    char dest2[] = "Hello";
    char src[] = "World";

    printf("%lu : adresse de dest\n", (unsigned long)dest);
    printf("%lu : adresse de dest2\n", (unsigned long)dest2);
    printf("%lu : adresse de src\n", (unsigned long)src);
	// memmove(NULL, NULL, 5);
    ft_memmove(NULL, NULL, 5);
    printf("%s\n", dest);
    printf("%s\n", dest2);
}
 */