/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:02:54 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/31 18:30:29 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*actuel;
	t_list	*temp;

	if (lst == NULL || del == NULL)
		return ;
	actuel = *lst;
	while (actuel)
	{
		temp = actuel->next;
		(*del)(actuel->content);
		free(actuel);
		actuel = temp;
	}
	*lst = NULL;
}
