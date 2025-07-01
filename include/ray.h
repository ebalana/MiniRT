#ifndef RAY_H
# define RAY_H

#include "vec3.h"

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

//Constructor
t_ray	ray(t_vec3 origin, t_vec3 direction);

// Evaluar punto a lo largo del rayo
t_vec3	ray_at(t_ray r, double t);

#endif