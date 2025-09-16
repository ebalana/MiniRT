/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_components.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:40:05 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/16 19:32:38 by ebalana-         ###   ########.fr       */
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
	scene->camera.direction = vec_normalize(parse_vector(tokens[2]));
	scene->camera.fov = ft_atof(tokens[3]);
	if (scene->camera.fov <= 0.0 || scene->camera.fov >= 180.0)
		ft_error();
}

static void	add_light_to_scene(t_scene *scene, t_light light)
{
	t_light	*new_lights;
	int		i;

	new_lights = malloc(sizeof(t_light) * (scene->light_count + 1));
	if (!new_lights)
		ft_error();
	i = 0;
	while (i < scene->light_count)
	{
		new_lights[i] = scene->lights[i];
		i++;
	}
	new_lights[scene->light_count] = light;
	if (scene->lights)
		free(scene->lights);
	scene->lights = new_lights;
	scene->light_count++;
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
	add_light_to_scene(scene, light);
}
