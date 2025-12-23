/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jdlv.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 11:13:19 by gajanvie          #+#    #+#             */
/*   Updated: 2025/12/23 13:33:03 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JDLV_H
# define JDLV_H

# include <ft_printf.h>
# include <libft.h>
# include <get_next_line.h>
# include <stdlib.h>
# include <fcntl.h> 
# include <errno.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <mlx.h>
# include <math.h>
# include <mlx_extended.h>
# include <pthread.h>

# define WIDTH       1500
# define HEIGHT      1000
# define SCALE       10
# define UNIVER_W  10000 
# define UNIVER_H  5000
# define THREADS_COUNT 8

typedef struct s_img
{
	void		*img;
	mlx_color	pixels[HEIGHT * WIDTH];
	int     	pixel_scale;
}				t_img;

typedef struct s_map
{
	unsigned char	**grid;
	unsigned char	**next_grid;
	int				width;
	int				height;
	int				byte_width;
	long			cam_x;
	long			cam_y;
	int             zoom;
}				t_map;

typedef struct s_data
{
	mlx_context	mlx;
	void		*win;
	t_img		img;
	t_map		map;
	int			is_paused;
	int			key_table[512];
	int			old_key_table[512];
	int			timer;
	int			speed;
	int			tor;
}				t_data;

typedef struct s_thread_info
{
	t_data	*data;
	int		start_y;
	int		end_y;
}				t_thread_info;

void	draw_every_point(t_data *data);
void	calculate_next_gen(t_data *data);
void	set_bit(unsigned char **grid, int y, int x, int state);
int		get_bit(t_map map, int y, int x);
void	free_grids_only(t_map map);
void	mouse_hook(int button, void *param);
void	mouse_down(int button, void *param);
void	save_map(t_map map);
int		load_binary_map(char *filename, t_data *data);
void	init_universe(t_data *data);
int		count_neighbors(t_data *data, int y, int x);
void	*thread_routine(void *arg);
void	safe_close(int fd);

#endif
