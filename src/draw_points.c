/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_points.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:37:09 by gajanvie          #+#    #+#             */
/*   Updated: 2025/12/19 18:30:05 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

void	draw_every_point(t_data *data)
{
	int	y_map;
	int	x_map;
	int	y_screen;
	int	x_screen;
	int	color;

	y_map = 0;
	while (y_map < data->map.height)
	{
		x_map = 0;
		while (x_map < data->map.width)
		{
			if (get_bit(data, y_map, x_map) == 1)
				color = 0xFFFFFFFF;
			else
				color = 0x000000FF;
			int start_y = y_map * data->img.pixel_scale;
			int start_x = x_map * data->img.pixel_scale;
			y_screen = start_y;
			while (y_screen < start_y + data->img.pixel_scale)
			{
				x_screen = start_x;
				while (x_screen < start_x + data->img.pixel_scale)
				{
					if (x_screen < WIDTH && y_screen < HEIGHT)
						data->img.pixels[y_screen * WIDTH + x_screen].rgba = color;
					x_screen++;
				}
				y_screen++;
			}
			x_map++;
		}
		y_map++;
	}
}


int	count_neighbors(t_data *data, int y, int x)
{
	int	count;
	int	i;
	int	j;
	int	col;
	int	row;

	i = -1;
	count = 0;
	while (i <= 1)
	{
		j = -1;
		while (j <= 1)
		{
			if (i == 0 && j == 0)
			{
				j++;
				continue;
			}
			row = y + i;
			col = x + j;
			if (data->tor == 1)
			{
				row = (y + i + data->map.height) % data->map.height;
				col = (x + j + data->map.width) % data->map.width;
			}
			if (row >= 0 && row < data->map.height && 
				col >= 0 && col < data->map.width)
			{
				if (get_bit(data, row, col) == 1)
					count++;
			}
			j++;
		}
		i++;
	}
	return (count);
}

void	calculate_next_gen(t_data *data)
{
	int				y;
	int				x;
	int				autour;
	unsigned char	**tmp;
	int				state;

	y = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			autour = count_neighbors(data, y, x);
			state = get_bit(data, y, x);
			if (state == 0 && autour == 3)
				set_bit(data->map.next_grid, y, x, 1);
			else if (state == 1 && (autour < 2 || autour > 3))
				set_bit(data->map.next_grid, y, x, 0);
			else
				set_bit(data->map.next_grid, y, x, state);

			x++;
		}
		y++;
	}
	tmp = data->map.grid;
	data->map.grid = data->map.next_grid;
	data->map.next_grid = tmp;
}
