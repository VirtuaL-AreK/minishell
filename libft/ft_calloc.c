/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:06:13 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/27 15:01:45 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*mem;

	if (size && nmemb > __SIZE_MAX__ / size)
		return (NULL);
	mem = (void *)malloc(nmemb * size);
	if (!mem)
		return (NULL);
	ft_bzero(mem, (nmemb * size));
	return (mem);
}
/* 
#include <stdio.h>
int main()
{
	char *ptr = ft_calloc(0, 1);
	char *ptr2 = calloc(0, 1);

	printf("%p\n", ptr);
	printf("%d\n", ptr[0]);
	printf("%p\n", ptr2);
	printf("%d\n", ptr2[0]);
}
 */