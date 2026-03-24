/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 13:09:11 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/24 14:45:10 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/fractol.h"
#include <stdio.h>

/* higher multiplier = more rapid colour cycle
* =========================================================================
* BITWISE COLOR SHIFT:
* uint32_t has 32 binary slots, grouped into 4 bytes:
* [  Byte 4  ] [  Byte 3  ] [  Byte 2  ] [  Byte 1  ]
* [ 00000000 ] [ 00000000 ] [ 00000000 ] [ 00000000 ]  (Empty 32-bit int)
* Let's say calculated colours are:
* r = 0xAA (Red), g = 0xBB (Green), b = 0xCC (Blue), Alpha = 0xFF (Solid)
* STEP 1: (<<)
* Shifting moves the 8 bits of color data to the left by N spaces.
* r << 24 pushes Red 24 bits left, slotting into Byte 4:
* [ AAAAAAAA ] [ 00000000 ] [ 00000000 ] [ 00000000 ]
* g << 16 pushes Green 16 bits left, slotting into Byte 3:
* [ 00000000 ] [ BBBBBBBB ] [ 00000000 ] [ 00000000 ]
* b << 8 pushes Blue 8 bits left, slotting into Byte 2:
* [ 00000000 ] [ 00000000 ] [ CCCCCCCC ] [ 00000000 ]
* 0xFF (Alpha) is already in Byte 1, no shift:
* [ 00000000 ] [ 00000000 ] [ 00000000 ] [ 11111111 ]
* STEP 2: (|)
* The OR operator compares the rows vertically. Since colours are isolated in
* separate bytes, they snap together without corrupting each other:
* | [ AAAAAAAA ] [ 00000000 ] [ 00000000 ] [ 00000000 ]  (r << 24)
* | [ 00000000 ] [ BBBBBBBB ] [ 00000000 ] [ 00000000 ]  (g << 16)
* | [ 00000000 ] [ 00000000 ] [ CCCCCCCC ] [ 00000000 ]  (b << 8)
* | [ 00000000 ] [ 00000000 ] [ 00000000 ] [ 11111111 ]  (0xFF)
* -----------------------------------------------------
* = [ AAAAAAAA ] [ BBBBBBBB ] [ CCCCCCCC ] [ 11111111 ]  (Final RGBA Pixel)
* = 0xAABBCCFF
* =========================================================================
*/
uint32_t	get_colour(int iterations, int max_iterations)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	if (iterations == max_iterations)
		return (0x000000FF);
	r = (iterations * 5) % 256;
	g = (iterations * 7) % 256;
	b = (iterations * 11) % 256;
	return (r << 24 | g << 16 | b << 8 | 0xFF);
}

/* mlx_put_pixel checks bounds each call
 * but while loops already make sure x-y are inbound
 * Mlx stores img->pixels buffer in 1D array
 * and each pixel has 4 colour channels, each a byte
 * */
void	opt_pixel_put(mlx_image_t *img, int x, int y, uint32_t colour)
{
	int	offset;

	offset = (y * img->width + x) *4;
	img->pixels[offset] = (colour >> 24) & 0xFF;
	img->pixels[offset + 1] = (colour >> 16) & 0xFF;
	img->pixels[offset + 3] = (colour) & 0xFF;
}

void	render_fractal(t_fractal *f)
{
	int				x;
	int				y;
	struct timeval	start;
	struct timeval	end;
	long			ms;

	gettimeofday(&start, NULL);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (f->fractal_flg == 1)
				calc_julia(f, x, y);
			else if (f->fractal_flg == 0)
				calc_mandelbrot(f, x, y);
			else if (f->fractal_flg == 2)
				calc_burning_ship(f, x, y);
			x++;
		}
		y++;
	}
	gettimeofday(&end, NULL);
	ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	printf("Render time: %ld ms\n", ms);
	ui_update(f);
}
