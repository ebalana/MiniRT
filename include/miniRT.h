/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:23:58 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/23 18:03:24 by ebalana-         ###   ########.fr       */
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

# define WIDTH 1900
# define HEIGHT 1280
# define M_PI 3.14159265358979323846

// main.c
void			validate_scene(t_scene *scene);
void			check_arguments(int argc, char **argv);
int				init_and_parse(int argc, char **argv, t_scene **scene);
int				init_and_render(t_scene *scene);

// init_utils.c
mlx_t			*init_mlx(void);
mlx_image_t		*create_image(mlx_t *mlx);
void			init_render_data(t_render_data *red, mlx_image_t *img,
					t_scene *scene);
void			display_image(mlx_t *mlx, mlx_image_t *img);
void			free_scene(t_scene *scene);

// hit_object.c
double			hit_sphere(t_sphere sphere, t_ray ray, double *t);
double			hit_plane(t_plane plane, t_ray ray, double *t);
double			hit_cylinder(t_cylinder cylinder, t_ray ray, double *t);

// hooks.c
void			key_hook(mlx_key_data_t keydata, void *param);

// shadow.c
int				check_shadow(t_object object, t_ray shadow_ray,
					double light_distance);
int				is_shadow(t_scene *scene, t_vec3 point, t_light light,
					t_vec3 surface_normal);

// error.c
void			ft_error(void);
// free.c
void			free_split(char **split);

// render.c
int				rgb_to_mlx_color(double r, double g, double b);
void			render_pixel(t_render_data *data, t_pixel *pixel,
					t_viewport *viewport);
void			render_rows(t_render_data *data, int start_row,
					int end_row, t_viewport *viewport);
void			render_scene(void *param);

//render_utils.c
void			update_render_progress(t_render_data *data, int end_row);
void			get_render_rows(int row, int rowsxframe,
					int *start_row, int *end_row);
void			get_camera_vectors(t_camera *camera,
					double aspect_ratio, t_viewport *viewport);

// ray_tracer.c
t_vec3			calculate_surface_normal(t_object object, t_vec3 hit_point);
int				find_closest_intersection(t_scene *scene,
					t_ray ray, double *closest_t);
t_vec3			ray_color(t_ray ray, t_scene *scene);

// lighting.c
t_vec3			clamp_color(t_vec3 color);
t_vec3			calculate_diffuse_lighting(t_scene *scene, t_vec3 hit_point,
					t_vec3 normal, t_vec3 object_color);
t_vec3			apply_lighting(t_scene *scene, t_object object,
					t_vec3 hit_point, t_vec3 normal);

//PARSING

// parse_vector.c
t_vec3			parse_vector(const char *str);
t_vec3			parse_color(const char *str);

// parse_components.c
void			parse_ambient(char **tokens, t_scene *scene);
void			parse_camera(char **tokens, t_scene *scene);
void			parse_light(char **tokens, t_scene *scene);

// parse_objects.c
void			parse_sphere(char **tokens, t_scene *scene);
void			parse_plane(char **tokens, t_scene *scene);
void			parse_cylinder(char **tokens, t_scene *scene);

// parse_scene.c
void			parse_line(const char *filename, t_scene *scene);

typedef struct s_cylinder_calc
{
	double	ray_dot_axis;
	double	oc_dot_axis;
	double	a;
	double	b;
	double	c;
	double	height_limit;
}	t_cylinder_calc;

t_cylinder_calc	init_cylinder_calc(t_cylinder cylinder, t_ray ray);

#endif