/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:18:11 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/16 17:50:16 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// void	parse_sphere(char **tokens, t_scene *scene)
// {
// 	t_object	obj;

// 	if (!tokens[1] || !tokens[2] || !tokens[3])
// 		ft_error();
// 	obj.type = SPHERE;
// 	obj.color = parse_color(tokens[3]);
// 	obj.data.sphere.center = parse_vector(tokens[1]);
// 	obj.data.sphere.radius = ft_atof(tokens[2]);
// 	add_object(scene, obj);
// }

// void	parse_plane(char **tokens, t_scene *scene)
// {
// 	t_object	obj;

// 	if (!tokens[1] || !tokens[2] || !tokens[3])
// 		ft_error();
// 	obj.type = PLANE;
// 	obj.color = parse_color(tokens[3]);
// 	obj.data.plane.point = parse_vector(tokens[1]);
// 	obj.data.plane.normal = parse_vector(tokens[2]);
// 	add_object(scene, obj);
// }

// void	parse_cylinder(char **tokens, t_scene *scene)
// {
// 	t_object	obj;

// 	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
// 		ft_error();
// 	obj.type = CYLINDER;
// 	obj.color = parse_color(tokens[5]);
// 	obj.data.cylinder.center = parse_vector(tokens[1]);
// 	obj.data.cylinder.axis = parse_vector(tokens[2]);
// 	obj.data.cylinder.radius = ft_atof(tokens[3]);
// 	obj.data.cylinder.height = ft_atof(tokens[4]);
// 	add_object(scene, obj);
// }

void	parse_sphere(char **tokens, t_scene *scene)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		ft_error();
	t_vec3 center = parse_vector(tokens[1]);
	double radius = ft_atof(tokens[2]);
	t_vec3 color = parse_color(tokens[3]);
	t_object obj = create_sphere(center, radius, color);
	add_object(scene, obj);
}

void	parse_plane(char **tokens, t_scene *scene)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		ft_error();
	t_vec3 point = parse_vector(tokens[1]);
	t_vec3 normal = parse_vector(tokens[2]);
	t_vec3 color = parse_color(tokens[3]);
	t_object obj = create_plane(point, normal, color);
	add_object(scene, obj);
}

void	parse_cylinder(char **tokens, t_scene *scene)
{
	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		ft_error();
	t_vec3 center = parse_vector(tokens[1]);
	t_vec3 axis = parse_vector(tokens[2]);
	double radius = ft_atof(tokens[3]);
	double height = ft_atof(tokens[4]);
	t_vec3 color = parse_color(tokens[5]);
	t_object obj = create_cylinder(center, axis, radius, height, color);
	add_object(scene, obj);
}