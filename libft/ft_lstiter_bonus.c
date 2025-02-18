/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:22:22 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/31 18:22:25 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*actuel;

	if (lst == NULL || f == NULL)
		return ;
	actuel = lst;
	while (actuel != NULL)
	{
		(*f)(actuel->content);
		actuel = actuel->next;
	}
}
