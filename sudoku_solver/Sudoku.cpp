#include "Sudoku.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

/**
 *	Construtor de sudoku vazio
 *
 *	de inicio, 
 *	col[k] = 1, lin[k] = 1, k = 0 .. 8,
 *	block[r][c] = r = 0 .. 2, c = 0 .. 2; 
 */

Sudoku::Sudoku()
{
	erase();
}

/**
 *	Sudoku::operator=
 *
 *	copia o conteudo de b no sudoku
 *
 *	@param b sudoku a ser copiado
 */

void Sudoku::operator=(Sudoku &b)
{
	for (int k = 0; k < 9; ++k) {
		col[k] = b.col[k];
		lin[k] = b.lin[k];
	}

	for (int r = 0; r < 3; ++r)
		for (int c = 0; c < 3; ++c)
			block[r][c] = b.block[r][c];

	for (int r = 0; r < 9; ++r) {
		for (int c = 0; c < 9; ++c)
			sudoku[r][c] = b.sudoku[r][c];
	}
}

/**
 *	Sudoku::mark
 *
 *	seta o elemento sudoku[r][c] com elem
 *
 * 	@param elem elemento a ser setado
 * 	@param r    linha
 * 	@param c    coluna
 */

bool Sudoku::mark(int elem, int r, int c)
{
	if (!canBePut(elem, r, c))
		return false;

	col[c] |= (1 << elem);
	lin[r] |= (1 << elem);
	block[r/3][c/3] |= (1 << elem);
	sudoku[r][c] = elem;
	
	return true;
}

/**
 *	Sudoku::unmark
 *
 * 	retira o elemento sudoku[r][c], atribui 0
 *
 * 	@param r linha
 * 	@param c coluna
 */

void Sudoku::unmark(int r, int c)
{
	col[c] &= ~(1 << sudoku[r][c]);
	lin[r] &= ~(1 << sudoku[r][c]);
	block[r/3][c/3] &= ~(1 << sudoku[r][c]);
	sudoku[r][c] = -1;
}

/**
 *	Sudoku::canBePut
 *
 * 	verifica se o elemento elem pode ser colocado em sudoku[r][c]
 *
 * 	@param elem elemento a ser verificado
 * 	@param r    linha
 * 	@param c    coluna
 */

bool Sudoku::canBePut(int elem, int r, int c)
{
	return !(col[c] & (1 << elem)) && !(lin[r] & (1 << elem)) && !(block[r/3][c/3] & (1 << elem));
}

/**
 *	Sudoku::isComplete
 *
 * 	verifica se o sudoku está completo. Devido
 * 	ao modo como os bitmasks são setados e utilizados
 * 	isso significa que
 *
 * 		col[k]      = 2**10 - 1 = 111111111b, k = 0 .. 8
 * 		lin[k]      = 2**10 - 1 = 111111111b, k = 0 .. 8
 * 		block[r][c] = 2**10 - 1 = 111111111b, r,c in [0,2]
 *
 */

bool Sudoku::sudokuIsComplete()
{
	int i;

	i = 0;
	while (i < 9 && col[i++] == (1 << 10) - 1);
	if (i != 9) return false;

	i = 0;
	while (i < 9 && lin[i++] == (1 << 10) - 1);
	if (i != 9) return false;

	for (int l = 0; l < 3; ++l) {
		i = 0;
		while (i < 3 && block[l][i++] == (1 << 10) - 1);
		if (i != 3) return false;
	}
	
	return true;
}

/**
 *	Sudoku::print()
 *
 *	imprime o tabuleiro de sudoku na saida padrão
 */

void Sudoku::print()
{
	for (int i = 0; i < 9; ++i) {
		if (i && i%3 == 0) 
			printf("----------------------\n");
		
		for (int j = 0; j < 9; ++j) {
			if (j && j%3 == 0) printf("| ");
			if (sudoku[i][j] > 0)  printf("%d ", sudoku[i][j]);
			else printf("# ");
		}
		printf("\n");
	}
	
	printf("\n\n");
}

void Sudoku::erase()
{
	for (int r = 0; r < 9; ++r)
		for (int c = 0; c < 9; ++c)
			sudoku[r][c] = -1;

	for (int k = 0; k < 9; ++k) {
		lin[k] = 1;
		col[k] = 1;
	}

	for (int r = 0; r < 3; ++r)
		for (int c = 0; c < 3; ++c)
			block[r][c] = 1;
}

int Sudoku::get(int r, int c)
{
	return sudoku[r][c];
}
