#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	print_queens(int *board, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		fprintf(stdout, "%d", board[i]);
		if (i < n - 1)
			fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

int	is_safe(int *board, int row, int col)
{
	int	prev_col;
	int	prev_row;

	prev_col = 0;
	while (prev_col < col)
	{
		prev_row = board[prev_col];
		if (prev_row == row || (prev_row - prev_col) == (row -col) || (prev_row + prev_col) == (row + col))
			return (0);
		prev_col++;
	}
	return (1);
}

void	solve_queens(int *board, int col, int n)
{
	int	row;

	if (col == n)
	{
		print_queens(board, n);
		return ;
	}
	row = 0;
	while (row < n)
	{
		if (is_safe(board, row, col))
		{
			board[col] = row;
			solve_queens(board, col + 1, n);
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	int	*board;
	int	n;

	if (ac != 2)
		return (1);
	n = atoi(av[1]);
	if (n <= 0)
		return (0);
	board = malloc(n * sizeof(int));
	if (!board)
		return (1);
	solve_queens(board, 0, n);
	free(board);
	return (0);
}
