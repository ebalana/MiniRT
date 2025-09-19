/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:26 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/19 11:39:42 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static double	check_sphere_intersections(double discriminant, double a,
	double b, double *t)
{
	double	t1;
	double	t2;

	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	if (t1 > 0.001)
	{
		*t = t1;
		return (t1);
	}
	else if (t2 > 0.001)
	{
		*t = t2;
		return (t2);
	}
	return (-1.0);
}

double	hit_sphere(t_sphere sphere, t_ray ray, double *t)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	oc = vec_sub(ray.origin, sphere.center);
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - (sphere.radius * sphere.radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	return (check_sphere_intersections(discriminant, a, b, t));
}

double	hit_plane(t_plane plane, t_ray ray, double *t)
{
	double	denominator;
	t_vec3	p0_to_origin;

	denominator = vec_dot(plane.normal, ray.direction);
	if (fabs(denominator) < 0.0001)
		return (-1.0);
	p0_to_origin = vec_sub(plane.point, ray.origin);
	*t = vec_dot(p0_to_origin, plane.normal) / denominator;
	if (*t > 0.001)
		return (*t);
	return (-1.0);
}

t_cylinder_calc	init_cylinder_calc(t_cylinder cylinder, t_ray ray)
{
	t_cylinder_calc	calc;
	t_vec3			oc;
	t_vec3			ray_perp;
	t_vec3			oc_perp;

	oc = vec_sub(ray.origin, cylinder.center);
	calc.ray_dot_axis = vec_dot(ray.direction, cylinder.axis);
	calc.oc_dot_axis = vec_dot(oc, cylinder.axis);
	ray_perp = vec_sub(ray.direction, vec_scale(cylinder.axis,
				calc.ray_dot_axis));
	oc_perp = vec_sub(oc, vec_scale(cylinder.axis, calc.oc_dot_axis));
	calc.a = vec_dot(ray_perp, ray_perp);
	calc.b = 2.0 * vec_dot(oc_perp, ray_perp);
	calc.c = vec_dot(oc_perp, oc_perp) - (cylinder.radius * cylinder.radius);
	calc.height_limit = cylinder.height / 2.0;
	return (calc);
}
