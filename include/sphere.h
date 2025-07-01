#ifndef SHPERE_H
# define SHPERE_H

# include "vec3.h"
# define SPHERES_COUNT 3

typedef struct s_sphere
{
	t_vec3	center;
	double	radius;
}	t_sphere;

#endif