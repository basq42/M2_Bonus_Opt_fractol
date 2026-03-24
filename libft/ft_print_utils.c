#include "libft.h"

int	ft_digit_count_int(int n)
{
	int	count;
	int	tmp;

	count = 0;
	tmp = n;
	if (n <= 0)
		count = 1;
	while (tmp)
	{
		tmp /= 10;
		count++;
	}
	return (count);
}

size_t	ft_digit_count_uint(unsigned int un)
{
	size_t		count;
	unsigned int	tmp;

	count = 0;
	tmp = un;
	if (un == 0)
		count = 1;
	while (tmp)
	{
		tmp /= 10;
		count++;
	}
	return (count);
}

size_t	ft_digit_count_hex(unsigned int un)
{
	size_t	count;
	unsigned int	tmp;

	count = 0;
	if (un == 0)
		return (1);
	tmp = un;
	while (tmp)
	{
		tmp /= 16;
		count++;
	}
	return (count);
}

/*
 * as ptr values change between 32bit 64bit archs,
 * long long is better to not have conv issues
 */
size_t	ft_digit_count_ptr(unsigned long long ulln)
{
	size_t	count;
	unsigned long long	tmp;

	count = 0;
	if (ulln == 0)
		return (1);
	tmp = ulln;
	while (tmp)
	{
		tmp /= 16;
		count++;
	}
	return (count);
}
