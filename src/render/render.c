/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:54:50 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 18:35:38 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

int rgb_to_mlx_color(double r, double g, double b)
{
	r = pow(r, 1.0/2.0);
	g = pow(g, 1.0/2.0);
	b = pow(b, 1.0/2.0);
	int ir = (int)(255.999 * r);
	int ig = (int)(255.999 * g);
	int ib = (int)(255.999 * b);
	if (ir > 255) ir = 255; if (ir < 0) ir = 0;
	if (ig > 255) ig = 255; if (ig < 0) ig = 0;
	if (ib > 255) ib = 255; if (ib < 0) ib = 0;

	return (ir << 24) | (ig << 16) | (ib << 8) | 0xFF;
}

void	render_pixel(t_render_data *data, t_pixel *pixel, t_viewport *viewport)
{
	t_vec3	direction;
	t_ray	ray;
	t_vec3	color;
	int		mlx_color;

	direction = vec_add(vec_add(viewport->lower_left_corner, \
				vec_scale(viewport->horizontal, pixel->u)), \
				vec_scale(viewport->vertical, pixel->v));
	direction = vec_sub(direction, viewport->origin);
	ray.origin = viewport->origin;
	ray.direction = vec_normalize(direction);	
	color = ray_color(ray, data->scene);
	mlx_color = rgb_to_mlx_color(color.x, color.y, color.z);
	mlx_put_pixel(data->img, pixel->i, HEIGHT - 1 - pixel->j, mlx_color);
}

void	render_rows(t_render_data *data, int start, int end, t_viewport *view)
{
	int		j;
	int		i;
	t_pixel	pixel;

	j = HEIGHT - 1 - start;
	while (j >= HEIGHT - end)
	{
		i = 0;
		while (i < WIDTH)
		{
			pixel.i = i;
			pixel.j = j;
			pixel.u = (double)i / (WIDTH - 1);
			pixel.v = (double)j / (HEIGHT - 1);
			render_pixel(data, &pixel, view);
			i++;
		}
		j--;
	}
}

void	render_scene(void *param)
{
	int				start_row;
	int				end_row;
	t_render_data	*data;
	t_viewport		viewport;
	double			aspect_ratio;

	data = (t_render_data *)param;
	if (data->rendering_complete)
		return ;
	get_render_rows(data->current_row, 5, &start_row, &end_row);
	aspect_ratio = (double)WIDTH / HEIGHT;
	get_camera_vectors(&data->scene->camera, aspect_ratio, &viewport);
	render_rows(data, start_row, end_row, &viewport);
	update_render_progress(data, end_row);
}
