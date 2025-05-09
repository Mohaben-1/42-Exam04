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

void	permutation(char *copy, int start_index, int end_index)
{
	int	i;

	if (start_index == end_index)
	{
		printf("%s\n", copy);
		return ;
	}
	i = start_index;
	while (i <= end_index)
	{
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
		return (printf("invalid nbr of args\n"), 1);
	copy = strdup(av[1]);
	if (!copy)
		return (printf("malloc failed\n"), 1);
	len = strlen(copy);
	permutation(copy, 0, len -1);
	free(copy);
	return (0);
}
