/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:18:11 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/18 20:29:46 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	parse_sphere(char **tokens, t_scene *scene)
{
	t_vec3		center;
	t_vec3		color;
	t_object	obj;
	double		radius;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		ft_error();
	center = parse_vector(tokens[1]);
	radius = ft_atof(tokens[2]);
	color = parse_color(tokens[3]);
	obj = create_sphere(center, radius, color);
	add_object(scene, obj);
}

void	parse_plane(char **tokens, t_scene *scene)
{
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	obj;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		ft_error();
	point = parse_vector(tokens[1]);
	normal = parse_vector(tokens[2]);
	color = parse_color(tokens[3]);
	obj = create_plane(point, normal, color);
	add_object(scene, obj);
}

void	parse_cylinder(char **tokens, t_scene *scene)
{
	t_cylinder	params;
	t_object	obj;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		ft_error();
	params.center = parse_vector(tokens[1]);
	params.axis = parse_vector(tokens[2]);
	params.radius = ft_atof(tokens[3]);
	params.height = ft_atof(tokens[4]);
	params.color = parse_color(tokens[5]);
	obj = create_cylinder(params);
	add_object(scene, obj);
}
