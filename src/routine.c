/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:50:29 by titan             #+#    #+#             */
/*   Updated: 2025/12/24 16:15:18 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <jdlv.h>

void *thread_routine(void *arg)
{
	t_thread_info   *info = (t_thread_info *)arg;
	t_data          *data = info->data;
	int y = info->start_y;
	int byte_x;
	int bit;
	int global_x;
	unsigned char *row_top;
	unsigned char *row_mid;
	unsigned char *row_bot;
	unsigned char *row_next;

	while (y < info->end_y)
	{
		if (y == 0 || y == data->map.height - 1)
		{
			data->map.next_row_empty[y] = 1;
			y++;
			continue ; 
		}
		if (data->map.row_empty[y] && data->map.row_empty[y-1] && data->map.row_empty[y+1])
		{
			data->map.next_row_empty[y] = 1;
			long *ptr = (long *)data->map.next_grid[y];
			int k = 0;
			while (k < data->map.byte_width / 8)
				ptr[k++] = 0;
			y++;
			continue;
		}
		row_top = data->map.grid[y - 1];
		row_mid = data->map.grid[y];
		row_bot = data->map.grid[y + 1];
		row_next = data->map.next_grid[y];
		int current_row_is_empty = 1;
		byte_x = 0;
		while (byte_x < data->map.byte_width)
		{
			if (byte_x > 0 && byte_x + 8 < data->map.byte_width - 1)
			{
				if (*(uint64_t*)&row_mid[byte_x] == 0 && 
					*(uint64_t*)&row_top[byte_x] == 0 && 
					*(uint64_t*)&row_bot[byte_x] == 0 &&
					row_mid[byte_x - 1] == 0 && row_mid[byte_x + 8] == 0 &&
					row_top[byte_x - 1] == 0 && row_top[byte_x + 8] == 0 &&
					row_bot[byte_x - 1] == 0 && row_bot[byte_x + 8] == 0)
				{
					*(uint64_t*)&row_next[byte_x] = 0;
					byte_x += 8;
					continue ;
				}
			}
			if (byte_x > 0 && byte_x < data->map.byte_width - 1)
			{
				if (row_mid[byte_x] == 0 && row_top[byte_x] == 0 && row_bot[byte_x] == 0 &&
					row_mid[byte_x - 1] == 0 && row_top[byte_x - 1] == 0 && row_bot[byte_x - 1] == 0 &&
					row_mid[byte_x + 1] == 0 && row_top[byte_x + 1] == 0 && row_bot[byte_x + 1] == 0)
				{
					row_next[byte_x] = 0;
					byte_x++;
					continue ;
				}
			}
			unsigned char res_byte = 0;
			bit = 0;
			while (bit < 8)
			{
				global_x = byte_x * 8 + bit;
				if (global_x == 0 || global_x >= data->map.width - 1)
				{
					bit++;
					continue ;
				}
				int count = count_neighbors(data, y, global_x);
				int state = (row_mid[byte_x] >> bit) & 1;
				if (state == 0 && count == 3)
					res_byte |= (1 << bit);
				else if (state == 1 && (count == 2 || count == 3))
					res_byte |= (1 << bit);
				bit++;
			}
			row_next[byte_x] = res_byte;
			if (res_byte > 0)
				current_row_is_empty = 0;
			byte_x++;
		}
		data->map.next_row_empty[y] = current_row_is_empty;
		y++;
	}
	return (NULL);
}
