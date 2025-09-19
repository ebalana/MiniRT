/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:47:15 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/19 11:58:30 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

int	check_shadow(t_object object, t_ray shadow_ray, double light_distance)
{
	double	t;

	if (object.type == SPHERE)
	{
		if (hit_sphere(object.u_data.sphere, shadow_ray, &t) >= 0 && \
			t > 0.01 && t < (light_distance - 0.01))
			return (1);
	}
	else if (object.type == PLANE)
	{
		if (hit_plane(object.u_data.plane, shadow_ray, &t) >= 0 && \
			t > 0.01 && t < (light_distance - 0.01))
			return (1);
	}
	else if (object.type == CYLINDER)
	{
		if (hit_cylinder(object.u_data.cylinder, shadow_ray, &t) >= 0 && \
			t > 0.01 && t < (light_distance - 0.01))
			return (1);
	}
	return (0);
}

int	is_shadow(t_scene *scene, t_vec3 point, t_light light, t_vec3 surf_normal)
{
	double	light_distance;
	int		i;
	t_vec3	light_dir;
	t_ray	shadow_ray;

	light_dir = vec_sub(light.position, point);
	light_distance = vec_length(light_dir);
	light_dir = vec_normalize(light_dir);
	shadow_ray.origin = vec_add(point, vec_scale(surf_normal, 0.001));
	shadow_ray.direction = light_dir;
	i = 0;
	while (i < scene->object_count)
	{
		if (check_shadow(scene->objects[i], shadow_ray, light_distance))
			return (1);
		i++;
	}
	return (0);
}
