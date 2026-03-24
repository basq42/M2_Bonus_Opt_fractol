#include "libft.h"

/*
 * int instead of size_t as even for long long max dgts ~20
 * not neccesarry for this but better practice to create tmp?
 */
int	ft_print_int(int n)
{
	if (n == INT_MIN)
	{
		return (write(1, INT_MIN_STR, INT_MIN_LEN));
	}
	ft_putnbr_fd(n, 1);
	return (ft_digit_count_int(n));
}
