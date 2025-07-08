/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/08 15:48:30 by ebalana-         ###   ########.fr       */
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
	double t;
	t_vec3 hit_point, normal;
	int i = 0;

	while (i < scene->object_count)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (hit_sphere(scene->objects[i].data.sphere, ray, &t) >= 0)
			{
				// Calcular punto e intersección y normal
				hit_point = vec_add(ray.origin, vec_scale(ray.direction, t));
				normal = vec_normalize(vec_sub(hit_point, scene->objects[i].data.sphere.center));
				
				// Ambient light
				t_vec3 object_color = scene->objects[i].color;
				t_vec3 ambient_effect = vec_scale(scene->ambient_color, scene->ambient_ratio);
				t_vec3 final_color = vec_add(object_color, ambient_effect);
				
				// Diffuse light
				if (scene->light_count > 0)
				{
					t_vec3 light_dir = vec_normalize(vec_sub(scene->lights[0].position, hit_point));
					double light_intensity = fmax(0.0, vec_dot(normal, light_dir));
					t_vec3 diffuse = vec_scale(scene->lights[0].color, 
												light_intensity * scene->lights[0].intensity);
					final_color = vec_add(final_color, diffuse);
					
					// Clamp colors
					final_color.x = fmin(final_color.x, 1.0);
					final_color.y = fmin(final_color.y, 1.0);
					final_color.z = fmin(final_color.z, 1.0);
				}
				
				return final_color;
			}
		}
		else if (scene->objects[i].type == PLANE)
		{
			if (hit_plane(scene->objects[i].data.plane, ray, &t) >= 0)
			{
				// Calcular punto de intersección
				hit_point = vec_add(ray.origin, vec_scale(ray.direction, t));
				normal = scene->objects[i].data.plane.normal;  // Normal ya está calculada
				
				// Aplicar iluminación (igual que esferas)
				t_vec3 object_color = scene->objects[i].color;
				t_vec3 ambient_effect = vec_scale(scene->ambient_color, scene->ambient_ratio);
				t_vec3 final_color = vec_add(object_color, ambient_effect);
				
				// Diffuse light
				if (scene->light_count > 0)
				{
					t_vec3 light_dir = vec_normalize(vec_sub(scene->lights[0].position, hit_point));
					double light_intensity = fmax(0.0, vec_dot(normal, light_dir));
					t_vec3 diffuse = vec_scale(scene->lights[0].color, 
												light_intensity * scene->lights[0].intensity);
					final_color = vec_add(final_color, diffuse);
					
					// Clamp colors
					final_color.x = fmin(final_color.x, 1.0);
					final_color.y = fmin(final_color.y, 1.0);
					final_color.z = fmin(final_color.z, 1.0);
				}
				
				return final_color;
			}
		}
		i++;	// Pasar al siguiente objeto
	}
	// Si no hay intersección con ningún objeto, devolver fondo negro
	return vec3(0.0, 0.0, 0.0);  // Fondo negro
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

static void key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t *mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

static void ft_error(void)
{
	fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// En tu main(), crear la escena hardcodeada:
int main(void)
{
	mlx_t *mlx;
	mlx_image_t *img;

	// Crear la escena
	t_scene *scene = init_scene();
	if (!scene)
	{
		printf("Error creando escena\n");
		return (EXIT_FAILURE);
	}

	// Configurar cámara hardcodeada
	scene->camera.position = vec3(0, 0, 0);
	scene->camera.direction = vec3(0, 0, -1);
	scene->camera.fov = 90.0;

	// // Crear esfera hardcodeada (roja)
	// t_object red_sphere = create_sphere(vec3(0, 0, -1), 0.5, vec3(0.1, 0.0, 0.0));
	// add_object(scene, red_sphere);

	// Crear 3 esferas de colores
	t_object red_sphere = create_sphere(vec3(-1.5, 0, -5), 0.5, vec3(0.8, 0.1, 0.1));
	add_object(scene, red_sphere);

	t_object blue_sphere = create_sphere(vec3(0, 0, -5), 0.5, vec3(0.1, 0.1, 0.8));
	add_object(scene, blue_sphere);

	t_object green_sphere = create_sphere(vec3(1.5, 0, -5), 0.5, vec3(0.1, 0.8, 0.1));
	add_object(scene, green_sphere);

	//Crear plano como "suelo" (horizontal)
	t_object floor_plane = create_plane(vec3(0, -2, 0), vec3(0, 1, 0), vec3(0.5, 0.5, 0.5));
	add_object(scene, floor_plane);

	//Crear plano como "pared"
	t_object wall_plane = create_plane(vec3(0, 0, -10), vec3(0, 0, 1), vec3(0.5, 0.5, 0.5));
	add_object(scene, wall_plane);

	// // Plano inclinado que sí se ve desde la cámara
	// t_object visible_plane = create_plane(vec3(0, 0, -8), vec3(0, 0.3, 1), vec3(0.5, 0.5, 0.5));
	// add_object(scene, visible_plane);

	// Crear luz hardcodeada
	t_light light;
	light.position = vec3(-40.0, 50.0, 0.0);
	light.color = vec3(1.0, 1.0, 1.0);  // Blanco para mandatory
	light.intensity = 0.6;

	// Añadir luz a la escena
	scene->lights = malloc(sizeof(t_light));
	if (!scene->lights)
		ft_error();
    scene->lights[0] = light;
    scene->light_count = 1;

	// Resto del código MLX (igual que tienes)
	mlx = mlx_init(WIDTH, HEIGHT, "miniRT - Ray Tracing", true);
	if (!mlx)
		ft_error();    
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(mlx);
		ft_error();
	}
	printf("Rendering scene...\n");
	render_scene(img, scene);
	printf("Rendering complete!\n");

	if (mlx_image_to_window(mlx, img, 0, 0) == -1)
	{
		mlx_terminate(mlx);
		ft_error();
	}    
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	// Liberar memoria de la escena
	free(scene->lights);
	free(scene->objects);
	free(scene);
	return (EXIT_SUCCESS);
}