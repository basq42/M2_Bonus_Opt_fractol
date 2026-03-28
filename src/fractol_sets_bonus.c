/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_sets_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:48:58 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/28 16:51:04 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol_bonus.h"

/*	Later for bonus do f->zoom + f->shift_* */
/*	Core equation: Z(n+1) = Z(n)^2 + c
 *	For mandelbrot Z always starts at 0
 *	and the constant c represents the actual coordinate
 *	of the pixel in complex plane
 *	Complex Number Squaring with i^2=-1:
 *		(x + iy)^2 = (x^2 - y^2) + (2xy)i
 *	Escape radius set to 2
 *		avoid slow sqrt() function
 *	*/
/*	
	while ((z.r * z.r) + (z.i * z.i) <= 4.0 && iter < f->max_iterations)
	{
		tmp_zr = (z.r * z.r) - (z.i * z.i) + c.r;
		z.i = (2.0 * z.r * z.i) + c.i;
		z.r = tmp_zr;
		iter++;
	}
	this was slow due to 6 multplctns per pixel
	instead now caching squared vals into new variables
	and hence down to 4 multiplications
 * */
/*	on top of periodicity, adding cardioid and bulb checks:
 *	a point is definetively inside the cardioid if:
 *		q * (q + (x - 0.25)) <= 0.25 * y^2
 *	a point is inside the circular bulb (left) definetively if:
 *	(x + 1)^2 + y^2 <= 0.0625
 * */
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

/*	Same core as mandelbrot but:
 *		constant c remains same for all pixels
 *		instead Z changes
 *		*/
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

/* The core equation is: Z(n+1) = (|Re(Z)| + i|Im(Z)|)^2 + c
	using fabs() to get absolute values
		real part is same as mandelbrot due to |x|^2=x^2
		img part in absolute
 */
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
