/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:39:23 by titan             #+#    #+#             */
/*   Updated: 2025/12/23 13:39:35 by titan            ###   ########.fr       */
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
			if (get_bit(data->map, y, x))
			{
				int new_y = y + offset_y;
				int new_x = x + offset_x;
				if (new_y >= 0 && new_y < new_height && 
					new_x >= 0 && new_x < new_width)
					set_bit(new_grid, new_y, new_x, 1);
			}
			x++;
		}
		y++;
	}
	free_grids_only(data->map);
	data->map.grid = new_grid;
	data->map.next_grid = new_next_grid;
	data->map.width = new_width;
	data->map.height = new_height;
	data->map.byte_width = new_byte_width;
	data->img.pixel_scale = new_scale;
}

long	convert_zoom(int screen_coord, int zoom)
{
	if (zoom > 0)
		return (screen_coord / zoom);
	else
		return (screen_coord * -zoom);
}

void	mouse_hook(int button, void *param)
{
	t_data	*data = (t_data *)param;
	int		mouse_x, mouse_y;
	long	mouse_univ_x, mouse_univ_y;

	mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
	mouse_univ_x = data->map.cam_x + convert_zoom(mouse_x, data->map.zoom);
	mouse_univ_y = data->map.cam_y + convert_zoom(mouse_y, data->map.zoom);
	if (button == 1)
	{
		if (data->map.zoom == -1)
			data->map.zoom = 1;
		else
			data->map.zoom++;
	}
	else if (button == 2)
	{
		if (data->map.zoom == 1)
			data->map.zoom = -1;
		else
			data->map.zoom--;
	}
	data->map.cam_x = mouse_univ_x - convert_zoom(mouse_x, data->map.zoom);
	data->map.cam_y = mouse_univ_y - convert_zoom(mouse_y, data->map.zoom);
}

void	toggle_cell(t_data *data)
{
	int		mouse_x;
	int		mouse_y;
	long	universe_x;
	long	universe_y;
	int		current_state;

	mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
	universe_x = data->map.cam_x + (mouse_x / data->map.zoom);
	universe_y = data->map.cam_y + (mouse_y / data->map.zoom);
	if (universe_x >= 0 && universe_x < data->map.width &&
		universe_y >= 0 && universe_y < data->map.height)
	{
		current_state = get_bit(data->map, universe_y, universe_x);
		set_bit(data->map.grid, universe_y, universe_x, !current_state);
		mlx_clear_window(data->mlx, data->win, (mlx_color){0});
		draw_every_point(data);
		mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	}
}

void	mouse_down(int button, void *param)
{
	t_data *data = (t_data *)param;
	(void)button;
	toggle_cell(data);
}
