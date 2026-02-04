#include "ft_printf.h"

void	start_stack(t_printf *head)
{
	head->args = 0;
	head->width = 0;
	head->precision - 0;
	head->zero = 0;
	head->point = 0;
	head->dash = 0;
	head->total_lenght = 0;
	head->sign = 0;
	head->is_zero = 0;
	head->percent = 0;
	head->sp = 0;
}

int parse_format(const char *s, int i, t_printf *f)
{
	init_flags(f);

	while (s[i] == '-' || s[i] == '0' || s[i] == '+' || s[i] == ' ' || s[i] == '#')
	{
		if (s[i] == '-')
			f->dash = 1;
		if (s[i] == '0')
			f->zero = 1;
		if (s[i] == '+')
			f->plus = 1;
		if (s[i] == ' ')
			f->space = 1;
		if (s[i] == '#')
			f->hash = 1;
		i++;
	}
	if (s[i] >= '0' && s[i] <= '9')
	{
		f->width = 0;
		while (s[i] >= '0' && s[i] <= '9')
		{
			f->width = f->width * 10 + (s[i] - '0');
			i++;
		}
	}
	if (s[i] == '.')
	{
		i++;
		f->precision = 0;
		while (s[i] >= '0' && s[i] <= '9')
		{
			f->precision = f->precision * 10 + (s[i] - '0');
			i++;
		}
	}
	return (i);
}