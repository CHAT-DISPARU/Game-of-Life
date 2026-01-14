/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 11:37:44 by titan             #+#    #+#             */
/*   Updated: 2025/12/24 16:51:30 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

int	load_binary_map(char *filename, t_data *data)
{
	int				fd;
	int				file_width;
	int				file_height;
	int				file_byte_width;
	int				offset_x;
	int				offset_y;
	int				x;
	int				current_y;
	unsigned char   *buffer;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_printf("Erreur: open map %s\n", filename), 0);
	if (read(fd, &file_width, sizeof(int)) != sizeof(int) ||
		read(fd, &file_height, sizeof(int)) != sizeof(int))
	{
		safe_close(fd);
		return (ft_printf("Erreur: Header \n"), 0);
	}
	init_universe(data);
	offset_x = (UNIVER_W - file_width) / 2;
	offset_y = (UNIVER_H - file_height) / 2;
	file_byte_width = (file_width / 8) + 1;
	buffer = malloc(file_byte_width * sizeof(unsigned char));
	if (!buffer)
	{
		safe_close(fd);
		return (ft_printf("Erreur: Malloc \n"), 0);
	}

	while (1)
	{
		if (read(fd, &current_y, sizeof(int)) != sizeof(int))
			break;
		if (current_y == -1)
			break;
		if (read(fd, buffer, file_byte_width) != file_byte_width)
			break;
		x = 0;
		while (x < file_width)
		{
			if ((buffer[x / 8] >> (x % 8)) & 1)
				set_bit(data, current_y + offset_y, x + offset_x, 1);
			x++;
		}
	}
	free(buffer);
	safe_close(fd);
	ft_printf("Map loaded!!!!!\n");
	return (1);
}
