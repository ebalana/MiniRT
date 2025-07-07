/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/07 18:23:31 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Convert RGB values (0-1) to MLX42 color format
static uint32_t rgb_to_mlx_color(double r, double g, double b)
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

double hit_sphere(t_sphere sphere, t_ray ray, double *t)
{
	t_vec3 oc;
	double a;
	double b;
	double c;
	double discriminant;
	double t1, t2;

	oc = vec_sub(ray.origin, sphere.center);
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - (sphere.radius * sphere.radius);
	discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
		return (-1.0); // No intersection

	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);

	// Return the closest positive intersection
	if (t1 > 0.001)
	{
		*t = t1;
		return (t1);
	}
	else if (t2 > 0.001)
	{
		*t = t2;
		return (t2);
	}
	return (-1.0);
}

// Cambiar la función ray_color para usar la escena
t_vec3 ray_color(t_ray ray, t_scene *scene)
{
	double t;
	t_vec3 hit_point;
	t_vec3 normal;
	int i;

	i = 0;
	// Buscar intersección con objetos en la escena
	while (i < scene->object_count)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (hit_sphere(scene->objects[i].data.sphere, ray, &t) >= 0)
			{
				// Calculate normal at hit point
				hit_point = vec_add(ray.origin, vec_scale(ray.direction, t));
				normal = vec_normalize(vec_sub(hit_point, scene->objects[i].data.sphere.center));
				
				//Ambient Light
				t_vec3 object_color = scene->objects[i].color;
				t_vec3 ambient_effect = vec_scale(scene->ambient_color, scene->ambient_ratio);
				t_vec3 final_color = vec_add(object_color, ambient_effect);
				return final_color;
			}
		}
		i++;  // Incrementar contador
	}
	return vec3(0.0, 0.0, 0.0);
}

// Modificar render_scene para recibir la escena
static void render_scene(mlx_image_t *img, t_scene *scene)
{
	// Camera setup (mismo código que tienes)
	double aspect_ratio = (double)WIDTH / HEIGHT;
	double viewport_height = 2.0;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.0;

	t_vec3 origin = scene->camera.position;  // Usar la cámara de la escena
	t_vec3 horizontal = vec3(viewport_width, 0, 0);
	t_vec3 vertical = vec3(0, viewport_height, 0);
	t_vec3 lower_left_corner = vec_sub(vec_sub(vec_sub(origin,
		vec_scale(horizontal, 0.5)), vec_scale(vertical, 0.5)), 
		vec3(0, 0, focal_length));

	// Render each pixel (mismo bucle)
	for (int j = HEIGHT - 1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			double u = (double)i / (WIDTH - 1);
			double v = (double)j / (HEIGHT - 1);
			
			t_vec3 direction = vec_add(vec_add(lower_left_corner, 
				vec_scale(horizontal, u)), vec_scale(vertical, v));
			direction = vec_sub(direction, origin);
			
			t_ray ray;
			ray.origin = origin;
			ray.direction = direction;
			
			// Llamar a ray_color con la escena
			t_vec3 color = ray_color(ray, scene);
			uint32_t mlx_color = rgb_to_mlx_color(color.x, color.y, color.z);
			
			mlx_put_pixel(img, i, HEIGHT - 1 - j, mlx_color);
		}
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

	// Configurar cámara
	scene->camera.position = vec3(0, 0, 0);
	scene->camera.direction = vec3(0, 0, -1);
	scene->camera.fov = 90.0;

	// Crear esfera hardcodeada (roja)
	t_object red_sphere = create_sphere(vec3(0, 0, -1), 0.5, vec3(0.1, 0.0, 0.0));
	add_object(scene, red_sphere);

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
	free(scene->objects);
	free(scene);
	return (EXIT_SUCCESS);
}