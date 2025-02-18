/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:46:28 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/30 11:46:29 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nblen(int n)
{
	int		len;
	long	nb;

	len = 0;
	nb = n;
	if (nb < 0)
	{
		len++;
		nb = -nb;
	}
	if (nb == 0)
	{
		len++;
		return (len);
	}
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*str;
	long	nb;

	nb = n;
	i = nblen(n) - 1;
	str = malloc(sizeof(char) * (nblen(n) + 1));
	if (!str)
		return (NULL);
	str[nblen(n)] = '\0';
	if (nb == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[i] = nb % 10 + 48;
		nb /= 10;
		i--;
	}
	return (str);
}

// #include <stdio.h>
// int main()
// {
//     printf("%s\n", ft_itoa(0123));
//     printf("%s\n", ft_itoa(07));
//     printf("%s\n", ft_itoa(012));
//     printf("%s\n", ft_itoa(0223));
//     printf("%s\n", ft_itoa(0xdeadbeef));
//     printf("%s\n", ft_itoa(-2147483648));

//     return(0);
// }