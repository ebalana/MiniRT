/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/22 16:12:04 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Convert RGB values (0-1) to MLX42 color format
int rgb_to_mlx_color(double r, double g, double b)
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
		else if (scene->objects[i].type == CYLINDER)
		{
			if (hit_cylinder(scene->objects[i].data.cylinder, ray, &t) >= 0)
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
	else if (scene->objects[closest_object].type == CYLINDER)
	{
		t_cylinder cyl = scene->objects[closest_object].data.cylinder;
		t_vec3 to_hit = vec_sub(hit_point, cyl.center);
		double projection = vec_dot(to_hit, cyl.axis);
		double height_limit = cyl.height / 2.0;
		
		// Verificar si estamos en una base
		if (fabs(projection - height_limit) < 0.001)
			normal = cyl.axis;  // Base superior
		else if (fabs(projection + height_limit) < 0.001)
			normal = vec_scale(cyl.axis, -1.0);  // Base inferior
		else
		{
			// Superficie lateral
			t_vec3 closest_point_on_axis = vec_add(cyl.center, vec_scale(cyl.axis, projection));
			normal = vec_normalize(vec_sub(hit_point, closest_point_on_axis));
		}
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

	/*----------------------------- HARDCODED PARAMS -----------------------------*/

	// Configurar cámara hardcodeada
	scene->camera.position = vec3(0, 0, 0);
	scene->camera.direction = vec3(0, 0, -1);
	scene->camera.fov = 90.0;

	//Crear plano como "suelo" (horizontal)
	t_object floor_plane = create_plane(vec3(0, -2, 0), vec3(0, 1, 0), vec3(0.5, 0.5, 0.5));
	add_object(scene, floor_plane);

	// Crear esfera izquierda
	t_object huevo_izquierdo = create_sphere(vec3(-0.2, -0.2, -5), 0.2,  vec3(0.5, 0.1, 0.8));
	add_object(scene, huevo_izquierdo);

	// Crear cilindro morado
	t_object bebe_arnau = create_cylinder(vec3(0, 0, -5), vec3(0, 1, 0), 0.2, 0.5, vec3(0.5, 0.1, 0.8));
	add_object(scene, bebe_arnau);
	
	// Crear esfera encima cilindro
	t_object puntita_arnau = create_sphere(vec3(0, 0.255, -5), 0.2, vec3(0.5, 0.1, 0.8));
	add_object(scene, puntita_arnau);

	// Crear esfera derecha
	t_object huevo_derecho = create_sphere(vec3(0.2, -0.2, -5), 0.2,  vec3(0.5, 0.1, 0.8));
	add_object(scene, huevo_derecho);
	
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
