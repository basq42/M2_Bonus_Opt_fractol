/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:39:05 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/26 12:39:06 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * recursively call until number is < 16
 * to isolate leftmost digits
 * n % 16 to get the current hex digit after /
 * depending on the flag either up or low
 * use bool if allowed?
 */
static void	ft_put_hex(unsigned int un, int upper_case)
{
	char		c;
	const char	*digits;

	if (upper_case)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	if (un >= 16)
		ft_put_hex((un / 16), upper_case);
	c = digits[un % 16];
	write(1, &c, 1);
}

int	ft_print_hex(unsigned int un, int upper_case)
{
	ft_put_hex(un, upper_case);
	return ((int)ft_digit_count_hex(un));
}
