/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/03 16:40:30 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
// En tu main(), crear la escena hardcodeada:
 int main(void)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_scene		*scene;
	t_render_data render_data;

	// Crear la escena
	scene = init_scene();
	if (!scene)
	{
		printf("Error creando escena\n");
		return (EXIT_FAILURE);
	}

	 ----------------------------- HARDCODED PARAMS -----------------------------
	// Configurar cámara hardcodeada
	scene->camera.position = vec3(0, 0.5, 2);
	scene->camera.direction = vec3(0, 0, -1);
	scene->camera.fov = 90.0;

	//Crear 3 esferas de colores
	t_object red_sphere = create_sphere(vec3(-1.5, -1, -8), 0.5, vec3(0.8, 0.1, 0.1));
	add_object(scene, red_sphere);

	t_object blue_sphere = create_sphere(vec3(0, 0, -5), 0.5, vec3(0.1, 0.1, 0.8));
	add_object(scene, blue_sphere);

	t_object green_sphere = create_sphere(vec3(1.5, 1, -8), 0.5, vec3(0.1, 0.8, 0.1));
	add_object(scene, green_sphere);

	//Crear plano como "suelo" (horizontal)
	t_object floor_plane = create_plane(vec3(0, -2, 0), vec3(0, 1, 0), vec3(0.5, 0.5, 0.5));
	add_object(scene, floor_plane);

	//Crear plano como "pared" (fondo)
	t_object wall_plane = create_plane(vec3(0, 0, -10), vec3(0, 0, 1), vec3(0.5, 0.5, 0.5));
	add_object(scene, wall_plane);

	// Crear plano vertical al lado de la esfera roja
	t_object side_wall = create_plane(vec3(-2.5, 0, 0), vec3(1, 0, 0), vec3(0.1, 0.1, 0.8));
	add_object(scene, side_wall);

	// Crear plano vertical al lado de la esfera verde
	t_object side_wall2 = create_plane(vec3(3, 0, 0), vec3(-1, 0, 0), vec3(0.8, 0.1, 0.1));
	add_object(scene, side_wall2);

	// Crear techo blanco
	t_object ceiling = create_plane(vec3(0, 3, 0), vec3(0, -1, 0), vec3(0.5, 0.5, 0.5));
	add_object(scene, ceiling);

	// Crear luz hardcodeada
	t_light light;
	light.position = vec3(0.0, 2.5, -5.0);
	light.color = vec3(1.0, 1.0, 1.0);  // Blanco para mandatory
	light.intensity = 0.6;

	// Añadir luz a la escena
	scene->lights = malloc(sizeof(t_light));
	if (!scene->lights)
		ft_error();
	scene->lights[0] = light;
	scene->light_count = 1;
	------------------------------------------------------------------------

	mlx = init_mlx();
	img = create_image(mlx);	
	init_render_data(&render_data, img, scene);
	display_image(mlx, img);
	printf("Starting progressive render...\n");
	
	// Usar loop hook para renderizado progresivo
	mlx_loop_hook(mlx, render_scene, &render_data);
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);
	
	mlx_terminate(mlx);
	free_scene(scene);
	return (EXIT_SUCCESS);
}
 */


/* int main(int argc, char **argv)
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_scene			*scene;
	t_render_data	render_data;

	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	// Inicializar la escena desde el archivo .rt
	scene = init_scene();
	if (!scene)
		ft_error();
	parse_line(argv[1], scene);

	// Inicializar MLX y la imagen
	mlx = init_mlx();
	img = create_image(mlx);
	init_render_data(&render_data, img, scene);
	display_image(mlx, img);

	printf("Starting progressive render...\n");

	// Renderizado progresivo
	mlx_loop_hook(mlx, render_scene, &render_data);
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);

	mlx_terminate(mlx);
	free_scene(scene);
	return (EXIT_SUCCESS);
}
 */

int main(void)
{
	t_scene *scene;

	scene = init_scene();
	if (!scene)
		return (1);

	parse_line("scene/test_min.rt", scene);

	printf("Scene parsed successfully!\n");
	printf("Objects: %d\n", scene->object_count);
	printf("Lights: %d\n", scene->light_count);

	for (int i = 0; i < scene->object_count; i++)
		printf("Object %d: type=%d color=(%.2f,%.2f,%.2f)\n", 
			i,
			scene->objects[i].type,
			scene->objects[i].color.x,
			scene->objects[i].color.y,
			scene->objects[i].color.z);

	for (int i = 0; i < scene->light_count; i++)
		printf("Light %d: intensity=%.2f color=(%.2f,%.2f,%.2f)\n", 
			i,
			scene->lights[i].intensity,
			scene->lights[i].color.x,
			scene->lights[i].color.y,
			scene->lights[i].color.z);

	free_scene(scene);
	return 0;
}