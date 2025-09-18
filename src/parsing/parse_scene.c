/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:48:38 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/18 18:00:06 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	handle_tokens(char **tokens, t_scene *scene)
{
	if (ft_strncmp(tokens[0], "A", 2) == 0)
		parse_ambient(tokens, scene);
	else if (ft_strncmp(tokens[0], "C", 2) == 0)
		parse_camera(tokens, scene);
	else if (ft_strncmp(tokens[0], "L", 2) == 0)
		parse_light(tokens, scene);
	else if (ft_strncmp(tokens[0], "sp", 3) == 0)
		parse_sphere(tokens, scene);
	else if (ft_strncmp(tokens[0], "pl", 3) == 0)
		parse_plane(tokens, scene);
	else if (ft_strncmp(tokens[0], "cy", 3) == 0)
		parse_cylinder(tokens, scene);
	else
		ft_error();
}

void	parse_line(const char *filename, t_scene *scene)
{
	int		fd;
	char	*line;
	char	**tokens;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error();
	line = get_next_line(fd);
	while (line)
	{
		if (*line == '\n' || *line == '#')
		{
			free(line);
			continue ;
		}
		tokens = ft_split(line, ' ');
		if (!tokens || !tokens[0])
			ft_error();
		handle_tokens(tokens, scene);
		free_split(tokens);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}
