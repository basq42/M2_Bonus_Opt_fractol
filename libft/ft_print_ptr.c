/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:38:01 by bkelav            #+#    #+#             */
/*   Updated: 2026/03/26 12:38:02 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_put_ptr(unsigned long long addr)
{
	static char	*hex_digits;
	char		c;

	hex_digits = "0123456789abcdef";
	if (addr >= 16)
		ft_put_ptr(addr / 16);
	c = hex_digits[addr % 16];
	write (1, &c, 1);
}

int	ft_print_ptr(void *addr)
{
	unsigned long long	ptr_val;

	if (!addr)
		return (write(1, "(nil)", 5));
	ptr_val = (unsigned long long)addr;
	write (1, "0x", 2);
	ft_put_ptr(ptr_val);
	return ((int)ft_digit_count_ptr(ptr_val) + 2);
}
