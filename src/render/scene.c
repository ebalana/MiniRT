/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:34:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 19:26:31 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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
	return (scene);
}

int	add_object(t_scene *scene, t_object obj)
{
	t_object	*new_objects;

	new_objects = realloc(scene->objects,
			sizeof(t_object) * (scene->object_count + 1));
	if (!new_objects)
		ft_error();
	scene->objects = new_objects;
	scene->objects[scene->object_count] = obj;
	scene->object_count++;
	return (0);
}
