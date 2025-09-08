/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:18:34 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/08 15:12:51 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
** update_render_progress - Actualiza el progreso del renderizado
**
** Descripción:
** Actualiza el estado del renderizado progresivo, mostrando el porcentaje
** de completado y marcando cuando el renderizado ha terminado.
**
** Parámetros:
** - data: Estructura con datos de renderizado
** - end_row: Última fila renderizada en este frame
**
** Funcionamiento:
** 1. Actualiza current_row con la nueva posición
** 2. Cada 20 filas muestra el porcentaje de progreso
** 3. Cuando todas las filas están renderizadas, marca como completo
** 4. Proporciona feedback visual al usuario durante el renderizado
*/
void	update_render_progress(t_render_data *data, int end_row)
{
	data->current_row = end_row;
	if (data->current_row % 20 == 0)
		printf("Rendering... %d%%\n", (data->current_row * 100) / HEIGHT);
	if (data->current_row >= HEIGHT)
	{
		data->rendering_complete = 1;
		printf("Rendering complete!\n");
	}
}

/*
** get_camera_up - Calcula el vector "arriba" de la cámara
**
** Descripción:
** Devuelve un vector que representa la dirección "up" de la cámara,
** asegurando que sea perpendicular a la dirección de la cámara.
** Evita problemas cuando la cámara apunta casi verticalmente.
**
** Parámetros:
** - forward: Vector que indica la dirección hacia donde apunta la cámara
**
** Retorno:
** - Vector t_vec3 que representa la dirección "up" de la cámara
**
** Funcionamiento:
** 1. Define un vector de referencia world_up (0,1,0)
** 2. Si forward está casi alineado con world_up, usa (1,0,0) como referencia
** 3. Calcula el vector derecho (right) como perpendicular a forward y world_up
** 4. Calcula el vector arriba (up) como perpendicular a forward y right
*/
static t_vec3	get_camera_up(t_vec3 forward)
{
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;

	world_up = vec3(0, 1, 0);
	if (fabs(vec_dot(forward, world_up)) > 0.9)
		world_up = vec3(1, 0, 0);
	right = vec_normalize(vec_cross(forward, world_up));
	up = vec_cross(right, forward);
	return (up);
}

/*
** get_camera_vectors - Configura los vectores de la cámara para ray tracing
**
** Descripción:
** Calcula todos los vectores necesarios para proyectar rayos desde la cámara:
** origen, direcciones horizontales/verticales y esquina inferior izquierda
** del viewport. Establece un sistema de coordenadas ortonormal para la cámara.
**
** Parámetros:
** - camera: Estructura con posición y configuración de la cámara
** - aspect: Relación de aspecto de la imagen (ancho/alto)
** - view: Estructura viewport donde se almacenan los vectores resultantes
**
** Retorno:
** - Ninguno (modifica la estructura view directamente)
**
** Funcionamiento:
** 1. Convierte el FOV de la cámara a radianes y calcula la mitad del tamaño
**    del viewport vertical y horizontal basado en aspect ratio
** 2. Normaliza la dirección de la cámara (forward)
** 3. Calcula los vectores "up" y "right" usando get_camera_up y cross product
** 4. Asigna origen de la cámara
** 5. Calcula los vectores horizontales y verticales del viewport escalados
** 6. Determina la esquina inferior izquierda del viewport restando la mitad
**    de los vectores horizontal y vertical al centro del viewport
**
** Nota: La configuración es simplificada, pero permite proyectar rayos
**       en un sistema de coordenadas ortonormal.
*/

void	get_camera_vectors(t_camera *camera, double aspect, t_viewport *view)
{
	double	theta;
	double	half_height;
	double	half_width;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	theta = camera->fov * M_PI / 180.0;
	half_height = tan(theta / 2.0);
	half_width = aspect * half_height;
	
	forward = vec_normalize(camera->direction);
	up = get_camera_up(forward);
	right = vec_normalize(vec_cross(forward, up));
	up = vec_cross(right, forward);
	
	view->origin = camera->position;
	view->horizontal = vec_scale(right, 2.0 * half_width);
	view->vertical = vec_scale(up, 2.0 * half_height);
	view->lower_left_corner = vec_sub(vec_sub(vec_add(camera->position, forward),
		vec_scale(view->horizontal, 0.5)), vec_scale(view->vertical, 0.5));
}

/*
** get_render_rows - Calcula el rango de filas a renderizar en un frame
**
** Descripción:
** Determina qué filas de la imagen se van a renderizar en el frame actual
** para el renderizado progresivo. Controla la cantidad de trabajo por frame
** para mantener la aplicación responsiva.
**
** Parámetros:
** - row: Fila actual desde donde continuar renderizando
** - rowsxframe: Número de filas a renderizar por frame
** - start_row: Puntero donde almacenar la fila inicial
** - end_row: Puntero donde almacenar la fila final
**
** Funcionamiento:
** 1. Establece start_row como la fila actual
** 2. Calcula end_row sumando filas por frame
** 3. Limita end_row al máximo de HEIGHT para evitar overflow
** 4. Permite controlar la velocidad vs responsividad del renderizado
*/
void	get_render_rows(int row, int rowsxframe, int *start_row, int *end_row)
{
	*start_row = row;
	*end_row = row + rowsxframe;
	if (*end_row > HEIGHT)
		*end_row = HEIGHT;
}
