/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:31:44 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 17:04:35 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = c;
		i++;
	}
	return (s);
}

/* 
#include <stdio.h>
int main()
{
    char str[50] = "42schools if for programming geeks."; 
    printf("\nBefore memset(): %s\n", str); 
  
    // Fill 8 characters starting from str[13] with '.' 
    memset(str + 13, '.', 8*sizeof(char)); 
  
    printf("After memset():  %s\n", str); 

	// Write an integer with memset (2024)
	int a = 10;
	unsigned char *ptr;

	ptr = (unsigned char *)&a;
	printf("a before memset : %d\n", a);
	ft_memset(ptr, 232, 1);
	ft_memset(ptr+1, 7, 1);
	printf("a after memset : %d\n", a);
    return 0; 
}
 */