/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_sets_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:48:58 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/30 14:12:57 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol_bonus.h"

static int	mandel_checks(double cr, double ci)
{
	double	q;
	double	cr_minus;
	double	ci_sq;

	ci_sq = ci * ci;
	cr_minus = cr - 0.25;
	q = (cr_minus * cr_minus) + ci_sq;
	if (q * (q + cr_minus) <= 0.25 * ci_sq)
		return (1);
	if ((cr + 1.0) * (cr + 1.0) + ci_sq <= 0.0625)
		return (1);
	return (0);
}

void	calc_mandelbrot(t_fractal *f, int x, int y)
{
	t_complex	c;
	t_complex	z;
	t_complex	z2;
	t_complex	old;
	int			iter;

	c.r = (x - f->width / 2.0) * f->scale_x + f->shift_x;
	c.i = (y - f->height / 2.0) * f->scale_y + f->shift_y;
	if (mandel_checks(c.r, c.i))
	{
		opt_pixel_put(f->img, x, y, 0x000000FF);
		return ;
	}
	z = (t_complex){0, 0};
	z2 = (t_complex){0, 0};
	old = (t_complex){0, 0};
	iter = 0;
	while ((z2.r + z2.i <= 4.0) && iter < f->max_iterations)
	{
		z.i = (2.0 * z.r * z.i) + c.i;
		z.r = z2.r - z2.i + c.r;
		z2.r = z.r * z.r;
		z2.i = z.i * z.i;
		if (z.r == old.r && z.i == old.i)
			iter = f->max_iterations - 1;
		if ((iter & 31) == 0)
			old = z;
		iter++;
	}
	opt_pixel_put(f->img, x, y, get_colour(iter, f->max_iterations, f->scheme));
}

/*	Symetry check for julia?
 *	if (f->shift_x == 0.0 && f->shift_y == 0.0)
		opt_pixel_put(f->img, f->width - 1 - x, f->height - 1 - y, f->scheme);
*/
void	calc_julia(t_fractal *f, int x, int y)
{
	t_complex	z;
	t_complex	z2;
	t_complex	old;
	int			iter;

	z.r = (x - f->width / 2.0) * f->scale_x + f->shift_x;
	z.i = (y - f->height / 2.0) * f->scale_y + f->shift_y;
	z2 = (t_complex){z.r * z.r, z.i * z.i};
	old = (t_complex){0, 0};
	iter = 0;
	while (z2.r + z2.i <= 4.0 && iter < f->max_iterations)
	{
		z.i = (2.0 * z.r * z.i) + f->julia_cy;
		z.r = z2.r - z2.i + f->julia_cx;
		z2.r = z.r * z.r;
		z2.i = z.i * z.i;
		if (z.r == old.r && z.i == old.i)
			iter = f->max_iterations - 1;
		if ((iter & 31) == 0)
			old = z;
		iter++;
	}
	opt_pixel_put(f->img, x, y, get_colour(iter, f->max_iterations, f->scheme));
}

void	calc_burning_ship(t_fractal *f, int x, int y)
{
	t_complex	c;
	t_complex	z;
	t_complex	z2;
	t_complex	old;
	int			iter;

	c.r = (x - f->width / 2.0) * f->scale_x + f->shift_x;
	c.i = (y - f->height / 2.0) * f->scale_y + f->shift_y;
	z = (t_complex){0, 0};
	z2 = (t_complex){0, 0};
	old = (t_complex){0, 0};
	iter = 0;
	while ((z2.r + z2.i <= 4.0) && iter < f->max_iterations)
	{
		z.i = 2.0 * fabs(z.r * z.i) + c.i;
		z.r = z2.r - z2.i + c.r;
		z2.r = z.r * z.r;
		z2.i = z.i * z.i;
		if (z.r == old.r && z.i == old.i)
			iter = f->max_iterations - 1;
		if ((iter & 31) == 0)
			old = z;
		iter++;
	}
	opt_pixel_put(f->img, x, y, get_colour(iter, f->max_iterations, f->scheme));
}
