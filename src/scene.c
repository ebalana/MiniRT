/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:34:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/03 16:36:22 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Crear objetos
t_object create_sphere(t_vec3 center, double radius, t_vec3 color)
{
	t_object obj;
	obj.type = SPHERE;
	obj.color = color;
	obj.data.sphere.center = center;
	obj.data.sphere.radius = radius;
	obj.data.sphere.color = color;
	return (obj);
}

t_object create_plane(t_vec3 point, t_vec3 normal, t_vec3 color)
{
	t_object obj;
	obj.type = PLANE;
	obj.color = color;
	obj.data.plane.point = point;
	obj.data.plane.normal = vec_normalize(normal);
	obj.data.plane.color = color;
	return (obj);
}

// Inicializar escena
t_scene *init_scene(void)
{
	t_scene *scene = malloc(sizeof(t_scene));
	if (!scene)
		return NULL;
	scene->objects = NULL;
	scene->object_count = 0;
	scene->lights = NULL;
	scene->light_count = 0;
	scene->ambient = vec3(0.1, 0.1, 0.1);  // Luz ambiente suave
	return (scene);
}

// Añadir objeto a la escena
int add_object(t_scene *scene, t_object obj)
{
	t_object *new_objects = realloc(scene->objects, 
		sizeof(t_object) * (scene->object_count + 1));
	if (!new_objects)
		return (-1);
	scene->objects = new_objects;
	scene->objects[scene->object_count] = obj;
	scene->object_count++;
	return (0);
}