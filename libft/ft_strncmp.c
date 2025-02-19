/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:45:48 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/25 21:45:49 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/* 
#include <stdio.h>

int main()
{
	char str1[]="aaab";
	char str2[]="aaac";
	char str3[]="aaaa";
	char str4[]="aaaaaa";

	printf("0 => %d\n", ft_strncmp(str1, str1, 4));
	printf("-1 => %d\n", ft_strncmp(str1, str2, 4));
	printf("1 => %d\n", ft_strncmp(str1, str3, 4));
	printf("1 => %d\n", ft_strncmp("abcdef", "abc\375xx", 5));

}
 */