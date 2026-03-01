#include "ft_printf.h"

void	ft_print_spaces_left(t_printf *rules, int len, long num)
{
	if (rules->hash == 1 && rules->zero == 0)
		rules->width = rules->width - 2;
	if (rules->space == 1)
		rules->width = rules->width - 1;
	if (len > rules->precision)
	{
		rules->width = rules->width - len;
		ft_left_cs_int(rules, num);
	}
	else
	{
		rules->width = rules->width - rules->precision;
		ft_left_cs_int(rules, num);
	}
}

void	ft_left_cs_int(t_printf *tab, long num)
{
	int	i;

	(void)num;
	i = tab->width;
	if (tab->zero == 0)
	{
		while (i > 0)
		{
			tab->total_lenght += write(1, " ", 1);
			i--;
		}
	}
	
}