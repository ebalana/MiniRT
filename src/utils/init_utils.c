/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:03 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 18:36:27 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

mlx_t	*init_mlx(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!mlx)
		ft_error();
	return (mlx);
}

mlx_image_t	*create_image(mlx_t *mlx)
{
	mlx_image_t	*img;

	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(mlx);
		ft_error();
	}
	return (img);
}

void	init_render_data(t_render_data *red, mlx_image_t *img, t_scene *scene)
{
	red->img = img;
	red->scene = scene;
	red->current_row = 0;
	red->rendering_complete = 0;
}

void	display_image(mlx_t *mlx, mlx_image_t *img)
{
	if (mlx_image_to_window(mlx, img, 0, 0) == -1)
	{
		mlx_terminate(mlx);
		ft_error();
	}
}

void	free_scene(t_scene *scene)
{
	if (scene)
	{
		if (scene->lights)
			free(scene->lights);
		if (scene->objects)
			free(scene->objects);
		free(scene);
	}
}
