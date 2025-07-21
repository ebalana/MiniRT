/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:23:58 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/10 16:44:48 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/ultimate_libft/ultimate_libft.h"
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <stdlib.h>
# include <errno.h>

# include "vec3.h"
# include "ray.h"
# include "scene.h"

#define WIDTH 1900
#define HEIGHT 1280

// init_utils.c
mlx_t		*init_mlx(void);
mlx_image_t	*create_image(mlx_t *mlx);
void		display_image(mlx_t *mlx, mlx_image_t *img);
void		free_scene(t_scene *scene);

// hit_object.c
double		hit_sphere(t_sphere sphere, t_ray ray, double *t);
double		hit_plane(t_plane plane, t_ray ray, double *t);
double		hit_cylinder(t_cylinder cylinder, t_ray ray, double *t);

// hooks.c
void		key_hook(mlx_key_data_t keydata, void *param);

// shadow.c
int			check_shadow(t_object object, t_ray shadow_ray, double light_distance);
int			is_shadow(t_scene *scene, t_vec3 point, t_light light, t_vec3 surface_normal);

// error.c
void		ft_error(void);

#endif