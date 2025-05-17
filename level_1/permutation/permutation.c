#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s, int len)
{
	char	*dup;
	int		i;

	if (!s)
		return (NULL);
	dup = malloc(len);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

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

void	solve(char *copy, char *perm, int *used, int pos, int len)
{
	int	i;

	if (pos == len)
	{
		perm[len] = '\0';
		puts(perm);
		return ;
	}
	i = 0;
	while (i < len)
	{
		if (!used[i])
		{
			used[i] = 1;
			perm[pos] = copy[i];
			solve(copy, perm, used, pos + 1, len);
			used[i] = 0;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	char	*copy;
	char	*perm;
	int		*used;
	int		len;
	int		i;

	if (ac != 2)
		return (1);
	len = ft_strlen(av[1]);
	copy = ft_strdup(av[1], len + 1);
	perm = malloc(len + 1);
	used = calloc(len, sizeof(int));
	if (!copy || !used || !perm)
		return (1);
	ft_sort(copy);
	solve(copy, perm, used, 0, len);
	free(copy);
	free(perm);
	free(used);
	return (0);
}
