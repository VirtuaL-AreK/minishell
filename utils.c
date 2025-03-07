#include "minishell.h"

void ft_free_args(char **args)
{
	int i = 0;

	while(args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// static int	ft_count_words(char const *str, char c)
// {
// 	int	i;
// 	int	count;

// 	i = 0;
// 	count = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == c)
// 			i++;
// 		else
// 		{
// 			count++;
// 			while (str[i] && str[i] != c)
// 				i++;
// 		}
// 	}
// 	return (count);
// }

// static int	ft_word_len(const char *s, char c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] && s[i] != c)
// 		i++;
// 	return (i);
// }

// static char	*ft_wordup(const char *str, char c)
// {
// 	int		wordlen;
// 	int		i;
// 	char	*word;

// 	i = 0;
// 	wordlen = ft_word_len(str, c);
// 	word = malloc(sizeof(char) * (wordlen + 1));
// 	if (!word)
// 		return (NULL);
// 	while (str[i] && str[i] != c)
// 	{
// 		word[i] = str[i];
// 		i++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

// static void	ft_freestr(char **str, int i)
// {
// 	while (i >= 0)
// 	{
// 		free(str[i]);
// 		i--;
// 	}
// 	free(str);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char	**strings;
// 	int		i;

// 	if (!s)
// 		return (0);
// 	i = 0;
// 	strings = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
// 	if (!strings)
// 		return (NULL);
// 	while (*s)
// 	{
// 		while (*s && *s == c)
// 			s++;
// 		if (*s)
// 		{
// 			strings[i] = ft_wordup(s, c);
// 			if (!strings[i])
// 				return (ft_freestr(strings, i), NULL);
// 			i++;
// 		}
// 		while (*s && *s != c)
// 			s++;
// 	}
// 	strings[i] = 0;
// 	return (strings);
// }