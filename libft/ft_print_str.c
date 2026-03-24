#include "libft.h"

/*static size_t	ft_test_strlen(const char *s)
{
	const char	*p;

	p = s;
	while (*p)
		p++;
	return (p - s);
}
*/
int	ft_print_str(const char *str)
{
	if (!str)
		return (write(1, "(null)", 6));
	return (write(1, str, ft_strlen(str)));
}
/*
#include <stdio.h>

int	main(void)
{
	const char	*s;
	const char	*s1;

	s = NULL;
	s1 = "This Is a String";
	printf("NULL STR\n");
	printf("\nCount: %i\n", ft_print_str(s));
	printf("STR\n");
	printf("\nCount: %i\n", ft_print_str(s1));
	return (1);
}*/
