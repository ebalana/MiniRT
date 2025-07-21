/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:41:47 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/21 16:21:13 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "miniRT.h"

// Tipos de objetos
typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER
} t_object_type;

// Estructura para la cámara
typedef struct s_camera
{
	t_vec3	position;
	t_vec3	direction;	// Hacia dónde mira
	t_vec3	up;			// Vector "arriba"
	double	fov;		// Campo de visión en grados
} t_camera;

// Estructura para la luz
typedef struct s_light
{
	t_vec3	position;
	t_vec3	color;		// RGB (0-1)
	double	intensity;	// 0-1
} t_light;

// Estructuras para nuevos objetos
typedef struct s_plane
{
	t_vec3	point;		// Punto en el plano
	t_vec3	normal;		// Vector normal
	t_vec3	color;
} t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;		// Dirección del eje
	double	radius;
	double	height;
	t_vec3	color;
} t_cylinder;

typedef struct s_sphere
{
	t_vec3	center;
	double	radius;
	t_vec3	color;
}	t_sphere;

// Objeto genérico (unión)
typedef struct s_object
{
	t_object_type	type;
	t_vec3			color;
	union {
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
	} data;
} t_object;

// Escena completa
typedef struct s_scene
{
	t_camera	camera;
	t_light		*lights;			// Array dinámico
	int			light_count;
	t_object	*objects;			// Array dinámico
	int			object_count;
	double		ambient_ratio;		// 0.2 (ratio)
	t_vec3		ambient_color;		// (1.0, 1.0, 1.0) color blanco
} t_scene;

typedef struct s_render_data
{
    mlx_image_t *img;
    t_scene *scene;
} t_render_data;


t_object create_sphere(t_vec3 center, double radius, t_vec3 color);
t_object create_plane(t_vec3 point, t_vec3 normal, t_vec3 color);
t_object create_cylinder(t_vec3 center, t_vec3 axis, double radius, double height, t_vec3 color);
t_scene *init_scene(void);
int add_object(t_scene *scene, t_object obj);




#endif