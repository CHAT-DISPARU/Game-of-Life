/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:39:23 by titan             #+#    #+#             */
/*   Updated: 2025/12/24 17:39:58 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

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
	int		n;

	if (data->shift > 0)
		n = 5;
	else
		n = 1;
	mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
	mouse_univ_x = data->map.cam_x + convert_zoom(mouse_x, data->map.zoom);
	mouse_univ_y = data->map.cam_y + convert_zoom(mouse_y, data->map.zoom);
	if (button == 1)
	{
		if (data->map.zoom == -1)
			data->map.zoom = 1;
		else
			data->map.zoom += n;
	}
	else if (button == 2)
	{
		if (data->map.zoom == 1)
			data->map.zoom = -1;
		else
			data->map.zoom -= n;
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
		set_bit(data, universe_y, universe_x, !current_state);
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
