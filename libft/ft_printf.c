#include "libft.h"

static int	ft_exec_conversion(char spec_letter, va_list ap)
{
	if (spec_letter == '%')
		return (write(1, "%", 1));
	else if (spec_letter == 'c')
	{
		ft_putchar_fd(va_arg(ap, int), 1);
		return (1);
	}
	else if (spec_letter == 's')
		return (ft_print_str(va_arg(ap, char *)));
	else if (spec_letter == 'p')
		return (ft_print_ptr(va_arg(ap, void *)));
	else if (spec_letter == 'd' || spec_letter == 'i')
		return (ft_print_int(va_arg(ap, int)));
	else if (spec_letter == 'u')
		return (ft_print_uint(va_arg(ap, unsigned int)));
	else if (spec_letter == 'x')
		return (ft_print_hex(va_arg(ap, unsigned int), 0));
	else if (spec_letter == 'X')
		return (ft_print_hex(va_arg(ap, unsigned int), 1));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	va_list	ap;

	if (!format)
		return (0);
	len = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			len += ft_exec_conversion(*(format + 1), ap);
			format += 2;
		}
		else
		{
			len += write(1, format, 1);
			format++;
		}
	}
	va_end(ap);
	return (len);
}
/*
 * short curcuit -> && *(++format) caused SIGSEGV
 * probably due to incrementing error
 * reading past the end of the string
int	ft_printf(const char *format, ...)
{
	int	len;
	va_list	ap;

	if (!format)
		return (0);
	len = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%' && *(++format))
			len += ft_exec_conversion(*format, ap);
		else
		{
			len += write(1, format, 1);
			format++;
		}
	}
	va_end(ap);
	return (len);
}
*/
