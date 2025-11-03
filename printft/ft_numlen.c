int	ft_numlen(int n)
{
	int	len;
	int	num;

	len = 0;
	num = n;
	if (num <= 0)
		len++;
	if (num < 0)
		num = -num;
	while (num > 0)
	{
		len++;
		num /= 10;
	}
	return (len);
}
