/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:55:29 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/19 11:58:12 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_vec3	get_cylinder_normal(t_cylinder cyl, t_vec3 hit_point)
{
	t_vec3	to_hit;
	double	projection;
	double	height_limit;

	to_hit = vec_sub(hit_point, cyl.center);
	projection = vec_dot(to_hit, cyl.axis);
	height_limit = cyl.height / 2.0;
	if (fabs(projection - height_limit) < 0.001)
		return (cyl.axis);
	else if (fabs(projection + height_limit) < 0.001)
		return (vec_scale(cyl.axis, -1.0));
	else
		return (vec_normalize(vec_sub(hit_point,
					vec_add(cyl.center, vec_scale(cyl.axis, projection)))));
}

t_vec3	calculate_surface_normal(t_object object, t_vec3 hit_point)
{
	t_vec3	sphere_normal;
	t_vec3	plane_normal;

	if (object.type == SPHERE)
	{
		sphere_normal = vec_normalize(vec_sub(hit_point,
					object.u_data.sphere.center));
		return (sphere_normal);
	}
	else if (object.type == PLANE)
	{
		plane_normal = vec_normalize(object.u_data.plane.normal);
		return (plane_normal);
	}
	else if (object.type == CYLINDER)
		return (get_cylinder_normal(object.u_data.cylinder, hit_point));
	return (vec3(0, 1, 0));
}

static int	check_object_intersection(t_object object, t_ray ray,
	double *closest_t, int i)
{
	double	t;

	if (object.type == SPHERE)
	{
		if (hit_sphere(object.u_data.sphere, ray, &t) >= 0)
		{
			if (*closest_t < 0 || t < *closest_t)
			{
				*closest_t = t;
				return (i);
			}
		}
	}
	return (-1);
}

static int	check_plane_cylinder_intersection(t_object object, t_ray ray,
	double *closest_t, int i)
{
	double	t;

	if (object.type == PLANE)
	{
		if (hit_plane(object.u_data.plane, ray, &t) >= 0)
		{
			if (*closest_t < 0 || t < *closest_t)
			{
				*closest_t = t;
				return (i);
			}
		}
	}
	else if (object.type == CYLINDER)
	{
		if (hit_cylinder(object.u_data.cylinder, ray, &t) >= 0)
		{
			if (*closest_t < 0 || t < *closest_t)
			{
				*closest_t = t;
				return (i);
			}
		}
	}
	return (-1);
}

int	find_closest_intersection(t_scene *scene, t_ray ray, double *closest_t)
{
	int	i;
	int	closest_object;
	int	temp_object;

	i = 0;
	closest_object = -1;
	*closest_t = -1.0;
	while (i < scene->object_count)
	{
		temp_object = check_object_intersection(scene->objects[i], ray,
				closest_t, i);
		if (temp_object >= 0)
			closest_object = temp_object;
		temp_object = check_plane_cylinder_intersection(scene->objects[i], ray,
				closest_t, i);
		if (temp_object >= 0)
			closest_object = temp_object;
		i++;
	}
	return (closest_object);
}
