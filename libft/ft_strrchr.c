/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:20:53 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 13:20:54 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i--;
	}
	return (0);
}

/* #include <stdio.h>
int main()
{
    char str[]="teste";
    char *ptr;
    ptr = ft_strrchr(str, '\0');
    printf("Adresse str AVANT : %p\n", str);

    printf("%s\n", ptr);
    printf("Adresse str APRES: %p\n", ptr);
} */