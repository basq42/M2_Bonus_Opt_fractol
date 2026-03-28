/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:49:23 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/28 14:38:17 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_BONUS_H
# define FRACTOL_BONUS_H

# include <MLX42/MLX42.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"
# include <sys/time.h>

/*
# define WIDTH 800
# define HEIGHT 800
*/
/* Struct for image buffering 
 * bits-per-pixel usually 32? 
 * Endianess indicates how color bytes are stored
 * may not need with MLX42
typdef struct	s_img {
	void	*img_ptr;
	char	*pixels;
	int		bits_ppixel;
	int		line_len;
	int		endian;
}	t_img;
*/
/*	shift_* for bonus, for mandatory keep at 0.0 */
typedef struct s_fractal
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			fractal_flg;
	double		julia_cx;
	double		julia_cy;
	double		zoom;
	double		shift_x;
	double		shift_y;
	int			max_iterations;
	double		scale_x;
	double		scale_y;
	mlx_image_t	*text_img;
	int32_t		width;
	int32_t		height;
	int			scheme;
}	t_fractal;

typedef struct s_complex
{
	double	r;
	double	i;
}	t_complex;

/*	fractol_sets.c	*/
void		calc_mandelbrot(t_fractal *f, int x, int y);
void		calc_julia(t_fractal *f, int x, int y);
void		calc_burning_ship(t_fractal *f, int x, int y);
/*	render.c	*/
void		opt_pixel_put(mlx_image_t *img, int x, int y, uint32_t colour);
void		pick_scheme(uint8_t *r, uint8_t *g, uint8_t *b, int iter, int scheme);
uint32_t	get_colour(int iterations, int max_iterations, int scheme);
void		render_fractal(t_fractal *f);
void		scroll_hook(double xdelta, double ydelta, void *param);
void		key_hook(mlx_key_data_t keydata, void *param);
void		size_hook(int32_t width, int32_t height, void *param);
/*	utils	*/
double		ft_atod(const char *str);
//void		ui_update(t_fractal *f);
//char		*ft_ftoa(double n, int precision);
//void		rand_julia_gen(t_fractal *f);
#endif
