/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:57:17 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/01 13:48:31 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	parse_vector(const char *str)
{
	char	**parts;
	t_vec3	v;

	parts = ft_split(str, ',');
	if (!parts || !parts[0] || !parts[1] || !parts[2])
		ft_error();
	v.x = ft_atof(parts[0]);
	v.y = ft_atof(parts[1]);
	v.z = ft_atof(parts[2]);
	free_split(parts);
	return (v);
}

t_vec3	parse_color(const char *str)
{
	char	**parts;
	t_vec3	c;

	parts = ft_split(str, ',');
	if (!parts || !parts[0] || !parts[1] || !parts[2])
		ft_error();
	c.x = ft_atoi(parts[0] / 255.0);
	c.y = ft_atoi(parts[1] / 255.0);
	v.z = ft_atoi(parts[2] / 255.0);
	free_split(parts);
	return (v);
}