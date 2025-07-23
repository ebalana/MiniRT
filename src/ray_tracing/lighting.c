/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:59:28 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/23 16:41:58 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
** clamp_color - Limita los valores de color entre 0.0 y 1.0
**
** Descripción:
** Asegura que los componentes RGB del color estén dentro del rango válido
** para evitar saturación y errores de renderizado.
**
** Parámetros:
** - color: Color a limitar
**
** Retorna:
** - Color con componentes limitados entre 0.0 y 1.0
*/
t_vec3	clamp_color(t_vec3 color)
{
	t_vec3	result;

	result.x = fmin(fmax(color.x, 0.0), 1.0);
	result.y = fmin(fmax(color.y, 0.0), 1.0);
	result.z = fmin(fmax(color.z, 0.0), 1.0);
	return (result);
}

/*
** calculate_diffuse_lighting - Calcula la iluminación difusa para un punto
**
** Descripción:
** Aplica el modelo de iluminación difusa de Lambert usando la primera luz
** de la escena. Considera sombras y aplica atenuación si el punto está en sombra.
**
** Parámetros:
** - scene: Escena con luces y objetos
** - hit_point: Punto de intersección
** - normal: Normal de la superficie en el punto
**
** Retorna:
** - Color difuso calculado
*/
// t_vec3	calculate_diffuse_lighting(t_scene *scene, t_vec3 hit_point, t_vec3 normal)
// {
// 	t_vec3	light_dir;
// 	double	light_intensity;
// 	t_vec3	diffuse;

// 	light_dir = vec_normalize(vec_sub(scene->lights[0].position, hit_point));
// 	light_intensity = fmax(0.0, vec_dot(normal, light_dir));
// 	diffuse = vec_scale(scene->lights[0].color,
// 			light_intensity * scene->lights[0].intensity);
// 	if (is_shadow(scene, hit_point, scene->lights[0], normal))
// 		diffuse = vec_scale(diffuse, 0.3);
// 	return (diffuse);
// }
t_vec3	calculate_diffuse_lighting(t_scene *scene, t_vec3 hit_point, t_vec3 normal, t_vec3 object_color)
{
	t_vec3	light_dir;
	double	light_intensity;
	t_vec3	light_contribution;
	t_vec3	diffuse;

	light_dir = vec_normalize(vec_sub(scene->lights[0].position, hit_point));
	light_intensity = fmax(0.0, vec_dot(normal, light_dir));

	// Modular el color del objeto con el color de la luz
	light_contribution = vec3(
		object_color.x * scene->lights[0].color.x,
		object_color.y * scene->lights[0].color.y,
		object_color.z * scene->lights[0].color.z
	);

	diffuse = vec_scale(light_contribution, light_intensity * scene->lights[0].intensity);

	if (is_shadow(scene, hit_point, scene->lights[0], normal))
		diffuse = vec_scale(diffuse, 0.3);

	return (diffuse);
}

/*
** apply_lighting - Aplica iluminación completa a un objeto intersectado
**
** Descripción:
** Combina iluminación ambiental y difusa para calcular el color final
** del objeto en el punto de intersección.
**
** Parámetros:
** - scene: Escena con configuración de iluminación
** - object: Objeto intersectado
** - hit_point: Punto de intersección
** - normal: Normal de la superficie
**
** Retorna:
** - Color final con iluminación aplicada
*/
t_vec3	apply_lighting(t_scene *scene, t_object object, t_vec3 hit_point, t_vec3 normal)
{
	t_vec3	object_color;
	t_vec3	ambient_effect;
	t_vec3	final_color;
	t_vec3	diffuse;

	object_color = object.color;
	ambient_effect = vec3(
		object_color.x * scene->ambient_color.x * scene->ambient_ratio,
		object_color.y * scene->ambient_color.y * scene->ambient_ratio,
		object_color.z * scene->ambient_color.z * scene->ambient_ratio
	);
	final_color = ambient_effect;
	if (scene->light_count > 0)
	{
		diffuse = calculate_diffuse_lighting(scene, hit_point, normal, object_color);
		final_color = vec_add(final_color, diffuse);
	}
	return (clamp_color(final_color));
}