/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:55:29 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 18:53:19 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	calculate_surface_normal(t_object object, t_vec3 hit_point)
{
	t_vec3		normal;
	t_cylinder	cyl;
	t_vec3		to_hit;
	double		projection;
	double		height_limit;

	normal = vec3(0, 1, 0);
	if (object.type == SPHERE)
	{
		normal = vec_normalize(vec_sub(hit_point, object.data.sphere.center));
	}
	else if (object.type == PLANE)
	{
		normal = vec_normalize(object.data.plane.normal);
	}
	else if (object.type == CYLINDER)
	{
		cyl = object.data.cylinder;
		to_hit = vec_sub(hit_point, cyl.center);
		projection = vec_dot(to_hit, cyl.axis);
		height_limit = cyl.height / 2.0;
		if (fabs(projection - height_limit) < 0.001)
			normal = cyl.axis;
		else if (fabs(projection + height_limit) < 0.001)
			normal = vec_scale(cyl.axis, -1.0);
		else
			normal = vec_normalize(vec_sub(hit_point, 
				vec_add(cyl.center, vec_scale(cyl.axis, projection))));
	}
	return (vec_normalize(normal));
}

int	find_closest_intersection(t_scene *scene, t_ray ray, double *closest_t)
{
	int		i;
	int		closest_object;
	double	t;

	i = 0;
	closest_object = -1;
	*closest_t = -1.0;
	while (i < scene->object_count)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (hit_sphere(scene->objects[i].data.sphere, ray, &t) >= 0)
			{
				if (*closest_t < 0 || t < *closest_t)
				{
					*closest_t = t;
					closest_object = i;
				}
			}
		}
		else if (scene->objects[i].type == PLANE)
		{
			if (hit_plane(scene->objects[i].data.plane, ray, &t) >= 0)
			{
				if (*closest_t < 0 || t < *closest_t)
				{
					*closest_t = t;
					closest_object = i;
				}
			}
		}
		else if (scene->objects[i].type == CYLINDER)
		{
			if (hit_cylinder(scene->objects[i].data.cylinder, ray, &t) >= 0)
			{
				if (*closest_t < 0 || t < *closest_t)
				{
					*closest_t = t;
					closest_object = i;
				}
			}
		}
		i++;
	}
	return (closest_object);
}
