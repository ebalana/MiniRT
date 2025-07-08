/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:06:20 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/03 14:06:59 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

#include "miniRT.h"

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

//Constructor
t_vec3	vec3(double x, double y, double z);

// Operaciones basicas
t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_sub(t_vec3 a,t_vec3 b);
t_vec3	vec_scale(t_vec3 v, double s);
t_vec3	vec_div(t_vec3 v, double s);

// Producto escalar y vectorial
double	vec_dot(t_vec3 a,t_vec3 b);
t_vec3	vec_cross(t_vec3 a, t_vec3 b);

// Magnitud y normalización
double	vec_lenght(t_vec3 v);
t_vec3	vec_normalize(t_vec3 v);

#endif