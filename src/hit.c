/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:26 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/08 15:19:14 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

double hit_sphere(t_sphere sphere, t_ray ray, double *t)
{
	t_vec3 oc;
	double a;
	double b;
	double c;
	double discriminant;
	double t1, t2;

	oc = vec_sub(ray.origin, sphere.center);
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - (sphere.radius * sphere.radius);
	discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
		return (-1.0); // No intersection

	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);

	// Return the closest positive intersection
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

double hit_plane(t_plane plane, t_ray ray, double *t)
{
	// Calcular denominador (dot product entre normal y dirección del rayo)
	double denominator = vec_dot(plane.normal, ray.direction);

	// Si denominator es 0, el rayo es paralelo al plano (no intersecta)
	if (fabs(denominator) < 0.0001)
		return (-1.0);

	// Calcular vector desde origen del rayo hasta punto en el plano
	t_vec3 p0_to_origin = vec_sub(plane.point, ray.origin);

	// Calcular distancia t
	*t = vec_dot(p0_to_origin, plane.normal) / denominator;

	// Solo intersecciones positivas (delante de la cámara)
	if (*t > 0.001)
		return (*t);

	return (-1.0);
}
