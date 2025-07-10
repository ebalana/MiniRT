/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/10 16:16:43 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Convert RGB values (0-1) to MLX42 color format
static int rgb_to_mlx_color(double r, double g, double b)
{
	int ir = (int)(255.999 * r);
	int ig = (int)(255.999 * g);
	int ib = (int)(255.999 * b);

	// Clamp values
	if (ir > 255) ir = 255; if (ir < 0) ir = 0;
	if (ig > 255) ig = 255; if (ig < 0) ig = 0;
	if (ib > 255) ib = 255; if (ib < 0) ib = 0;

	return (ir << 24) | (ig << 16) | (ib << 8) | 0xFF;
}

t_vec3 ray_color(t_ray ray, t_scene *scene)
{
	double closest_t = -1.0;
	int closest_object = -1;
	double t;
	
	// Find closest intersection
	for (int i = 0; i < scene->object_count; i++)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (hit_sphere(scene->objects[i].data.sphere, ray, &t) >= 0)
			{
				if (closest_t < 0 || t < closest_t)
				{
					closest_t = t;
					closest_object = i;
				}
			}
		}
		else if (scene->objects[i].type == PLANE)
		{
			if (hit_plane(scene->objects[i].data.plane, ray, &t) >= 0)
			{
				if (closest_t < 0 || t < closest_t)
				{
					closest_t = t;
					closest_object = i;
				}
			}
		}
	}
	
	// If no intersection found, return background
	if (closest_object == -1)
		return vec3(0.0, 0.0, 0.0);
	
	// Calculate lighting for closest object
	t_vec3 hit_point = vec_add(ray.origin, vec_scale(ray.direction, closest_t));
	t_vec3 normal;
	
	if (scene->objects[closest_object].type == SPHERE)
	{
		normal = vec_normalize(vec_sub(hit_point, scene->objects[closest_object].data.sphere.center));
	}
	else if (scene->objects[closest_object].type == PLANE)
	{
		normal = scene->objects[closest_object].data.plane.normal;
	}
	
	// Apply lighting
	t_vec3 object_color = scene->objects[closest_object].color;
	t_vec3 ambient_effect = vec_scale(scene->ambient_color, scene->ambient_ratio);
	t_vec3 final_color = vec_add(object_color, ambient_effect);
	
	if (scene->light_count > 0)
	{
		t_vec3 light_dir = vec_normalize(vec_sub(scene->lights[0].position, hit_point));
		double light_intensity = fmax(0.0, vec_dot(normal, light_dir));
		t_vec3 diffuse = vec_scale(scene->lights[0].color, 
									light_intensity * scene->lights[0].intensity);
		
		 if (is_shadow(scene, hit_point, scene->lights[0], normal))
			diffuse = vec_scale(diffuse, 0.3);
			
		final_color = vec_add(final_color, diffuse);
		
		// Clamp colors
		final_color.x = fmin(final_color.x, 1.0);
		final_color.y = fmin(final_color.y, 1.0);
		final_color.z = fmin(final_color.z, 1.0);
	}	
	return final_color;
}

static void render_scene(mlx_image_t *img, t_scene *scene)
{
	// Camera setup
	double aspect_ratio = (double)WIDTH / HEIGHT;
	double viewport_height = 1.0;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.0;

	// Definir el espacio de la cámara
	t_vec3 origin = scene->camera.position;  // Usar la cámara de la escena
	t_vec3 horizontal = vec3(viewport_width, 0, 0);
	t_vec3 vertical = vec3(0, viewport_height, 0);
	t_vec3 lower_left_corner = vec_sub(vec_sub(vec_sub(origin,
		vec_scale(horizontal, 0.5)), vec_scale(vertical, 0.5)), 
		vec3(0, 0, focal_length));

	// Render each pixel
	int j = HEIGHT - 1;
	while (j >= 0) // Cada fila
	{
		int i = 0;
		while (i < WIDTH) // Cada columna
		{
			// Calcular coordenadas normalizadas (0-1)
			double u = (double)i / (WIDTH - 1);
			double v = (double)j / (HEIGHT - 1);

			// Calcular dirección del rayo desde cámara hacia el píxel
			t_vec3 direction = vec_add(vec_add(lower_left_corner, 
				vec_scale(horizontal, u)), vec_scale(vertical, v));
			direction = vec_sub(direction, origin);
			
			// Crear rayo desde cámara hacia el píxel
			t_ray ray;
			ray.origin = origin;
			ray.direction = direction;
			
			// Obtener color final y convertir a formato MLX
			t_vec3 color = ray_color(ray, scene);
			int mlx_color = rgb_to_mlx_color(color.x, color.y, color.z);			
			mlx_put_pixel(img, i, HEIGHT - 1 - j, mlx_color);
			
			i++;
		}
		j--;
	}
}

// En tu main(), crear la escena hardcodeada:
int main(void)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_scene		*scene;

	// Crear la escena
	scene = init_scene();
	if (!scene)
	{
		printf("Error creando escena\n");
		return (EXIT_FAILURE);
	}

	/*----------------------------- HARDCODED PARAMS -----------------------------*/

	// Configurar cámara hardcodeada
	scene->camera.position = vec3(0, 0, 0);
	scene->camera.direction = vec3(0, 0, -1);
	scene->camera.fov = 90.0;

	//Crear 3 esferas de colores
	t_object red_sphere = create_sphere(vec3(-1.5, -1, -5), 0.5, vec3(0.8, 0.1, 0.1));
	add_object(scene, red_sphere);

	t_object blue_sphere = create_sphere(vec3(0, 0, -5), 0.5, vec3(0.1, 0.1, 0.8));
	add_object(scene, blue_sphere);

	t_object green_sphere = create_sphere(vec3(1.5, 1, -5), 0.5, vec3(0.1, 0.8, 0.1));
	add_object(scene, green_sphere);

	//Crear plano como "suelo" (horizontal)
	t_object floor_plane = create_plane(vec3(0, -2, 0), vec3(0, 1, 0), vec3(0.5, 0.5, 0.5));
	add_object(scene, floor_plane);

	//Crear plano como "pared" (fondo)
	t_object wall_plane = create_plane(vec3(0, 0, -10), vec3(0, 0, 1), vec3(0.5, 0.5, 0.5));
	add_object(scene, wall_plane);

	// Crear plano vertical al lado de la esfera roja
	t_object side_wall = create_plane(vec3(-2, 0, 0), vec3(1, 0, 0), vec3(0.1, 0.1, 0.8));
	add_object(scene, side_wall);

	// Crear plano vertical al lado de la esfera verde
	t_object side_wall2 = create_plane(vec3(2, 0, 0), vec3(-1, 0, 0), vec3(0.8, 0.1, 0.1));
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

	/*----------------------------------------------------------------------------*/

	mlx = init_mlx();
	img = create_image(mlx);

	printf("Rendering scene...\n");
	render_scene(img, scene);
	printf("Rendering complete!\n");

	display_image(mlx, img);
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);
	
	mlx_terminate(mlx);
	free_scene(scene);
	return (EXIT_SUCCESS);
}