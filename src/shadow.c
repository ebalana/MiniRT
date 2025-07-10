/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:47:15 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/10 14:59:19 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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
	return (0);
}

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
