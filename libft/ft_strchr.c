/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:16:58 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 13:20:37 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
/* 
int main()
{
	char str1[]="Salut";

	printf("%p\n", str1);
	printf("%s\n", ft_strchr(str1, 'l'));
	printf("%p\n", ft_strchr(str1, 'l'));
	printf("%p\n", ft_strchr(str1, '\0'));
}
 */