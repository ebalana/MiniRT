#include "../include/miniRT.h"

t_vec3	vec3(double x, double y, double z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	return (vec3(a.x + b.x, a.y + b.y, a.z + b.z));
}

t_vec3	vec_sub(t_vec3 a,t_vec3 b)
{
	return (vec3(a.x - b.x, a.y - b.y, a.z - b.z));
}

t_vec3	vec_scale(t_vec3 v, double s)
{
	return (vec3(v.x *s, v.y *s, v.z * s));
}

t_vec3	vec_div(t_vec3 v, double s)
{
	return (vec3(v.x / s, v.y / s, v.z / s));
}

double	vec_dot(t_vec3 a,t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec_cross(t_vec3 a, t_vec3 b)
{
	return (vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	));
}

double	vec_lenght(t_vec3 v)
{
	return (sqrt(vec_dot(v, v)));
}

t_vec3	vec_normalize(t_vec3 v)
{
	return (vec_div(v, vec_lenght(v)));
}

/* int main(int argc, char **argv)
{
	t_vec3 a = vec3(10, 4, 6);
	t_vec3 b = vec3(4, 5, 6);
	t_vec3 div = vec_div(a, 2);
	t_vec3 result = vec_normalize(a);
	//double dot = vec_dot(a, a);
	//double result2 = vec_lenght(a);
	
	printf("Div = %f\n", div);
	printf("Resultado: %f, %f, %f\n", result.x, result.y, result.z);

	//printf("Resultado2 = %f", result2);

} */