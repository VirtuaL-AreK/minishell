/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:51:07 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/30 16:51:08 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
/* 
#include <stdio.h>

int	main(void)
{
    char *content = "Hello, world!";

    t_list *new_node = ft_lstnew((void *)content);

    if (new_node == NULL)
    {
        printf("Erreur : Allocation mémoire échouée\n");
        return (1);
    }

    printf("Contenu de la node : %s\n", (char *)new_node->content);
    printf("Next de la node : %p\n", (void *)new_node->next);

    free(new_node);

    return (0);
}
*/