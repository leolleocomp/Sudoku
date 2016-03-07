#ifndef SUDOKU_OPR
#define SUDOKU_OPR

/**
 *	class SudokuOpr
 *	
 *	Opera sobre objetos da classe Sudoku:
 *
 *	- encontra uma solução para o sudoku
 *	- gera aleatoriamente um sudoku com base na dificuldade
 *	  setada 
 *
 * 	@author Leonardo Cavalcante do Prado (leolleo)	
 */

#include "Sudoku.h"

#define FACIL   0
#define MEDIO   1
#define DIFICIL 2

class SudokuOpr
{
public:
	SudokuOpr(int lvl = FACIL);		// construtor, dificuldade padrão = FACIL

	bool solve(Sudoku &table, int elem);	// soluciona o sudoku via backtracking
	void randomGen(Sudoku &table);		// gera um sudoku (com solução única) aleatoriamente
	void setLevel(int lvl);			// seta dificuldade

private:
	int  num_gaps;

	void setGaps(Sudoku &table);			// seta os lugares vazios do sudoku aleatoriamente
	int  countSolutions(Sudoku &table, int elem);	// conta o número de soluções do sudoku

};

#endif
