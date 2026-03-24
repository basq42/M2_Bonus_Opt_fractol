/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 15:03:04 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/24 13:35:35 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/fractol.h"

static void	skip_ws(const char *str, int *i)
{
	while (str[*i] == ' ' || (str[*i] >= '\t' && str[*i] <= '\r'))
		(*i)++;
}

static void	find_sign(const char *str, int *i, int *sign)
{
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

static void	precision(const char *str, int *i, double *result, double *fraction)
{
	if (str[*i] == '.')
	{
		(*i)++;
		while (str[*i] >= '0' && str[*i] <= '9')
		{
			*result = (*result * 10.0) + (str[*i] - '0');
			*fraction *= 10.0;
			(*i)++;
		}
	}
}

double	ft_atod(const char *str)
{
	double	result;
	double	fraction;
	int		sign;
	int		i;

	result = 0.0;
	fraction = 1.0;
	sign = 1;
	i = 0;
	skip_ws(str, &i);
	find_sign(str, &i, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10.0) + (str[i] - '0');
		i++;
	}
	precision(str, &i, &result, &fraction);
	return ((result / fraction) * sign);
}

void	ui_update(t_fractal *f)
{
	char	*zoom_str;
	char	*ui_str;

	if (f->text_img)
		mlx_delete_image(f->mlx, f->text_img);
	zoom_str = ft_ftoa(f->zoom, 10);
	ui_str = ft_strjoin("Zoom: ", zoom_str);
	f->text_img = mlx_put_string(f->mlx, ui_str, 10, 10);
	free(zoom_str);
	free(ui_str);
}
