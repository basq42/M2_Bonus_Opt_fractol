/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:49:23 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/30 15:55:55 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_BONUS_H
# define FRACTOL_BONUS_H

# include <MLX42/MLX42.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"

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

/*	fractol_sets_bonus.c	*/
void		calc_mandelbrot(t_fractal *f, int x, int y);
void		calc_julia(t_fractal *f, int x, int y);
void		calc_burning_ship(t_fractal *f, int x, int y);
/*	render_bonus.c	*/
void		opt_pixel_put(mlx_image_t *img, int x, int y, uint32_t colour);
uint32_t	get_colour(int iterations, int max_iterations, int scheme);
void		render_fractal(t_fractal *f);
void		scroll_hook(double xdelta, double ydelta, void *param);
void		key_hook(mlx_key_data_t keydata, void *param);
void		size_hook(int32_t width, int32_t height, void *param);
/*	utils_bonus.c	*/
double		ft_atod(const char *str);
void		scale_update(t_fractal *f);

#endif
