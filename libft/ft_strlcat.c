/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:01 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/28 17:25:34 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	slen;
	size_t	dlen;

	i = 0;
	if (!dst && size == 0)
		return (ft_strlen(src));
	slen = ft_strlen(src);
	dlen = ft_strlen(dst);
	if (size <= dlen || size == 0)
		return (size + slen);
	while (src[i] && i < size - dlen - 1)
	{
		dst[dlen + i] = src[i];
		i++;
	}
	dst[dlen + i] = '\0';
	return (dlen + slen);
}
/* 
#include <stdio.h>
#include <string.h>
int main()
{
	char dest[10] = "Hello";
	char dest2[10] = "Hello";
	char src[] = " you";


	printf("%ld\n", strlcat(NULL, src, 0));
	printf("%s\n", dest);
	printf("%lu\n", ft_strlcat(NULL, src, 0));
	printf("%s\n", dest2);
}
 */