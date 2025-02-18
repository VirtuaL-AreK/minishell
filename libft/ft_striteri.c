/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:30:17 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/30 00:30:17 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i])
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*
void function(unsigned int i, char *c)
{
    if(i%2 == 0)
        *c = ft_toupper(*c);
    else
        *c = ft_tolower(*c);
}

#include <stdio.h>
int main()
{
    char str[]="bonjour, cava";
    void (*f)(unsigned int, char*);
    f = function;

    printf("Avant : %s\n", str);
    ft_striteri(str, f);
    printf("Après : %s\n", str);
}
 */