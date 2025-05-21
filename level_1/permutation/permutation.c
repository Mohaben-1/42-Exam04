#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	ft_swap(char *c1, char *c2)
{
	char	tmp;

	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

void	ft_sort(char *s)
{
	int	i;
	int	j;

	i = 0;
	while (s[i])
	{
		j = i + 1;
		while (s[j])
		{
			if (s[i] > s[j])
				ft_swap(&s[i], &s[j]);
			j++;
		}
		i++;
	}
}

void	permutation(char *copy, int start_index, int end_index)
{
	int	i;

	if (start_index == end_index)
	{
		puts(copy);
		return ;
	}
	i = start_index;
	while (i <= end_index)
	{
		ft_sort(&copy[start_index]);
		ft_swap(&copy[start_index], &copy[i]);
		permutation(copy, start_index + 1, end_index);
		ft_swap(&copy[start_index], &copy[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	char	*copy;
	int		len;

	if (ac != 2)
		return (1);
	copy = strdup(av[1]);// not allowed!
	if (!copy)
		return (1);
	len = strlen(copy);// not allowed!
	permutation(copy, 0, len -1);
	free(copy);
	return (0);
}
