/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:30:44 by iel-kher          #+#    #+#             */
/*   Updated: 2024/10/31 18:30:45 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*temp;
	void	*content;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		content = (*f)(lst->content);
		temp = ft_lstnew(content);
		if (temp == NULL)
		{
			del(content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, temp);
		lst = lst->next;
	}
	return (new_lst);
}
/* 
int main()
{
//Ajout des noeuds
	t_list *head = ft_lstnew(ft_strdup("World !"));
	t_list *new_node1 = ft_lstnew(ft_strdup("Hello"));
	t_list *new_node2 = ft_lstnew(ft_strdup("Yo"));
	t_list *back_node = ft_lstnew(ft_strdup("Back"));

	ft_lstadd_front(&head, new_node1);
	ft_lstadd_front(&head, new_node2);
	ft_lstadd_back(&head, back_node);

// Affichage des noeuds
	printf("\033[31m" "Liste :\n" "\033[0m");
	print_list(head);

// Affichage de la taille de la liste
	printf("size = %d\n", ft_lstsize(head));

//Affichage du dernier élément de la liste
	printf("last = %s\n", (char *)ft_lstlast(head)->content);

// Application d'une foncttion sur la liste
	ft_lstiter(head, ft_maj);
	printf("Liste après ft_lstiter : \n");
	print_list(head);

//Création et application d'une fonction
	t_list *new_lst = ft_lstmap(head, ft_min, del);
	printf("Liste après ft_lstmap : \n");
	print_list(new_lst);

// Suppression du noeud en décalant le head
	t_list *temp;
	temp = head;
	head = head->next; 
	ft_lstdelone(temp, del);
	printf("Liste après ft_lstdelone : \n");
	print_list(head);

// Clear list : suppression de tout les noeuds
	ft_lstclear(&head, del);
	printf("Liste après ft_lstclear : \n");
	print_list(head);
	return 0;
}
 */