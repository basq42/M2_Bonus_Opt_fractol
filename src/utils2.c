/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 13:28:18 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/25 12:52:05 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/fractol.h"

static int	fill_int(long int_p, char *str)
{
	int		len;
	long	tmp;

	len = 1;
	tmp = int_p;
	while (tmp >= 10)
	{
		tmp /= 10;
		len++;
	}
	tmp = len;
	while (len-- > 0)
	{
		str[len] = (int_p % 10) + '0';
		int_p /= 10;
	}
	return (tmp);
}

char	*ft_ftoa(double n, int precision)
{
	char	*str;
	int		i;

	str = malloc(32);
	if (!str)
		return (NULL);
	i = 0;
	if (n < 0.0)
	{
		str[i++] = '-';
		n = -n;
	}
	i += fill_int((long)n, str + i);
	if (precision > 0)
		str[i++] = '.';
	n -= (double)(long)n;
	while (precision-- > 0)
	{
		n *= 10.0;
		str[i++] = (int)n + '0';
		n -= (int)n;
	}
	str[i] = '\0';
	return (str);
}

void	rand_julia_gen(t_fractal *f)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	f->julia_cx = ((tv.tv_usec % 4000) - 2000) / 1000.0;
	f->julia_cy = (((tv.tv_usec / 10) % 4000) - 2000) / 1000.0;
}
