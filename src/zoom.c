/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:39:23 by titan             #+#    #+#             */
/*   Updated: 2025/12/19 18:47:28 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

void resize_map(t_data *data, int new_scale, int mouse_x, int mouse_y)
{
	unsigned char   **new_grid;
	unsigned char   **new_next_grid;
	int             new_width;
	int             new_height;
	int             new_byte_width;
	int             offset_x;
	int             offset_y;
	int             y, x;

	new_width = WIDTH / new_scale;
	new_height = HEIGHT / new_scale;
	new_byte_width = (new_width / 8) + 1;
	new_grid = malloc(new_height * sizeof(unsigned char *));
	new_next_grid = malloc(new_height * sizeof(unsigned char *));
	if (!new_grid || !new_next_grid)
		return ;
	y = 0;
	while (y < new_height)
	{
		new_grid[y] = ft_calloc(new_byte_width, sizeof(unsigned char));
		new_next_grid[y] = ft_calloc(new_byte_width, sizeof(unsigned char));
		y++;
	}
	int cell_mouse_old_x = mouse_x / data->img.pixel_scale;
	int cell_mouse_old_y = mouse_y / data->img.pixel_scale;
	int cell_mouse_new_x = mouse_x / new_scale;
	int cell_mouse_new_y = mouse_y / new_scale;
	offset_x = cell_mouse_new_x - cell_mouse_old_x;
	offset_y = cell_mouse_new_y - cell_mouse_old_y;
	y = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (get_bit(data, y, x))
			{
				int new_y = y + offset_y;
				int new_x = x + offset_x;
				if (new_y >= 0 && new_y < new_height && 
					new_x >= 0 && new_x < new_width)
					new_grid[new_y][new_x / 8] |= (1 << (new_x % 8));
			}
			x++;
		}
		y++;
	}
	free_grids_only(data);
	data->map.grid = new_grid;
	data->map.next_grid = new_next_grid;
	data->map.width = new_width;
	data->map.height = new_height;
	data->map.byte_width = new_byte_width;
	data->img.pixel_scale = new_scale;
}

void mouse_hook(int button, void *param)
{
	t_data *data = (t_data *)param;
	int new_scale = data->img.pixel_scale;
	int     mouse_x;
    int     mouse_y;

    mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
	if (button == 1)
		new_scale++;
	else if (button == 2)
		new_scale--;
	if (new_scale < 1) new_scale = 1; 
	if (new_scale > 100) new_scale = 100;
	if (new_scale != data->img.pixel_scale)
	{
		resize_map(data, new_scale, mouse_x, mouse_y);
		mlx_clear_window(data->mlx, data->win, (mlx_color){0});
		draw_every_point(data);
		mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	}
}
