#include <cstdio>
#include "Sudoku.h"
#include "SudokuOpr.h"

int main()
{
	Sudoku table;
	SudokuOpr op(DIFICIL);

	op.randomGen(table);
	table.print();

	int cont = 0;

	for (int r = 0; r < 9; ++r)
		for (int c = 0; c < 9; ++c)
			cont += table.get(r,c) < 0;

	printf("%d\n", cont);
}
