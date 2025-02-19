/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:08:20 by iel-kher          #+#    #+#             */
/*   Updated: 2024/11/03 15:07:48 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
	write(fd, "\n", 1);
}
/* 
#include <fcntl.h>
#include <stdio.h>
int main()
{
	// On alloue une chaine assez grande pour read
	char *s = malloc(sizeof(char) * 100);

	// Création et ouverture du fichier en read and write et les permissions
	int fd = open("hello.txt", O_RDWR | O_CREAT, 0644);

	// Ecrtiture dans le fichier avec write
    ft_putendl_fd("Hello World !", fd);
	printf("open return : %d\n", fd);

	//Reset le curseur au début du fichier
	lseek(fd, 0, SEEK_SET); 

	// Read 13 bytes du file fd et stockage dans ssz du nb de byte lu
	int sz = read(fd, s, 13);
	printf("read return : %d\n", sz);
	s[sz] = '\0';
	printf("%s\n", s);

	// Close file pour ne pas le laisser ouvert
	int cls = close(fd);
	if(cls == 0)
		printf("closed\n");
	else
		printf("error\n");
}
 */