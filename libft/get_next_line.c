/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:37:42 by iel-kher          #+#    #+#             */
/*   Updated: 2025/01/21 20:09:38 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_extract_line(int fd, char *stash)
{
	ssize_t		sz;
	char		*buffer;
	char		*new_stash;

	sz = 1;
	buffer = malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!buffer || !stash)
		return (free(buffer), free(stash), NULL);
	while (sz > 0 && !ft_strchr(stash, '\n'))
	{
		sz = read(fd, buffer, BUFFER_SIZE);
		if (sz < 0)
			return (free(buffer), free(stash), NULL);
		buffer[sz] = '\0';
		new_stash = ft_strjoin(stash, buffer);
		if (!new_stash)
			return (free(buffer), free(stash), NULL);
		free(stash);
		stash = new_stash;
	}
	free(buffer);
	return (stash);
}

static char	*ft_get_line(char *stash)
{
	char	*line;
	int		i;

	i = 0;
	if (!stash || !stash[0])
		return (NULL);
	while (stash[i] != '\n' && stash[i] != '\0')
		i++;
	line = ft_substr(stash, 0, i + 1);
	return (line);
}

static char	*ft_clean_stash(char *stash)
{
	int		i;
	int		len;
	char	*new_stash;

	i = 0;
	len = 0;
	while (stash[i] != '\n' && stash[i] != '\0')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	i++;
	len = ft_strlen(stash) - i;
	new_stash = ft_substr(stash, i, len);
	free(stash);
	return (new_stash);
}

void	check_null(char **stash)
{
	if (*stash && **stash == '\0')
	{
		free(*stash);
		*stash = NULL;
	}
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
	{
		stash = ft_strdup("");
	}
	if (!stash)
		return (NULL);
	stash = ft_extract_line(fd, stash);
	if (!stash)
		return (NULL);
	line = ft_get_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = ft_clean_stash(stash);
	check_null(&stash);
	return (line);
}
