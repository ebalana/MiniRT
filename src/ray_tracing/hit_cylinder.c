/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:38:21 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/19 11:39:41 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static int	is_within_height(t_cylinder_calc calc, double t)
{
	double	height_at_t;

	height_at_t = calc.oc_dot_axis + t * calc.ray_dot_axis;
	if (height_at_t >= -calc.height_limit && height_at_t <= calc.height_limit)
		return (1);
	return (0);
}

static double	check_cylinder_body(t_cylinder_calc calc, double *closest_t)
{
	double	discriminant;
	double	t1;
	double	t2;

	discriminant = calc.b * calc.b - 4 * calc.a * calc.c;
	if (discriminant < 0)
		return (*closest_t);
	t1 = (-calc.b - sqrt(discriminant)) / (2.0 * calc.a);
	t2 = (-calc.b + sqrt(discriminant)) / (2.0 * calc.a);
	if (t1 > 0.001 && is_within_height(calc, t1))
		*closest_t = t1;
	else if (t2 > 0.001 && is_within_height(calc, t2))
		*closest_t = t2;
	return (*closest_t);
}

static double	check_cap(t_cylinder cylinder, t_ray ray,
	t_cylinder_calc calc, double cap_height)
{
	double	temp_t;
	t_vec3	hit_point;
	t_vec3	to_hit;
	t_vec3	cap_center;

	if (fabs(calc.ray_dot_axis) <= 0.0001)
		return (-1.0);
	temp_t = (cap_height - calc.oc_dot_axis) / calc.ray_dot_axis;
	if (temp_t <= 0.001)
		return (-1.0);
	hit_point = vec_add(ray.origin, vec_scale(ray.direction, temp_t));
	cap_center = vec_add(cylinder.center, vec_scale(cylinder.axis, cap_height));
	to_hit = vec_sub(hit_point, cap_center);
	if (vec_dot(to_hit, to_hit) <= cylinder.radius * cylinder.radius)
		return (temp_t);
	return (-1.0);
}

static double	check_cylinder_caps(t_cylinder cylinder, t_ray ray,
	t_cylinder_calc calc, double closest_t)
{
	double	bottom_t;
	double	top_t;

	bottom_t = check_cap(cylinder, ray, calc, -calc.height_limit);
	top_t = check_cap(cylinder, ray, calc, calc.height_limit);
	if (bottom_t > 0 && (closest_t < 0 || bottom_t < closest_t))
		closest_t = bottom_t;
	if (top_t > 0 && (closest_t < 0 || top_t < closest_t))
		closest_t = top_t;
	return (closest_t);
}

double	hit_cylinder(t_cylinder cylinder, t_ray ray, double *t)
{
	t_cylinder_calc	calc;
	double			closest_t;

	calc = init_cylinder_calc(cylinder, ray);
	closest_t = -1.0;
	closest_t = check_cylinder_body(calc, &closest_t);
	closest_t = check_cylinder_caps(cylinder, ray, calc, closest_t);
	if (closest_t > 0)
	{
		*t = closest_t;
		return (closest_t);
	}
	return (-1.0);
}
