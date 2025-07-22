/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:18:34 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/22 16:58:57 by ebalana-         ###   ########.fr       */
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
** get_camera_vectors - Configura los vectores de la cámara para ray tracing
**
** Descripción:
** Calcula los vectores necesarios para la proyección de la cámara:
** origen, direcciones horizontales/verticales y esquina inferior izquierda
** del viewport. Establece el sistema de coordenadas para lanzar rayos.
**
** Parámetros:
** - camera: Estructura con posición y configuración de la cámara
** - aspect: Relación de aspecto de la imagen (ancho/alto)
** - view: Estructura viewport donde se almacenan los vectores resultantes
**
** Funcionamiento:
** 1. Define el tamaño del viewport basado en aspect ratio
** 2. Establece el origen en la posición de la cámara
** 3. Calcula vectores horizontales y verticales del viewport
** 4. Determina la esquina inferior izquierda para el ray casting
**
** Nota: Actualmente usa configuración simplificada (cámara mira hacia -Z)
*/
void	get_camera_vectors(t_camera *camera, double aspect, t_viewport *view)
{
	double	viewport_height;
	double	viewport_width;
	double	focal_length;

	viewport_height = 1.0;
	viewport_width = aspect * viewport_height;
	focal_length = 1.0;
	view->origin = camera->position;
	view->horizontal = vec3(viewport_width, 0, 0);
	view->vertical = vec3(0, viewport_height, 0);
	view->lower_left_corner = vec_sub(vec_sub(vec_sub(view->origin, \
		vec_scale(view->horizontal, 0.5)), vec_scale(view->vertical, 0.5)), \
		vec3(0, 0, focal_length));
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
