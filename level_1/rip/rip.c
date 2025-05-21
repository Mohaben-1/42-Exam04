#include <unistd.h>
#include <stdio.h>

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	is_valid(char *str)
{
	int	balanced;
	int	i;

	balanced = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			balanced++;
		else if (str[i] == ')')
		{
			balanced--;
			if (balanced < 0)
				return (0);
		}
		i++;
	}
	return (balanced == 0);
}

void	find_min_removes(char *str, int *min, int index, int current_removal)
{
	char	saved;
	int		i;

	if (current_removal > *min)
		return ;
	if (is_valid(str))
	{
		if (current_removal < *min)
			*min = current_removal;
		return ;
	}
	i = index;
	while (str[i])
	{
		if (str[i] != '(' && str[i] != ')')
		{
			i++;
			continue ;
		}
		saved = str[i];
		str[i] = ' ';
		find_min_removes(str, min, i + 1, current_removal + 1);
		str[i] = saved;
		i++;
	}
}

void	generate_valide(char *str, int min, int index, int current_removal)
{
	char	saved;
	int	i;

	if (current_removal > min)
		return ;
	if (is_valid(str) && current_removal == min)
	{
		puts(str);
		return ;
	}
	i = index;
	while (str[i])
	{
		if (str[i] != '(' && str[i] != ')')
		{
			i++;
			continue ;
		}
		saved = str[i];
		str[i] = ' ';
		generate_valide(str, min, i + 1, current_removal + 1);
		str[i] = saved;
		i++;
	}
}

int	main(int ac, char **av)
{
	int	min;

	if (ac != 2 || !av[1][0])
		return (1);
	min = ft_strlen(av[1]);
	find_min_removes(av[1], &min, 0, 0);
	generate_valide(av[1], min, 0, 0);
	return (0);
}
