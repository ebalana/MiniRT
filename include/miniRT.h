/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:23:58 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/08 15:56:26 by ebalana-         ###   ########.fr       */
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

# define SPHERES_COUNT 3

double hit_sphere(t_sphere sphere, t_ray ray, double *t);
double hit_plane(t_plane plane, t_ray ray, double *t);
int is_in_shadow(t_scene *scene, t_vec3 point, t_light light);

#endif