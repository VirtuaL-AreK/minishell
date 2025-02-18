/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 21:29:49 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/24 21:30:36 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;

	nb = n;
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
	}
	if (nb > 9)
	{
		ft_putnbr_fd(nb / 10, fd);
		ft_putnbr_fd(nb % 10, fd);
	}
	else
		ft_putchar_fd(nb + 48, fd);
}
/* 
int main()
{
    ft_putnbr_fd(42, 1);
    ft_putchar_fd('\n', 1);
    ft_putnbr_fd(-42, 1);
    ft_putchar_fd('\n', 1);
    ft_putnbr_fd(0, 1);
    ft_putchar_fd('\n', 1);
    ft_putnbr_fd(2147483647, 1);
    ft_putchar_fd('\n', 1);
    ft_putnbr_fd(-2147483648, 1);
    ft_putchar_fd('\n', 1);
}
 */