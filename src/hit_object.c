/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:26 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/22 13:43:19 by ebalana-         ###   ########.fr       */
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

double hit_cylinder(t_cylinder cylinder, t_ray ray, double *t)
{
    double closest_t = -1.0;
    double temp_t;
    
    // Vector desde centro del cilindro al origen del rayo
    t_vec3 oc = vec_sub(ray.origin, cylinder.center);
    
    // Proyección del rayo y oc sobre el eje del cilindro
    double ray_dot_axis = vec_dot(ray.direction, cylinder.axis);
    double oc_dot_axis = vec_dot(oc, cylinder.axis);
    
    // 1. SUPERFICIE LATERAL
    // Componentes perpendiculares al eje
    t_vec3 ray_perp = vec_sub(ray.direction, vec_scale(cylinder.axis, ray_dot_axis));
    t_vec3 oc_perp = vec_sub(oc, vec_scale(cylinder.axis, oc_dot_axis));
    
    // Ecuación cuadrática para la superficie lateral
    double a = vec_dot(ray_perp, ray_perp);
    double b = 2.0 * vec_dot(oc_perp, ray_perp);
    double c = vec_dot(oc_perp, oc_perp) - (cylinder.radius * cylinder.radius);
    
    double discriminant = b * b - 4 * a * c;
    double height_limit = cylinder.height / 2.0;
    
    if (discriminant >= 0)
    {
        double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
        
        // Verificar t1
        if (t1 > 0.001)
        {
            double height_at_t1 = oc_dot_axis + t1 * ray_dot_axis;
            if (height_at_t1 >= -height_limit && height_at_t1 <= height_limit)
            {
                closest_t = t1;
            }
        }
        
        // Verificar t2 si t1 no es válido
        if (closest_t < 0 && t2 > 0.001)
        {
            double height_at_t2 = oc_dot_axis + t2 * ray_dot_axis;
            if (height_at_t2 >= -height_limit && height_at_t2 <= height_limit)
            {
                closest_t = t2;
            }
        }
    }
    
    // 2. BASE INFERIOR (cap at -height/2)
    if (fabs(ray_dot_axis) > 0.0001)
    {
        temp_t = (-height_limit - oc_dot_axis) / ray_dot_axis;
        if (temp_t > 0.001)
        {
            t_vec3 hit_point = vec_add(ray.origin, vec_scale(ray.direction, temp_t));
            t_vec3 to_hit = vec_sub(hit_point, vec_add(cylinder.center, vec_scale(cylinder.axis, -height_limit)));
            if (vec_dot(to_hit, to_hit) <= cylinder.radius * cylinder.radius)
            {
                if (closest_t < 0 || temp_t < closest_t)
                    closest_t = temp_t;
            }
        }
    }
    
    // 3. BASE SUPERIOR (cap at +height/2)
    if (fabs(ray_dot_axis) > 0.0001)
    {
        temp_t = (height_limit - oc_dot_axis) / ray_dot_axis;
        if (temp_t > 0.001)
        {
            t_vec3 hit_point = vec_add(ray.origin, vec_scale(ray.direction, temp_t));
            t_vec3 to_hit = vec_sub(hit_point, vec_add(cylinder.center, vec_scale(cylinder.axis, height_limit)));
            if (vec_dot(to_hit, to_hit) <= cylinder.radius * cylinder.radius)
            {
                if (closest_t < 0 || temp_t < closest_t)
                    closest_t = temp_t;
            }
        }
    }
    
    if (closest_t > 0)
    {
        *t = closest_t;
        return (closest_t);
    }
    
    return (-1.0);
}