/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:04:38 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/29 12:04:39 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}
/* 
#include <fcntl.h>
int main()
{
	int fd = open("amine.text", O_WRONLY | O_CREAT , 0644);
    ft_putchar_fd('a', fd);
	int fd2 = open("amine1.text", O_WRONLY | O_CREAT , 0644);
    ft_putchar_fd('b', fd2);
	int fd3 = open("amine2.text", O_WRONLY | O_CREAT , 0644);
    ft_putchar_fd('c', fd3);
	int fd4 = open("amine3.text", O_WRONLY | O_CREAT , 0644);
    ft_putchar_fd('d', fd4);
	write(3, "_amine", 6);
	write(4, "_amine", 6);
	write(5, "_amine", 6);
	write(6, "_amine", 6);
}
 */