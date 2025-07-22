/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:54:50 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/22 16:58:30 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
** render_pixel - Renderiza un píxel específico usando ray tracing
**
** Descripción:
** Calcula la dirección del rayo desde la cámara hacia el píxel especificado,
** lanza el rayo en la escena, obtiene el color resultante y lo dibuja en
** la imagen MLX en las coordenadas correspondientes.
**
** Parámetros:
** - data: Estructura con datos de renderizado (imagen, escena)
** - pixel: Estructura con coordenadas y valores UV del píxel
** - viewport: Estructura con vectores de la cámara (origen, direcciones)
**
** Funcionamiento:
** 1. Calcula la dirección del rayo interpolando en el viewport
** 2. Crea un rayo desde la cámara hacia el píxel
** 3. Obtiene el color final usando ray_color()
** 4. Convierte el color a formato MLX y lo dibuja
*/
void	render_pixel(t_render_data *data, t_pixel *pixel, t_viewport *viewport)
{
	t_vec3	direction;
	t_ray	ray;
	t_vec3	color;
	int		mlx_color;

	direction = vec_add(vec_add(viewport->lower_left_corner, \
				vec_scale(viewport->horizontal, pixel->u)), \
				vec_scale(viewport->vertical, pixel->v));
	direction = vec_sub(direction, viewport->origin);
	ray.origin = viewport->origin;
	ray.direction = direction;
	color = ray_color(ray, data->scene);
	mlx_color = rgb_to_mlx_color(color.x, color.y, color.z);
	mlx_put_pixel(data->img, pixel->i, HEIGHT - 1 - pixel->j, mlx_color);
}

/*
** render_rows - Renderiza un rango específico de filas de la imagen
**
** Descripción:
** Itera sobre las filas desde start hasta end, y para cada píxel en esas
** filas calcula sus coordenadas UV y llama a render_pixel() para procesarlo.
** Esta función es clave para el renderizado progresivo.
**
** Parámetros:
** - data: Estructura con datos de renderizado (imagen, escena)
** - start: Fila inicial a renderizar
** - end: Fila final a renderizar (exclusiva)
** - view: Estructura con vectores de la cámara
**
** Funcionamiento:
** 1. Itera desde HEIGHT-1-start hasta HEIGHT-end (coordenadas MLX)
** 2. Para cada píxel, calcula coordenadas normalizadas U,V (0-1)
** 3. Crea estructura t_pixel y llama a render_pixel()
** 4. Permite renderizado por chunks para mantener responsividad
*/
void	render_rows(t_render_data *data, int start, int end, t_viewport *view)
{
	int		j;
	int		i;
	t_pixel	pixel;

	j = HEIGHT - 1 - start;
	while (j >= HEIGHT - end)
	{
		i = 0;
		while (i < WIDTH)
		{
			pixel.i = i;
			pixel.j = j;
			pixel.u = (double)i / (WIDTH - 1);
			pixel.v = (double)j / (HEIGHT - 1);
			render_pixel(data, &pixel, view);
			i++;
		}
		j--;
	}
}

/*
** render_scene - Función principal de renderizado progresivo
**
** Descripción:
** Función callback para mlx_loop_hook que renderiza la escena progresivamente.
** En cada frame renderiza solo unas pocas filas para mantener la aplicación
** responsiva y evitar el bloqueo de la interfaz.
**
** Parámetros:
** - param: Puntero a t_render_data (se castea desde void*)
**
** Funcionamiento:
** 1. Verifica si el renderizado ya está completo
** 2. Calcula qué filas renderizar en este frame
** 3. Configura la cámara y viewport
** 4. Renderiza las filas asignadas
** 5. Actualiza el progreso y estado de completado
**
** Nota: Se llama automáticamente por MLX en cada frame del loop principal
*/
void	render_scene(void *param)
{
	int				start_row;
	int				end_row;
	t_render_data	*data;
	t_viewport		viewport;
	double			aspect_ratio;

	data = (t_render_data *)param;
	if (data->rendering_complete)
		return ;
	get_render_rows(data->current_row, 5, &start_row, &end_row);
	aspect_ratio = (double)WIDTH / HEIGHT;
	get_camera_vectors(&data->scene->camera, aspect_ratio, &viewport);
	render_rows(data, start_row, end_row, &viewport);
	update_render_progress(data, end_row);
}
