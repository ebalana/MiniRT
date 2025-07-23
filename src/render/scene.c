/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:34:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/23 17:10:01 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Crear sphere
t_object	create_sphere(t_vec3 center, double radius, t_vec3 color)
{
	t_object	obj;

	obj.type = SPHERE;
	obj.color = color;
	obj.data.sphere.center = center;
	obj.data.sphere.radius = radius;
	obj.data.sphere.color = color;
	return (obj);
}

// Crear plane
t_object	create_plane(t_vec3 point, t_vec3 normal, t_vec3 color)
{
	t_object	obj;

	obj.type = PLANE;
	obj.color = color;
	obj.data.plane.point = point;
	obj.data.plane.normal = vec_normalize(normal);
	obj.data.plane.color = color;
	return (obj);
}

// Crear cylinder
t_object	create_cylinder(t_vec3 center, t_vec3 axis, double radius, double height, t_vec3 color)
{
	t_object	obj;

	obj.type = CYLINDER;
	obj.color = color;
	obj.data.cylinder.center = center;
	obj.data.cylinder.axis = vec_normalize(axis);
	obj.data.cylinder.radius = radius;
	obj.data.cylinder.height = height;
	obj.data.cylinder.color = color;
	return (obj);
}

// Inicializar escena
t_scene	*init_scene(void)
{
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	scene->objects = NULL;
	scene->object_count = 0;
	scene->lights = NULL;
	scene->light_count = 0;
	scene->ambient_ratio = 0.5;
	scene->ambient_color = vec3(1.0, 1.0, 1.0);
	return (scene);
}

// Añadir objeto a la escena
int	add_object(t_scene *scene, t_object obj)
{
	t_object	*new_objects;

	new_objects = realloc(scene->objects,
			sizeof(t_object) * (scene->object_count + 1));
	if (!new_objects)
		return (-1);
	scene->objects = new_objects;
	scene->objects[scene->object_count] = obj;
	scene->object_count++;
	printf("Objects count: %d\n", scene->object_count);
	return (0);
}
