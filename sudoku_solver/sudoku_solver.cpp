#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

class SudokuOpr
{
public:
	bool solve(Sudoku &table, int elem) {
		if (elem >= 81) {
			table.print();
			return true;
		}

		int r = elem / 9, c = elem % 9;

		if (table.sudoku[r][c])
			return solve(table, elem + 1);	
		else
			for (int k = 1; k <= 9; ++k)
				if (table.canBePut(k, r, c)) {
					table.print();
					table.mark(k, r, c);

					if (solve(table, elem + 1))
						return true;

					table.unmark(k, r, c);
				}

		return false;
	}

	void sudokuRandomGen(Sudoku &table) {
		srand(time(NULL)); // 'alimenta' o gerador de números pseudo aleatórios

		while (!table.sudokuIsComplete()) {
			int r = rand()%9,
			    c = rand()%9,
			    elem = rand()%9;

			if (table.canBePut(elem, r, c)) {
				table.mark(elem, r, c);

				if (!solve(table, 0))
					table.unmark(elem, r, c);
			}
		}
	}

private:
	void sudokuSetGaps(Sudoku &table, int n)
	{
		while (n) {
			int r = rand() % 9,
			    c = rand() % 9;

			table.unmark(table.sudoku[r][c], r, c);
			
			if (countSolutions(table, 0) == 1) 
				n--; 
		}
	}

	int countSolutions(Sudoku &table, int elem) {
		int cont = 0;

		if (elem >= 81) {
			table.print();
			return 1;
		}

		int r = elem / 9, c = elem % 9;

		if (table.sudoku[r][c])
			return solve(table, elem + 1);	
		else
			for (int k = 1; k <= 9; ++k)
				if (table.canBePut(k, r, c)) {
					cont++;
					table.print();
					table.mark(k, r, c);

					cont += solve(table, k + 1);

					table.unmark(k, r, c);
				}

		return cont;
	}

};


int main()
{
}
