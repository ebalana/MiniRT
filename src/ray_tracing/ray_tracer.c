/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:55:29 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/23 16:45:57 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
** calculate_surface_normal - Calcula la normal de la superficie en el punto de intersección
**
** Descripción:
** Determina la normal de la superficie del objeto en el punto donde el rayo
** intersecta. Maneja diferentes tipos de objetos: esferas, planos y cilindros.
**
** Parámetros:
** - object: Objeto intersectado
** - hit_point: Punto de intersección en el espacio 3D
**
** Retorna:
** - Vector normal unitario en el punto de intersección
*/
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
		normal = object.data.plane.normal;
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
	return (normal);
}

/*
** find_closest_intersection - Encuentra el objeto más cercano que intersecta con el rayo
**
** Descripción:
** Itera por todos los objetos de la escena y encuentra cuál es el más cercano
** que intersecta con el rayo dado. Maneja esferas, planos y cilindros.
**
** Parámetros:
** - scene: Escena con todos los objetos
** - ray: Rayo a verificar intersecciones
** - closest_t: Puntero donde almacenar la distancia más cercana
**
** Retorna:
** - Índice del objeto más cercano, o -1 si no hay intersecciones
*/
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

/*
** ray_color - Calcula el color final de un rayo lanzado en la escena
**
** Descripción:
** Función principal del algoritmo de ray tracing que determina el color
** que debe mostrar un píxel. Encuentra intersecciones, calcula normales
** y aplica el modelo de iluminación para obtener el color final.
**
** Parámetros:
** - ray: Rayo lanzado desde la cámara hacia la escena
** - scene: Escena completa con objetos, luces y configuración
**
** Funcionamiento:
** 1. Busca la intersección más cercana del rayo con los objetos
** 2. Si no hay intersecciones, retorna color de fondo (negro)
** 3. Calcula el punto de intersección en el espacio 3D
** 4. Determina la normal de la superficie en ese punto
** 5. Aplica el modelo de iluminación (ambiental + difusa + sombras)
** 6. Retorna el color final calculado
**
** Retorna:
** - Vector color RGB (0.0-1.0) representando el color final del píxel
** - Negro (0,0,0) si el rayo no intersecta ningún objeto
**
** Nota: Esta es la función core del ray tracing que determina la apariencia visual
*/
t_vec3 ray_color(t_ray ray, t_scene *scene)
{
	double closest_t;
	int closest_object;

	closest_object = find_closest_intersection(scene, ray, &closest_t);	
	
	// Si no hay intersecciones, return background black
	if (closest_object == -1)
		return vec3(0.0, 0.0, 0.0);
	
	// Calculate lighting for closest object
	t_vec3 hit_point = vec_add(ray.origin, vec_scale(ray.direction, closest_t));
	t_vec3 normal = calculate_surface_normal(scene->objects[closest_object], hit_point);

	// Apply lighting
	t_vec3 final_color = apply_lighting(scene, scene->objects[closest_object], hit_point, normal);
	return (final_color);
}