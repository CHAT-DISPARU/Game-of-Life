/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 11:37:44 by titan             #+#    #+#             */
/*   Updated: 2025/12/23 13:39:47 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

int load_binary_map(char *filename, t_data *data)
{
	int             fd;
	int             file_width;
	int             file_height;
	int             file_byte_width;
	int             offset_x;
	int             offset_y;
	int             x, y;
	unsigned char   *buffer;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_printf("Erreur: Impossible d'ouvrir %s\n", filename), 0);
	if (read(fd, &file_width, sizeof(int)) != sizeof(int) ||
		read(fd, &file_height, sizeof(int)) != sizeof(int))
	{
		safe_close(fd);
		return (ft_printf("Erreur: Header corrompu\n"), 0);
	}
	init_universe(data);
	offset_x = (UNIVER_W - file_width) / 2;
	offset_y = (UNIVER_H - file_height) / 2;
	file_byte_width = (file_width / 8) + 1;
	buffer = malloc(file_byte_width * sizeof(unsigned char));
	if (!buffer)
	{
		safe_close(fd);
		return (ft_printf("Erreur: Malloc buffer\n"), 0);
	}

	y = 0;
	while (y < file_height)
	{
		if (read(fd, buffer, file_byte_width) != file_byte_width)
		{
			free(buffer);
			safe_close(fd);
			return (ft_printf("Erreur: Lecture données ligne %d\n", y), 0);
		}
		x = 0;
		while (x < file_width)
		{
			if ((buffer[x / 8] >> (x % 8)) & 1)
			{
				if (y + offset_y < UNIVER_H && x + offset_x < UNIVER_W)
					set_bit(data->map.grid, y + offset_y, x + offset_x, 1);
			}
			x++;
		}
		y++;
	}
	free(buffer);
	safe_close(fd);
	ft_printf("Map chargée et centrée dans l'univers!!!!!\n");
	return (1);
}
