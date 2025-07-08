/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:07:20 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/03 14:07:33 by ebalana-         ###   ########.fr       */
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

//Evalúa el punto a lo largo del rayo según la fórmula: P(t) = origin + t * direction
t_vec3	ray_at(t_ray r, double t)
{
	return	(vec_add(r.origin, vec_scale(r.direction, t)));
}