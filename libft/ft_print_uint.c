/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:38:11 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/26 12:38:12 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putunbr_fd(unsigned int un, int fd)
{
	char	c;

	if (un >= 10)
		ft_putunbr_fd((un / 10), fd);
	c = (un % 10) + '0';
	ft_putchar_fd(c, fd);
}

int	ft_print_uint(unsigned int un)
{
	ft_putunbr_fd(un, 1);
	return ((int)ft_digit_count_uint(un));
}
