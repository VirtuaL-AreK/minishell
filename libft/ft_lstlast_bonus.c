/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:50:51 by iel-kher          #+#    #+#             */
/*   Updated: 2024/11/01 23:40:09 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*actuel;

	if (lst == NULL)
		return (NULL);
	actuel = lst;
	while (actuel->next != NULL)
	{
		actuel = actuel->next;
	}
	return (actuel);
}
