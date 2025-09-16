/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:47:15 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/16 18:29:28 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
** check_shadow - Verifica si un objeto específico bloquea la luz
**
** Descripción:
** Determina si un objeto individual intersecta con el rayo de sombra
** entre un punto y la fuente de luz. Maneja diferentes tipos de objetos
** y verifica que la intersección esté dentro del rango válido.
**
** Parámetros:
** - object: Objeto a verificar si causa sombra
** - shadow_ray: Rayo desde el punto hacia la luz
** - light_distance: Distancia máxima a la fuente de luz
**
** Funcionamiento:
** 1. Verifica intersección según el tipo de objeto
** 2. Comprueba que la intersección esté entre el punto y la luz
** 3. Retorna 1 si el objeto bloquea la luz, 0 si no
**
** Retorna:
** - 1 si el objeto causa sombra (bloquea la luz)
** - 0 si el objeto no interfiere con la luz
*/
int	check_shadow(t_object object, t_ray shadow_ray, double light_distance)
{
	double	t;

	if (object.type == SPHERE)
	{
		if (hit_sphere(object.data.sphere, shadow_ray, &t) >= 0 && \
			t < light_distance)
			return (1);
	}
	else if (object.type == PLANE)
	{
		if (hit_plane(object.data.plane, shadow_ray, &t) >= 0 && \
			t < light_distance)
			return (1);
	}
	else if (object.type == CYLINDER)
	{
		if (hit_cylinder(object.data.cylinder, shadow_ray, &t) >= 0 && \
			t < light_distance)
			return (1);
	}
	return (0);
}

/*
** is_shadow - Determina si un punto está en sombra respecto a una luz
**
** Descripción:
** Verifica si existe algún objeto entre un punto de la superficie y una
** fuente de luz específica. Lanza un rayo de sombra y verifica intersecciones
** con todos los objetos de la escena para determinar oclusión.
**
** Parámetros:
** - scene: Escena con todos los objetos a verificar
** - point: Punto de la superficie donde verificar sombra
** - light: Fuente de luz a verificar
** - surface_normal: Normal de la superficie (para evitar auto-intersección)
**
** Funcionamiento:
** 1. Calcula dirección y distancia hacia la luz
** 2. Crea rayo de sombra con offset para evitar auto-intersección
** 3. Verifica intersecciones con todos los objetos de la escena
** 4. Retorna si el punto está en sombra o iluminado
**
** Retorna:
** - 1 si el punto está en sombra (hay oclusión)
** - 0 si el punto está iluminado (sin oclusión)
**
** Nota: Usa offset en la normal para evitar que el objeto se haga sombra a sí mismo
*/
int	is_shadow(t_scene *scene, t_vec3 point, t_light light, t_vec3 surface_normal)
{
	// Crear rayo desde el punto hacia la luz
	double	light_distance;
	int		i;
	t_vec3	light_dir;
	t_ray	shadow_ray;

	light_dir = vec_sub(light.position, point);
	light_distance = vec_length(light_dir);
	light_dir = vec_normalize(light_dir);

	// Crear rayo de sombra con offset along surface normal to avoid self-intersection
	shadow_ray.origin = vec_add(point, vec_scale(surface_normal, 0.001));
	shadow_ray.direction = light_dir;

	i = 0;
	// Verificar intersección con todos los objetos
	while (i < scene->object_count)
	{
		if (check_shadow(scene->objects[i], shadow_ray, light_distance))
			return (1);
		i++;
	}
	return (0);
}
