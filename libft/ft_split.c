/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:56:27 by iel-kher          #+#    #+#             */
/*   Updated: 2024/11/01 23:53:48 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			i++;
		else
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (count);
}

static int	ft_word_len(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*ft_wordup(const char *str, char c)
{
	int		wordlen;
	int		i;
	char	*word;

	i = 0;
	wordlen = ft_word_len(str, c);
	word = malloc(sizeof(char) * (wordlen + 1));
	if (!word)
		return (NULL);
	while (str[i] && str[i] != c)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_freestr(char **str, int i)
{
	while (i >= 0)
	{
		free(str[i]);
		i--;
	}
	free(str);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	int		i;

	if (!s)
		return (0);
	i = 0;
	strings = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!strings)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			strings[i] = ft_wordup(s, c);
			if (!strings[i])
				return (ft_freestr(strings, i), NULL);
			i++;
		}
		while (*s && *s != c)
			s++;
	}
	strings[i] = 0;
	return (strings);
}
/* 
#include <stdio.h>

int main(void)
{
    char **result;
    char str[] = "Hello  World   42!";
    char delim = ' ';
    int i = 0;

    result = ft_split(str, delim);
    if (!result)
    {
        printf("Error in ft_split\n");
        return (1);
    }
    while (result[i])
    {
        printf("Word %d: %s\n", i + 1, result[i]);
        free(result[i]);
        i++;
    }
    free(result);
    return (0);
}
 */