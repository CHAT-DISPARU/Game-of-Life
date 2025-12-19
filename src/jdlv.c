/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jdlv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 10:47:21 by gajanvie          #+#    #+#             */
/*   Updated: 2025/12/20 00:19:11 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

void	free_grids_only(t_data *data)
{
	int	i;

	i = 0;
	if (data->map.grid)
	{
		while (i < data->map.height)
		{
			if (data->map.grid[i]) free(data->map.grid[i]);
			if (data->map.next_grid[i]) free(data->map.next_grid[i]);
			i++;
		}
		free(data->map.grid);
		free(data->map.next_grid);
	}
}

int	get_bit(t_data *data, int y, int x)
{
	return ((data->map.grid[y][x / 8] >> (x % 8)) & 1);
}

void	set_bit(unsigned char **grid, int y, int x, int state)
{
	if (state == 1)
		grid[y][x / 8] |= (1 << (x % 8));
	else
		grid[y][x / 8] &= ~(1 << (x % 8));
}

void	create_window(t_data *data, mlx_window_create_info	*info)
{
	info->title = "Game Of Life";
	info->width = WIDTH;
	info->height = HEIGHT;
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, info);
	data->img.img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->speed = 5;
	data->timer = 0;
	data->img.pixel_scale = SCALE;
	data->is_paused = -1;
	data->tor = -1;
	ft_memset(data->key_table, 0, sizeof(data->key_table));
}

int	is_valid_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '0' && line[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

int parse_map(char *filename, t_data *data)
{
	int     fd;
	char    *line;
	int     file_width;
	int     file_height;
	int     offset_x; 
	int     offset_y;
	int     i, j;

	if (filename)
	{
		fd = open(filename, O_RDONLY);
		if (fd < 0)
			return (0);
		line = get_next_line(fd, 0);
		if (!line)
		{
			close(fd);
			return (0);
		}
		file_width = ft_strlen(line);
		if (line[file_width - 1] == '\n')
			file_width--;
		file_height = 0;
		while (line)
		{
			int current_len = ft_strlen(line);
			if (line[current_len - 1] == '\n')
				current_len--;
			if (current_len != file_width || !is_valid_line(line))
			{
				free(line);
				get_next_line(fd, 1);
				close(fd);
				return (0);
			}
			file_height++;
			free(line);
			line = get_next_line(fd, 0);
		}
		close(fd);
	}
	data->map.width = WIDTH / SCALE;
	data->map.height = HEIGHT / SCALE;
	data->map.byte_width = (data->map.width / 8) + 1;
	data->map.grid = malloc(data->map.height * sizeof(unsigned char *));
	data->map.next_grid = malloc(data->map.height * sizeof(unsigned char *));
	
	i = 0;
	while (i < data->map.height)
	{
		data->map.grid[i] = ft_calloc(data->map.byte_width, sizeof(unsigned char));
		data->map.next_grid[i] = ft_calloc(data->map.byte_width, sizeof(unsigned char));
		i++;
	}
	if (filename)
	{
		offset_x = (data->map.width - file_width) / 2;
		offset_y = (data->map.height - file_height) / 2;
		fd = open(filename, O_RDONLY);
		line = get_next_line(fd, 0);
		j = 0;
		while (line && j < file_height)
		{
			i = 0;
			while (i < file_width)
			{
				if (line[i] == '1')
					set_bit(data->map.grid, j + offset_y, i + offset_x, 1);
				i++;
			}
			free(line);
			line = get_next_line(fd, 0);
			j++;
		}
		close(fd);
	}
	return (1);
}

void free_struct(t_data *data, int j)
{
	int i = 0;
	while (i < data->map.height)
	{
		free(data->map.grid[i]);
		free(data->map.next_grid[i]);
		i++;
	}
	free(data->map.grid);
	free(data->map.next_grid);
	if (j)
	{
		mlx_destroy_image(data->mlx, data->img.img);
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_context(data->mlx);
	}
	free(data);
}

void	key_hook(int key, void *param)
{
	if (key == 41)
		mlx_loop_end((mlx_context)param);
}

void	key_up(int key, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	data->key_table[key] = 0;
}

void	key_down(int key, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	//printf("%d\n", key);
	data->key_table[key] = 1;
}

void	window_hook(int event, void *param)
{
	if (event == 0)
		mlx_loop_end((mlx_context)param);
}

void	fill_random(t_data *data)
{
	int	fd;
	int	y;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		perror("Erreur ouverture urandom");
		return;
	}
	y = 0;
	while (y < data->map.height)
	{
		if (read(fd, data->map.grid[y], data->map.byte_width) < 0)
			break;
		y++;
	}
	close(fd);
	mlx_clear_window(data->mlx, data->win, (mlx_color){0});
	draw_every_point(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}

void	clear_map(t_data *data)
{
	int	y;

	y = 0;
	while (y < data->map.height)
	{
		ft_bzero(data->map.grid[y], data->map.byte_width);
		ft_bzero(data->map.next_grid[y], data->map.byte_width);
		y++;
	}
	mlx_clear_window(data->mlx, data->win, (mlx_color){0});
	draw_every_point(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}

void	update(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (data->key_table[87] == 1 && data->speed > 1)
		data->speed--;
	if (data->key_table[86] == 1)
		data->speed++;
	if (data->key_table[44] == 1 && data->old_key_table[44] != 1)
	{
		if (data->is_paused > 0){ft_printf("game paused\n");}
		if (data->is_paused < 0){ft_printf("game active\n");}
		data->is_paused = -data->is_paused;
	}
	if (data->key_table[22] == 1 && data->old_key_table[22] == 0)
		fill_random(data);
	if (data->key_table[6] == 1 && data->old_key_table[6] == 0)
		clear_map(data);
	if (data->key_table[23] == 1 && data->old_key_table[23] != 1)
	{
		if (data->tor > 0){ft_printf("tor disabled\n");}
		if (data->tor < 0){ft_printf("tor enabled\n");}
		data->tor = -data->tor;
	}
	if (data->is_paused > 0)
	{
		data->timer++;
		if (data->timer >= data->speed)
		{
			calculate_next_gen(data); 
			data->timer = 0;
		}
	}
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0x000000FF});
	draw_every_point(data);
	mlx_set_image_region(data->mlx, data->img.img, 0, 0, WIDTH, HEIGHT, data->img.pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}

int	main(int ac, char **av)
{
	t_data					*data;
	mlx_window_create_info	info;

	if (ac > 2)
		ft_printf("./game_of_life <map>\n");
	data = malloc(sizeof(t_data));
	ft_memset(data, 0, sizeof(t_data));
	ft_memset(&info, 0, sizeof(mlx_window_create_info));
	if (ac == 1)
	{
		if (!parse_map(NULL, data))
		{
			free_struct(data, 0);
			ft_printf("Error from: %s\n", av[1]);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		if (!parse_map(av[1], data))
		{
			free_struct(data, 0);
			ft_printf("Error from: %s\n", av[1]);
			return (EXIT_FAILURE);
		}
	}
	printf("Parsing finit ...\n");
	create_window(data, &info);
	mlx_set_fps_goal(data->mlx, 60);
	mlx_on_event(data->mlx, data->win, MLX_MOUSEWHEEL, mouse_hook, data);
	mlx_on_event(data->mlx, data->win, MLX_MOUSEDOWN, mouse_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_hook, data->mlx);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYUP, key_up, data);
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT,
		window_hook, data->mlx);
	mlx_add_loop_hook(data->mlx, update, data);
	mlx_loop(data->mlx);
	free_struct(data, 1);
	return (0);
}
