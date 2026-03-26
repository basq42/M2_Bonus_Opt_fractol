/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_hooks_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 15:18:25 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/26 15:32:16 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/fractol_bonus.h"

/*	mlx_scroll_hook expecects xdelta even though
 *	only using vertical scroll*/
/*	Implementing dynamic/adaptive zoom instead
 *	static
 * */
void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_fractal	*f;
	double		mouse_r;
	double		mouse_i;
	int32_t		x;
	int32_t		y;

	f = (t_fractal *)param;
	(void)xdelta;
	mlx_get_mouse_pos(f->mlx, &x, &y);
	mouse_r = (x - f->width / 2.0) * f->scale_x + f->shift_x;
	mouse_i = (y - f->height / 2.0) * f->scale_y + f->shift_y;
	if (ydelta > 0)
		f->zoom *= 0.9;
	else if (ydelta < 0)
		f->zoom *= 1.1;
	f->scale_x = f->zoom / (f->width / 4.0);
	f->scale_y = f->zoom / (f->height / 4.0);
	f->shift_x = mouse_r - ((x - f->width / 2.0) * f->scale_x);
	f->shift_y = mouse_i - ((y - f->height / 2.0) * f->scale_y);
	render_fractal(f);
}

/* */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_fractal	*f;

	f = (t_fractal *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(f->mlx);
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
	{
		f->scheme = (f->scheme + 1) % 3;
		render_fractal(f);
	}
}

void	size_hook(int32_t width, int32_t height, void *param)
{
	t_fractal	*f;

	f = (t_fractal *)param;
	f->width = width;
	f->height = height;
	mlx_resize_image(f->img, f->width, f->height);
	f->scale_x = f->zoom / (f->width / 4.0);
	f->scale_y = f->zoom / (f->height / 4.0);
	render_fractal(f);
}
