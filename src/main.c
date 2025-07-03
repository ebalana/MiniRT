/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/03 14:07:05 by ebalana-         ###   ########.fr       */
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

t_vec3 ray_color(t_ray ray)
{
	t_sphere sphere;
	double t;
	t_vec3 hit_point, normal;

	// Create a simple sphere at the center
	sphere.center = vec3(0, 0, -1);
	sphere.radius = 0.5;

	if (hit_sphere(sphere, ray, &t) >= 0)
	{
		// Calculate normal at hit point
		hit_point = vec_add(ray.origin, vec_scale(ray.direction, t));
		normal = vec_normalize(vec_sub(hit_point, sphere.center));
		
		// Color based on normal (convert from [-1,1] to [0,1])
		return (vec3(
			(normal.x + 1.0) * 0.5,
			(normal.y + 1.0) * 0.5,
			(normal.z + 1.0) * 0.5
		));
	}

	// Background gradient (blue to white)
	t_vec3 unit_direction = vec_normalize(ray.direction);
	double blend = 0.5 * (unit_direction.y + 1.0);

	t_vec3 white = vec3(1.0, 1.0, 1.0);
	t_vec3 blue = vec3(0.5, 0.7, 1.0);

	return (vec_add(vec_scale(white, (1.0 - blend)), vec_scale(blue, blend)));
}

static void render_scene(mlx_image_t *img)
{
	// Camera setup
	double aspect_ratio = (double)WIDTH / HEIGHT;
	double viewport_height = 2.0;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.0;

	t_vec3 origin = vec3(0, 0, 0);
	t_vec3 horizontal = vec3(viewport_width, 0, 0);
	t_vec3 vertical = vec3(0, viewport_height, 0);
	t_vec3 lower_left_corner = vec_sub(vec_sub(vec_sub(origin, 
		vec_scale(horizontal, 0.5)), vec_scale(vertical, 0.5)), 
		vec3(0, 0, focal_length));

	// Render each pixel
	for (int j = HEIGHT - 1; j >= 0; j--)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			double u = (double)i / (WIDTH - 1);
			double v = (double)j / (HEIGHT - 1);
			
			// Calculate ray direction
			t_vec3 direction = vec_add(vec_add(lower_left_corner, 
				vec_scale(horizontal, u)), vec_scale(vertical, v));
			direction = vec_sub(direction, origin);
			
			t_ray ray;
			ray.origin = origin;
			ray.direction = direction;
			
			// Get pixel color
			t_vec3 color = ray_color(ray);
			uint32_t mlx_color = rgb_to_mlx_color(color.x, color.y, color.z);
			
			// MLX42 uses inverted Y coordinate
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

int main(void)
{
	mlx_t *mlx;
	mlx_image_t *img;
		
	// Initialize MLX
	mlx = mlx_init(WIDTH, HEIGHT, "miniRT - Ray Tracing", true);
	if (!mlx)
		ft_error();    
	// Create image
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(mlx);
		ft_error();
	}    
	// Render the scene
	printf("Rendering scene...\n");
	render_scene(img);
	printf("Rendering complete!\n");    
	// Display image
	if (mlx_image_to_window(mlx, img, 0, 0) == -1)
	{
		mlx_terminate(mlx);
		ft_error();
	}    
	// Set up keyboard hook
	mlx_key_hook(mlx, key_hook, mlx);    
	// Start main loop
	mlx_loop(mlx);    
	// Cleanup
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}