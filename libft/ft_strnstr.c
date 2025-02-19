/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:27:15 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/28 17:55:59 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[j] == '\0')
		return ((char *)big);
	if (len == 0)
		return (0);
	while (big[i] && i < len)
	{
		while (big[i + j] == little[j] && little[j] \
		&& big[i + j] && (i + j) < len)
		{
			j++;
		}
		if (little[j] == '\0')
			return ((char *)big + i);
		j = 0;
		i++;
	}
	return (NULL);
}
/* 
#include <stdio.h>
#include <string.h>
int main()
{
    char big[]="Boo Faz Bad";
    char little[]="Faz bad boooooo";

    printf("%p\n", ft_strnstr(big, little, 12));
    printf("%s\n", ft_strnstr(big, little, 12));
    printf("%s\n", strnstr(big, little, 12));
	printf("%s\n", ft_strnstr(NULL, little, 0));
	printf("%s\n", strnstr(NULL, little, 0));

}
 */