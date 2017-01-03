#include <unistd.h>

void ft_putchar (char c)
{
		write(1, &c, 1);
}

void ft_initialize (int *tab, int n)
{
		int i;

		i = 0;
		while (i < n)
		{
				tab[i] = 0;
				i++;
		}
}

void ft_print_array (int *tab, int n)
{
		int i;

		i = 0;
		while (i < n)
		{
				ft_putchar (tab[i] + '0');
				i++;
		}
		if (tab[0] < 10 - n)
		{
				ft_putchar (',');
				ft_putchar (' ');
		}
}

void ft_print_combn_ (int n, int k, int *tab)
{
		int num;

		if (k == n)
				num = 0;
		else
				num = tab[n - k - 1] + 1;
		tab[n - k] = num;
		while (tab[n - k] <= 10 - k)
		{
				if (k == 1)
						ft_print_array(tab, n);
				else
						ft_print_combn_(n, k - 1, tab);
				tab[n - k]++;
		}
}

void ft_print_combn (int n)
{
		int k ;
		int tab[n];

		k = n;
		ft_initialize (tab, n);
		ft_print_combn_(n, k, tab);
}

int		combinaisons(int n, int k)
{
		double C[k+1];
		int i;

		C[0] = 1;
		i = -1;
		while (++i < n)
				C[i + 1] = (C[i] * (n - i)) / (i + 1)
		return (int)(C[k]);
}

#include <stdio.h>

int		main(void)
{
		printf("%d\n", combinaisons(10, 8));
		return (1);
}
