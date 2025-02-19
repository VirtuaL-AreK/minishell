/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:06:19 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 14:36:48 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = s;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}
/*
#include <stdio.h>
int main()
{
    char str[] = "Hello";
	ft_bzero(str+2, 5);
	int nb =6;
	int i =0;
    while(nb--)
    {
        printf("ptr[%d] = %d\n", i, str[i]);
        i++;
    }

    return(0);
} 
*/