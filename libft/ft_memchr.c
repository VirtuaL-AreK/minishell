/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:09:03 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 15:18:21 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return ((unsigned char *) s + i);
		i++;
	}
	return ((void *)0);
}
/* 
#include <stdio.h>
int main()
{
    char str[]="Salut";
	int a = 10;

    printf("%s\n", (char *)ft_memchr(str, 't', 5));
	printf("%p\n", &a);
	printf("%p\n", ft_memchr(&a-1, 10, 5));
	printf("%p\n", memchr(&a-1, 10, 5));
	printf("%p\n", ft_memchr(&a-1, 10, 4));
	printf("%p\n", memchr(&a-1, 10, 4));
}
 */