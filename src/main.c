/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcampas- <dcampas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:56:06 by ebalana-          #+#    #+#             */
/*   Updated: 2025/09/01 13:48:33 by dcampas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

#define WIDTH 800
#define HEIGHT 600
#define WHITE 0xFFFFFFFF
#define BLACK 0x000000FF

// Función para manejar errores
static void ft_error(void)
{
	fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// Función para dibujar un pixel
static void put_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
		mlx_put_pixel(img, x, y, color);
}

// Función simple para dibujar un rectángulo (para formar letras)
static void draw_rect(mlx_image_t *img, int x, int y, int w, int h, uint32_t color)
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			put_pixel(img, x + j, y + i, color);
		}
	}
}

// Función para dibujar texto simple "MLX SETUP"
static void draw_text(mlx_image_t *img)
{
	int start_x = WIDTH / 2 - 120;
	int start_y = HEIGHT / 2 - 30;
	// int letter_width = 20;
	int letter_height = 40;
	int spacing = 25;

	// M
	draw_rect(img, start_x, start_y, 4, letter_height, WHITE);
	draw_rect(img, start_x + 16, start_y, 4, letter_height, WHITE);
	draw_rect(img, start_x + 4, start_y, 12, 4, WHITE);
	draw_rect(img, start_x + 8, start_y + 4, 4, 16, WHITE);

	// L
	start_x += spacing;
	draw_rect(img, start_x, start_y, 4, letter_height, WHITE);
	draw_rect(img, start_x, start_y + 36, 16, 4, WHITE);

	// X
	start_x += spacing;
	for (int i = 0; i < letter_height; i++)
	{
		put_pixel(img, start_x + i / 2, start_y + i, WHITE);
		put_pixel(img, start_x + i / 2 + 1, start_y + i, WHITE);
		put_pixel(img, start_x + 16 - i / 2, start_y + i, WHITE);
		put_pixel(img, start_x + 16 - i / 2 - 1, start_y + i, WHITE);
	}

	// Espacio
	start_x += spacing + 10;

	// S
	draw_rect(img, start_x, start_y, 16, 4, WHITE);
	draw_rect(img, start_x, start_y, 4, 18, WHITE);
	draw_rect(img, start_x, start_y + 18, 16, 4, WHITE);
	draw_rect(img, start_x + 12, start_y + 22, 4, 18, WHITE);
	draw_rect(img, start_x, start_y + 36, 16, 4, WHITE);

	// E
	start_x += spacing;
	draw_rect(img, start_x, start_y, 4, letter_height, WHITE);
	draw_rect(img, start_x, start_y, 16, 4, WHITE);
	draw_rect(img, start_x, start_y + 18, 12, 4, WHITE);
	draw_rect(img, start_x, start_y + 36, 16, 4, WHITE);

	// T
	start_x += spacing;
	draw_rect(img, start_x, start_y, 20, 4, WHITE);
	draw_rect(img, start_x + 8, start_y, 4, letter_height, WHITE);

	// U
	start_x += spacing;
	draw_rect(img, start_x, start_y, 4, 36, WHITE);
	draw_rect(img, start_x + 16, start_y, 4, 36, WHITE);
	draw_rect(img, start_x, start_y + 36, 20, 4, WHITE);

	// P
	start_x += spacing;
	draw_rect(img, start_x, start_y, 4, letter_height, WHITE);
	draw_rect(img, start_x, start_y, 16, 4, WHITE);
	draw_rect(img, start_x + 12, start_y, 4, 22, WHITE);
	draw_rect(img, start_x, start_y + 18, 16, 4, WHITE);
}

// Hook de teclas para cerrar con ESC
static void key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t *mlx = param;

	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

int main(void)
{
	mlx_t *mlx;
	mlx_image_t *img;

	// Inicializar MLX
	mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!mlx)
		ft_error();

	// Crear una imagen del tamaño de la ventana
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(mlx);
		ft_error();
	}

	// Llenar fondo negro
	for (uint32_t y = 0; y < img->height; y++)
	{
		for (uint32_t x = 0; x < img->width; x++)
		{
			mlx_put_pixel(img, x, y, BLACK);
		}
	}

	// Dibujar el texto
	draw_text(img);

	// Mostrar la imagen en la ventana
	if (mlx_image_to_window(mlx, img, 0, 0) == -1)
	{
		mlx_terminate(mlx);
		ft_error();
	}

	// Configurar hook de teclado
	mlx_key_hook(mlx, key_hook, mlx);

	// Iniciar el loop
	mlx_loop(mlx);

	// Limpiar recursos
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}