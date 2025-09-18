/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:18:34 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 18:35:02 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	update_render_progress(t_render_data *data, int end_row)
{
	data->current_row = end_row;
	if (data->current_row % 20 == 0)
		printf("Rendering... %d%%\n", (data->current_row * 100) / HEIGHT);
	if (data->current_row >= HEIGHT)
	{
		data->rendering_complete = 1;
		printf("Rendering complete!\n");
	}
}

static t_vec3	get_camera_up(t_vec3 forward)
{
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;

	world_up = vec3(0, 1, 0);
	if (fabs(vec_dot(forward, world_up)) > 0.9)
		world_up = vec3(1, 0, 0);
	right = vec_normalize(vec_cross(forward, world_up));
	up = vec_cross(right, forward);
	return (up);
}

void	get_camera_vectors(t_camera *camera, double aspect, t_viewport *view)
{
	double	theta;
	double	half_height;
	double	half_width;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	theta = camera->fov * M_PI / 180.0;
	half_height = tan(theta / 2.0);
	half_width = aspect * half_height;	
	forward = vec_normalize(camera->direction);
	up = get_camera_up(forward);
	right = vec_normalize(vec_cross(forward, up));
	up = vec_cross(right, forward);	
	view->origin = camera->position;
	view->horizontal = vec_scale(right, 2.0 * half_width);
	view->vertical = vec_scale(up, 2.0 * half_height);
	view->lower_left_corner = vec_sub(vec_sub(vec_add(camera->position, forward),
		vec_scale(view->horizontal, 0.5)), vec_scale(view->vertical, 0.5));
}

void	get_render_rows(int row, int rowsxframe, int *start_row, int *end_row)
{
	*start_row = row;
	*end_row = row + rowsxframe;
	if (*end_row > HEIGHT)
		*end_row = HEIGHT;
}
