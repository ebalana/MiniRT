/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:07:20 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 18:52:59 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_ray	ray(t_vec3 origin, t_vec3 direction)
{
	t_ray	r;

	r.origin = origin;
	r.direction = direction;
	return (r);
}

t_vec3	ray_at(t_ray r, double t)
{
	return (vec_add(r.origin, vec_scale(r.direction, t)));
}

t_vec3	ray_color(t_ray ray, t_scene *scene)
{
	t_vec3	hit_point;
	t_vec3	normal;
	t_vec3	fn_clr;
	double	closest_t;
	int		cl_obj;

	cl_obj = find_closest_intersection(scene, ray, &closest_t);
	if (cl_obj == -1)
		return (vec3(0.0, 0.0, 0.0));
	hit_point = vec_add(ray.origin, vec_scale(ray.direction, closest_t));
	normal = calculate_surface_normal(scene->objects[cl_obj], hit_point);
	fn_clr = apply_lighting(scene, scene->objects[cl_obj], hit_point, normal);
	return (fn_clr);
}
