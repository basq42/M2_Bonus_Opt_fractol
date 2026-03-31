/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot_set_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:48:58 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/31 13:06:19 by bkelav           ###   ########.fr       */
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

static void	period_check(t_complex *z, t_complex *old, int *iter, t_fractal *f)
{
	if (z->r == old->r && z->i == old->i)
		*iter = f->max_iterations - 1;
	if ((*iter & 31) == 0)
		*old = *z;
}

static void	map_complex(t_fractal *f, int x, int y, t_complex *c)
{
	c->r = (x - f->width / 2.0) * f->scale_x + f->shift_x;
	c->i = (y - f->height / 2.0) * f->scale_y + f->shift_y;
}

void	calc_mandelbrot(t_fractal *f, int x, int y)
{
	t_complex	c;
	t_complex	z;
	t_complex	z2;
	t_complex	old;
	int			iter;

	map_complex(f, x, y, &c);
	if (mandel_checks(c.r, c.i))
		return (opt_pixel_put(f->img, x, y, 0x000000FF));
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
		period_check(&z, &old, &iter, f);
		iter++;
	}
	opt_pixel_put(f->img, x, y, get_colour(iter, f->max_iterations, f->scheme));
}
