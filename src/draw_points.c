/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_points.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:37:09 by gajanvie          #+#    #+#             */
/*   Updated: 2025/12/23 22:57:45 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

void	draw_every_point(t_data *data)
{
	int		screen_y = 0;
	int		screen_x;
	long	universe_x;
	long	universe_y;
	int		color;
	int 	border_size = 50;

	while (screen_y < HEIGHT)
	{
		screen_x = 0;
		while (screen_x < WIDTH)
		{
			if (data->map.zoom > 0)
			{
				universe_x = data->map.cam_x + (screen_x / data->map.zoom);
				universe_y = data->map.cam_y + (screen_y / data->map.zoom);
			}
			else
			{
				universe_x = data->map.cam_x + (screen_x * -data->map.zoom);
				universe_y = data->map.cam_y + (screen_y * -data->map.zoom);
			}
			if (universe_x >= 0 && universe_x < UNIVER_W &&
				universe_y >= 0 && universe_y < UNIVER_H)
			{
				if (get_bit(data->map, universe_y, universe_x))
					color = 0xFFFFFFFF;
				else
					color = 0x000000FF;
			}
			else if (universe_x >= -border_size && universe_x < UNIVER_W + border_size &&
					 universe_y >= -border_size && universe_y < UNIVER_H + border_size)
			{
				color = 0x444444FF;
			}
			else
			{
				unsigned long hash = data->map.seed;
				hash ^= (universe_x * 222222222222225UL);
				hash ^= (universe_y * 404040404040407UL);
				if (hash % 1001 == 0)
					color = 0xFFFFFFFF;
				else
					color = 0x000000FF;
			}
			data->img.pixels[screen_y * WIDTH + screen_x].rgba = color;
			screen_x++;
		}
		screen_y++;
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
				if (get_bit(data->map, row, col) == 1)
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
	pthread_t		threads[THREADS_COUNT];
	t_thread_info	infos[THREADS_COUNT];
	int				i;
	int				slice_height;
	unsigned char	**tmp;

	slice_height = data->map.height / THREADS_COUNT;
	i = 0;
	while (i < THREADS_COUNT)
	{
		infos[i].data = data;
		infos[i].start_y = i * slice_height;
		if (i == THREADS_COUNT - 1)
			infos[i].end_y = data->map.height;
		else
			infos[i].end_y = (i + 1) * slice_height;
		if (pthread_create(&threads[i], NULL, thread_routine, &infos[i]) != 0)
			return ;
		i++;
	}
	i = 0;
	while (i < THREADS_COUNT)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	tmp = data->map.grid;
	data->map.grid = data->map.next_grid;
	data->map.next_grid = tmp;
}
