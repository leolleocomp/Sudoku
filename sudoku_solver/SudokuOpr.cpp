#include "SudokuOpr.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>

/**
 *	SudokuOpr::SudokuOpr
 *
 *	construtor do sudoku, a dificuldade é setada
 *	de acordo com o número de espaços vazios no sudoku final
 *
 * 	@param lvl dificuldade do sudoku, padrão = FACIL
 */

SudokuOpr::SudokuOpr(int lvl)
{
	switch(lvl) {
	case FACIL:   num_gaps = 41; break;
	case MEDIO:   num_gaps = 48; break;
	case DIFICIL: num_gaps = 51; break;
	}
}

/**
 *	SudokuOpr::solve
 *
 *	resolve o sudoku usando backtracking
 *
 *	@param table objeto Sudoku
 *	@param elem  elemento sendo avaliado pela instância de solve
 */

bool SudokuOpr::solve(Sudoku &table, int elem)
{
	if (elem >= 81) {
		return true;
	}

	int r = elem / 9, c = elem % 9;

	if (table.get(r,c))
		return solve(table, elem + 1);	
	else
		for (int k = 1; k <= 9; ++k)
			if (table.canBePut(k, r, c)) {
				table.mark(k, r, c);

				if (solve(table, elem + 1))
					return true;

				table.unmark(r, c);
			}

	return false;
}

/**
 *	SudokuOpr::randomGen
 *
 * 	gera aleatoriamente um tabuleiro de sudoku válido
 *
 * 	@param table objeto da classe Sudoku
 */

void SudokuOpr::randomGen(Sudoku &table)
{
	Sudoku tmp;
	int givens = 11;

	srand(time(NULL)); // 'alimenta' o gerador de números pseudo aleatórios

	while (givens) {
		int pos = rand() % 81;

		while (table.get(pos / 9, pos % 9)) {
			pos = rand() % 81;
		}

		int i = 1;
		while (i <= 9 && !table.canBePut(i, pos / 9, pos % 9)) i++;
		if (i > 9) continue;

		tmp = table;

		table.mark(i, pos / 9,  pos % 9);

		if (solve(table, 0)) {
			tmp.mark(i, pos / 9, pos % 9);
			givens--;
		}

		table = tmp;
	}

	solve(table, 0);
	setGaps(table);
}

/**
 *	SudokuOpr::setGaps
 *
 *	pões espaços em branco aleatoriamente no tabuleiro gerado
 *	por randomGen. O número de espaços em branco é setado
 *	de acordo com a dificuldade escolhida
 *
 *	@param table objedo da classe Sudoku
 */

void SudokuOpr::setGaps(Sudoku &table)
{
	int n = num_gaps;
	Sudoku tmp;
	
	while (n) {
		int pos = rand() % 81, aux;

		while (!table.get(pos / 9, pos % 9))
			pos = rand() % 81;

		tmp = table;
		table.unmark(pos / 9, pos % 9);
		
		if ((aux = countSolutions(table, 0)) == 1) {
			tmp.unmark(pos / 9, pos % 9);
			n--;
		}
		
		table = tmp;
	}
}

/**
 *	SudokuOpr::countSolutions
 *
 *	conta o número de soluções de um dado
 *	tabuleiro de sudoku
 *
 *	@param table objeto da classe Sudoku
 *	@param elem  elemento da tabela sendo avaliado pela
 *		     instância de SudokuOpr::countSolutions
 */

int SudokuOpr::countSolutions(Sudoku &table, int elem)
{
	int cont = 0;

	if (elem >= 81) {
		return 1;
	}

	int r = elem / 9, c = elem % 9;

	if (table.get(r, c))
		return solve(table, elem + 1);	
	else
		for (int k = 1; k <= 9; ++k)
			if (table.canBePut(k, r, c)) {
				cont++;
				table.mark(k, r, c);

				cont += solve(table, k + 1);

				table.unmark(r, c);
			}

	return cont;
}
