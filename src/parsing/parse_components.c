/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_components.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:40:05 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/03 14:01:17 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*Comprobamos que existen los tokens de intensidad y color.
Convertimos la intensidad a double con ft_atof().
Validamos que esté entre 0 y 1.
Parseamos el color con tu función parse_color() y lo guardamos en la escena.*/
void	parse_ambient(char **tokens, t_scene *scene)
{
	if (!tokens[1] || !tokens[2])
		ft_error();
	scene->ambient_ratio = ft_atof(tokens[1]);
	if (scene->ambient_ratio < 0.0 || scene->ambient_ratio > 1.0)
		ft_error();
	scene->ambient_color = parse_color(tokens[2]);
}

/*Comprobamos que existan los 3 tokens.
Parseamos la posición y dirección con parse_vector().
Parseamos el FOV y validamos que esté entre 0 y 180 grados.*/
void	parse_camera(char **tokens, t_scene *scene)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		ft_error();
	scene->camera.position = parse_vector(tokens[1]);
	scene->camera.direction = parse_vector(tokens[2]);
	scene->camera.fov = ft_atof(tokens[3]);
	if (scene->camera.fov <= 0.0 || scene->camera.fov >= 180.0)
		ft_error();
}

/*Validamos que existan todos los tokens.
Parseamos posición, intensidad y color.
Validamos que la intensidad esté entre 0 y 1.
Añadimos la luz al array dinámico de la escena, aumentando light_count.*/
void	parse_light(char **tokens, t_scene *scene)
{
	t_light	light;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		ft_error();
	light.position = parse_vector(tokens[1]);
	light.intensity = ft_atof(tokens[2]);
	if (light.intensity < 0.0 || light.intensity > 1.0)
		ft_error();
	light.color = parse_color(tokens[3]);
	scene->lights = realloc(scene->lights, sizeof(t_light) * (scene->light_count + 1));
	if (!scene->lights)
		ft_error();
	scene->lights[scene->light_count] = light;
	scene->light_count++;
}


