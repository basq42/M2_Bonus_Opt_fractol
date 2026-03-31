/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 15:18:25 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/31 13:07:45 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/fractol.h"

/*	mlx_scroll_hook expecects xdelta even though
 *	only using vertical scroll*/
/*	Implementing dynamic/adaptive zoom instead
 *	static
 * */
void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_fractal	*f;

	f = (t_fractal *)param;
	(void)xdelta;
	if (ydelta > 0)
		f->zoom *= 0.9;
	else if (ydelta < 0)
		f->zoom *= 1.1;
	scale_update(f);
	render_fractal(f);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_fractal	*f;

	f = (t_fractal *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(f->mlx);
}

void	size_hook(int32_t width, int32_t height, void *param)
{
	t_fractal	*f;

	f = (t_fractal *)param;
	f->width = width;
	f->height = height;
	mlx_resize_image(f->img, f->width, f->height);
	scale_update(f);
	render_fractal(f);
}
