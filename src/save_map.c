/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:39:18 by titan             #+#    #+#             */
/*   Updated: 2025/12/23 13:39:39 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

void	save_map(t_map map)
{
	int		r;
	char	s;
	char	*result;
	int		i = 0;
	int		y = 0;
	int		fd;

	ft_putstr_fd("\nname of the save : ", 1);
	result = NULL;
	r = read(0, &s, 1);
	while (r > 0)
	{
		if (s != '\n')
		{
			if (!result)
			{
				result = ft_calloc(2, sizeof(char));
				if (!result)
					return (perror("Malloc fail :"));
				result[0] = s;
			}
			else
			{
				result = realloc(result, ft_strlen(result) + 2);
				if (!result)
					return (perror("Malloc fail :"));
				result[i] = s;
				result[i + 1] = '\0';
			}
			i++;
		}
		else
		{
			if (!result)
				return (ft_putstr_fd("invalid file name\ngame running...\n", 2));
			result = realloc(result, ft_strlen(result) + 5);
			if (!result)
				return (perror("Malloc fail :"));
			ft_strlcat(result, ".map", ft_strlen(result) + 5);
			fd = open(result, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0)
			{
				free(result);
				perror("Open fail : ");
				return ;
			}
			int w = UNIVER_W; 
			int h = UNIVER_H;
			write(fd, &w, sizeof(int));
			write(fd, &h, sizeof(int));
			while (y < UNIVER_H)
			{
				write(fd, map.grid[y], map.byte_width);
				y++;
			}
			free(result);
			safe_close(fd);
			ft_printf("file saved\ngame running...\n");
			return ;
		}
		r = read(0, &s, 1);
	}
	if (r < 0)
		perror("Read fail : ");
	if (result)
		free(result);
}
