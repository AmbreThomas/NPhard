#include "graphe.h"

char	*ft_strnew(size_t size){
	char *str;
	size_t i;

	if ((str = (char*)malloc(size + 1)) == NULL)
		return (0);
	i = 0;
	while (i < size)
		str[i++] = 0;
	str[i] = 0;
	return (str);
}

size_t	get_str_len(int n){
	size_t i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char*	ft_itoa(int n){
	char*str;
	size_t str_len;
	unsigned int n_cpy;

	str_len = get_str_len(n);
	n_cpy = n;
	if (n < 0)
	{
		n_cpy = -n;
		str_len++;
	}
	if (!(str = ft_strnew(str_len)))
		return (0);
	str[--str_len] = n_cpy % 10 + '0';
	while (n_cpy /= 10)
		str[--str_len] = n_cpy % 10 + '0';
	if (n < 0)
		*(str + 0) = '-';
	return (str);
}

int		combinaisons(int n, int k)
{
	double C[k+1];
	int i;

	C[0] = 1;
	i = -1;
	while (++i < n)
		C[i + 1] = (C[i] * (n - i)) / (i + 1);
	return (int)(C[k]);
}
