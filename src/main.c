/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/18 19:11:15 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	validate_scene(t_scene *scene)
{
	if (scene->light_count == 0)
	{
		printf("Error: Scene must have at least one light\n");
		free_scene(scene);
		exit(EXIT_FAILURE);
	}
	if (scene->object_count == 0)
	{
		printf("Error: Scene must have at least one object\n");
		free_scene(scene);
		exit(EXIT_FAILURE);
	}
}

void	check_arguments(int argc, char **argv)
{
	int		len;
	char	*extension;

	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	len = ft_strlen(argv[1]);
	if (len < 4)
	{
		printf("Error: Invalid file format\n");
		exit(EXIT_FAILURE);
	}
	extension = argv[1] + len - 3;
	if (ft_strncmp(extension, ".rt", 3) != 0)
	{
		printf("Error: File must have .rt extension\n");
		exit(EXIT_FAILURE);
	}
}

int	init_and_parse(int argc, char **argv, t_scene **scene)
{
	check_arguments(argc, argv);
	*scene = init_scene();
	if (!*scene)
	{
		printf("Error: Failed to initialize scene\n");
		return (EXIT_FAILURE);
	}
	parse_line(argv[1], *scene);
	validate_scene(*scene);
	return (EXIT_SUCCESS);
}

int	init_and_render(t_scene *scene)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_render_data	render_data;

	mlx = init_mlx();
	if (!mlx)
	{
		free_scene(scene);
		return (EXIT_FAILURE);
	}
	img = create_image(mlx);
	if (!img)
	{
		mlx_terminate(mlx);
		free_scene(scene);
		return (EXIT_FAILURE);
	}
	init_render_data(&render_data, img, scene);
	display_image(mlx, img);
	printf("Starting progressive render...\n");
	mlx_loop_hook(mlx, render_scene, &render_data);
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_scene	*scene;
	int		result;

	if (init_and_parse(argc, argv, &scene) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	result = init_and_render(scene);
	free_scene(scene);
	return (result);
}
