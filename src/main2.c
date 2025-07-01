/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:52:23 by dcampas-          #+#    #+#             */
/*   Updated: 2025/07/01 17:51:02 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

double	hit_sphere(t_sphere sphere, t_ray ray, double *t)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	oc = vec_sub(ray.origin, sphere.center);
	a = vec_dot(ray.direction, ray.direction);
	b = 2 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - (sphere.radius * sphere.radius);
	discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
		return (0);
	if (t)
		*t = (-b - sqrt(discriminant)) / (2.0 * a);
	return (1);
}

void	write_color(FILE *f, t_vec3 color)
{
	int	ir;
	int	ig;
	int	ib;

	ir = (int)(255.999 * color.x);
	ig = (int)(255.999 * color.y);
	ib = (int)(255.999 * color.z);
	fprintf(f, "%d %d %d\n", ir, ig, ib);
}

t_vec3	ray_color(t_ray ray, t_sphere *spheres, int count)
{
	double	closest_t = INFINITY;
	t_vec3	closest_normal;
	int		hit_any;
	t_vec3	unit_dir;
	int		i;

	i = 0;
	while (i < count)
	{
		double	t;
		if (hit_sphere(spheres[i], ray, &t) && t < closest_t)
		{
			t_vec3	hit_point = ray_at(ray, t);	// P(t) = origin + t * dir
			closest_normal = vec_normalize(vec_sub(hit_point, spheres[i].center));
			hit_any = 1;
		}
		i++;
	}
	if (hit_any)
		return (vec_scale(vec_add(closest_normal, vec3(1, 1, 1)), 0.5)); // map normal [-1,1] → [0,1]

	unit_dir = vec_normalize(ray.direction);
	double	t = 0.5 * (unit_dir.y + 1.0);
	return (vec_add(vec_scale(vec3(1.0, 1.0, 1.0), 1.0 - t),
					vec_scale(vec3(0.5, 0.7, 1.0), t)));
}

int	main(void)
{
	int			width = 400;
	int			height = 200;
	FILE		*f = fopen("image.ppm", "w");
	t_sphere	spheres[SPHERES_COUNT] = {
				{vec3(0, 0, -1), 0.5},
				{vec3(1, 0, -2), 0.5},
				{vec3(-1, 0, 1), 0.5}};

	if (!f)
		return (1);
	fprintf(f, "P3\n%d %d\n255\n", width, height);

// Camera setup
	t_vec3 origin = vec3(0, 0, 0);
	double viewport_height = 2.0;
	double viewport_width = 4.0;
	double focal_length = 1.0;

	t_vec3 horizontal = vec3(viewport_width, 0, 0);
	t_vec3 vertical = vec3(0, viewport_height, 0);
	t_vec3 lower_left_corner = vec_sub(
		vec_sub(
			vec_sub(origin,
				vec_scale(horizontal, 0.5)),
			vec_scale(vertical, 0.5)),
		vec3(0, 0, focal_length));

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			double u = (double)i / (width - 1);
			double v = (double)j / (height - 1);
			t_vec3 dir = vec_add(
				vec_add(lower_left_corner,
					vec_add(vec_scale(horizontal, u),
							vec_scale(vertical, v))),
				vec_scale(origin, -1));
			t_ray r = ray(origin, dir);
			t_vec3 color = ray_color(r, spheres, SPHERES_COUNT);
			write_color(f, color);
		}
	}
	fclose(f);
	return (0);
}