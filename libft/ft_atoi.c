/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:42 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/25 21:44:42 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	size_t	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10;
		res = res + str[i] - 48;
		i++;
	}
	if (res > LONG_MAX)
		return (0 - (sign == 1));
	return (res * sign);
}
/* 
#include <stdio.h>
#include <string.h>
int main()
{
    printf("%d\n", ft_atoi("--42"));
    printf("%d\n", ft_atoi("42"));
    printf("%d\n", ft_atoi("-42"));
    printf("%d\n", ft_atoi("+15asd613"));
    printf("%d\n", ft_atoi("-2147483648"));
    printf("%d\n", ft_atoi("2147483650"));
    printf("%d\n", atoi("2147483650"));
	printf("%d\n", ft_atoi("-18446744073709551614"));
    printf("%d\n", atoi("-18446744073709551614"));
    printf("%d\n", ft_atoi("999999999999999999999999999999999999"));
    printf("%d\n", atoi("999999999999999999999999999999999999"));
	printf("%d\n", ft_atoi("-999999999999999999999999999999999999"));
    printf("%d\n", atoi("-999999999999999999999999999999999999"));
} 
 */