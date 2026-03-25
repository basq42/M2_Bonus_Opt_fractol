/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:48:50 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/25 16:23:08 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

static void	print_exit(void)
{
	ft_putstr_fd("|Options:\n", 1);
	ft_putstr_fd("	mandelbrot\n", 1);
	ft_putstr_fd("	julia\n", 1);
	ft_putstr_fd("	burning_ship\n", 1);
	ft_putstr_fd("|Usage:\n", 1);
	ft_putstr_fd(" |Mandelbrot:\n", 1);
	ft_putstr_fd("./fractol mandelbrot\n", 1);
	ft_putstr_fd(" |Burning Ship:\n", 1);
	ft_putstr_fd("./fractol burning_ship\n", 1);
	ft_putstr_fd(" |Julia:\n", 1);
	ft_putstr_fd("./fractol julia <real> <imaginary>\n", 1);
	ft_putstr_fd("Some Examples for Julia:\n", 1);
	ft_putstr_fd("Basilica   : -1.0   0.0\n", 1);
	ft_putstr_fd("Dendrite   :  0.0   1.0\n", 1);
	ft_putstr_fd("Douady's   : -0.123 0.745\n", 1);
	ft_putstr_fd("Fatou Dust :  0.285 0.01\n", 1);
	ft_putstr_fd("Snowflake  : -0.8   0.156\n", 1);
	exit(1);
}

static void	init_fractal(t_fractal *f)
{
	f->width = 800;
	f->height = 800;
	f->zoom = 1.0;
	f->shift_x = 0.0;
	f->shift_y = 0.0;
	f->max_iterations = 100;
	f->text_img = NULL;
	f->mlx = mlx_init(f->width, f->height, "fract'ol", true);
	if (!f->mlx)
	{
		ft_printf("%s\n", mlx_strerror(mlx_errno));
		exit(1);
	}
	f->img = mlx_new_image(f->mlx, f->width, f->height);
	if (!f->img || (mlx_image_to_window(f->mlx, f->img, 0, 0) < 0))
	{
		ft_printf("%s\n", mlx_strerror(mlx_errno));
		mlx_terminate(f->mlx);
		exit(1);
	}
	f->scale_x = f->zoom / (f->width / 4.0);
	f->scale_y = f->zoom / (f->height / 4.0);
}

static void	which_fractal(int argc, char **argv, t_fractal *fractal)
{
	if (argc == 2 && !ft_strncmp(argv[1], "mandelbrot", 11))
		fractal->fractal_flg = 0;
	else if (argc == 4 && !ft_strncmp(argv[1], "julia", 6))
	{
		fractal->fractal_flg = 1;
		fractal->julia_cx = ft_atod(argv[2]);
		fractal->julia_cy = ft_atod(argv[3]);
	}
	else if (argc == 2 && !ft_strncmp(argv[1], "burning_ship", 13))
		fractal->fractal_flg = 2;
	else
		print_exit();
}

int	main(int argc, char **argv)
{
	t_fractal	fractal;

	ft_memset(&fractal, 0, sizeof(t_fractal));
	which_fractal(argc, argv, &fractal);
	init_fractal(&fractal);
	render_fractal(&fractal);
	mlx_scroll_hook(fractal.mlx, scroll_hook, &fractal);
	mlx_resize_hook(fractal.mlx, size_hook, &fractal);
	mlx_key_hook(fractal.mlx, key_hook, &fractal);
	mlx_loop(fractal.mlx);
	mlx_terminate(fractal.mlx);
	return (0);
}
