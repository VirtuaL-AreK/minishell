/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:16:00 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 16:31:38 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
/* 
#include <stdio.h>

int main()
{
    char dest[] = "Hello";
    char src[] = "World";
	char dest2[] = "Hello";

    ft_memcpy(dest, src, 5);
    memcpy(dest2, src, 5);
    printf("ft_memcpy : %s\n", dest);
    printf("memcpy : %s\n", dest2);
    printf("%s\n", ft_memcpy(src+2, src, 5));
}
 */