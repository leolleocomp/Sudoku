#ifndef SUDOKU_H
#define SUDOKU_H

/**
 *	class Sudoku
 *
 *	implementa uma tabela de sudoku, junto com estruturas
 *	necessárias para memorizar os números já presentes em cada
 *	linha, coluna e bloco.
 *
 *	@author Leonardo Cavalcante do Prado (leolleo)
 */

class Sudoku
{
public:
	Sudoku(); // construtor de sudoku vazio

	void operator=(Sudoku &b); 			// copia o sudoku b no sudoku atual
	void operator=(int table[9][9]);		// seta o valor de Sudoku.sudoku como o de table
	bool mark(int elem, int r, int c);		// coloca o elemento elem em sudoku[r][c]
	void unmark(int r, int c);			// retira o elemento eleme de sudoku[r][c]
	bool canBePut(int elem, int r, int c); 		// verifica se elem pode ser colocado em sudoku[r][c]
	bool sudokuIsComplete();			// verifica se o sudoku está completo
	void print();					// imprime o sudoku na saida padrão
	void erase();					// erase sudoku

	int get(int r, int c);

private:
	int sudoku[9][9], 	// tabela
	    col[9],		// bitmask elementos das colunas
	    lin[9],		// bitmask elementos das linhas
	    block[3][3];	// bitmask elementos dos blocos
};

#endif
