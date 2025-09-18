/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:57:17 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/18 17:53:17 by ebalana-         ###   ########.fr       */
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
	t_vec3	c;

	c = parse_vector(str);
	c.x /= 255.0;
	c.y /= 255.0;
	c.z /= 255.0;
	return (c);
}
