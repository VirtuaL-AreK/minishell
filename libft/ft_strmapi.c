/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:34:58 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/30 11:34:58 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	len;
	char			*result;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = (*f)(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
/* 
char function(unsigned int i, char c)
{
    if(i%2 == 0)
        return(ft_toupper(c));
    else
        return(ft_tolower(c));
}

#include <stdio.h>
int main()
{
    char str[]="hello world !";
    char *result = ft_strmapi(str, function);

    if(!result)
    {
        printf("Error\n");
        return(1);
    }

    printf("Avant fonction : '%s'\n", str);
    printf("Après fonction : '%s'\n", result);

    free(result);
    return(0);
}
 */