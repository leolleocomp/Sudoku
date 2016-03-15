/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  make  && ./main
 *
 *         Author:  GUSTAVO MARQUES (), GUTODISSE AT GMAIL DOT COM
 *		   			JARDEL RIBEIRO  (), JARDELRIBEIRO.LIMA AT GMAIL DOT COM
 *                  LEONARDO PRADO  (),  LEOLLEO.COMP AT GMAIL DOT COM		
 *   Organization:  
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include "sudoku_solver/Sudoku.h"
#include "sudoku_solver/SudokuOpr.h"


/*-----------------------------------------------------------------------------
 *  CONSTANTES
 *-----------------------------------------------------------------------------*/

// TELA
#define MAX_X 600
#define MAX_Y 600


// CORES
#define VERMELHO   1,0,0
#define VERDE      0,1,0
#define AZUL       0,0,1
#define BRANCO     1,1,1
#define AMARELO    1,1,0
#define PRETO      0,0,0

/*-----------------------------------------------------------------------------
 *  VARIAVEIS
 *-----------------------------------------------------------------------------*/
int linha=0, coluna=0;
char sudoku[9][9], sudoku_preenchido[9][9];
Sudoku Csudoku, Csudoku_preenchido, Csudoku_resolvido;

float win, aspecto;
int largura, altura;
int game_over = 0;
float angulo_rotacao = 0 ;

float du_largura, du_altura;
float win_lagura =  16.0;
float win_altura = 12.0;

int tr_x=0, tr_y=0, tr_z=0; // TRANSLATE

float p_scale=1;

/*-----------------------------------------------------------------------------
 * VARIÁVEIS ASSOCIADAS AO MENU INCIAL
 *-----------------------------------------------------------------------------*/

int  st_x = 4, 		// posição x inicial do menu
	 st_y = 8, 		// posição y inicial do menu
	 largura_bloco = 8,	// largura de cada box do menu
	 box_select    = 0,	// indica qual box do menu está atualmente selecionada
	 box_select_fim	= 0;	// indica qual box do menu de fim de jogo está atualmente selecionada

double	altura_bloco = 1.0, 	// altura de cada box do menu 
	 	space = 0.5;		// espaço entre blocos

int	menu_limit = 3; 	// limite do menu, usado em menuInicial e menuDificuldade


/*-----------------------------------------------------------------------------
 * VARIÁVEIS e CABEÇALHOS DE FUNÇÕES ASSOCIADOS AO SUDOKU 
 *-----------------------------------------------------------------------------*/
void setaValores(int dificuldade);
void desenhaMenuDificuldade(void);
void desenhaMenu();
void menuDificuldadeTecladoHandle(unsigned char key, int x, int y);
void menuTecladoHandle(unsigned char key, int x, int y);
void menuTecladoSpecHandle(int key, int x, int y);
void escreveMenuDificuldadeTexto(int choos, double x, double y);
void DesenhaFudoTabuleiro();
void desenhaMenuFimDeJogo();
void escreveMenuTextoFimDeJogo(int choos, double x, double y);

void DesenhaCaixaSelecionado(GLfloat x1, GLfloat y1);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  renderStrokeFontLetter
 *  Description:  Fun��o que recebe a fonte e um texto por par�metro para ser exibido na
 *		    tela usando fonte de linhas
 * =====================================================================================
 */
void Escreve(float x, float y, char value)
{
	glPushMatrix();
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	//glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	//glLoadIdentity(); // reinicializa as transforma��es  
	glTranslatef(3.2+x,2.1+y,0);	/* posiciona com base na matrix */
	glScalef(0.008, 0.008, 0.008);  /* diminui o tamanho do fonte	*/
	glLineWidth(2); 		/* define a espessura da linha 	*/
	//temp = x+48; 			/* CONVERS�O ASCII 		*/
	glutStrokeCharacter(GLUT_STROKE_ROMAN,value);
	
	glPopMatrix();
}

void DesenhaNumeros()
{
	int x,y;
	char temp=1;
	//glRotatef(10,4.5f,4.5f,1.0f);
	
	for(x=0;x<9;x++)
		for(y=0;y<9;y++)
			if(sudoku[x][8-y] != -1) {
				if (Csudoku_preenchido.get(x, 8 - y) > 0)
					glColor3f(PRETO);
				else
					glColor3f(AZUL);

//				Escreve(x,y,sudoku[x][8-y]);
				Escreve(x, y, Csudoku.get(x, 8 - y) < 0 ? ' ' : Csudoku.get(x, 8 - y) + '0');
			}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaPoligono
 *  Description:  Fun��o que faz o desenho do tabuleiro
 * =====================================================================================
 */
void DesenhaTabela()
{
	int i;
	
	DesenhaFudoTabuleiro();
	// TRANSLATE                		
	glTranslatef(tr_x, tr_y, tr_z);
	tr_x = tr_y = tr_z = 0;

	// ESCALA
	glScalef(p_scale, p_scale, p_scale);
	p_scale = 1;
	
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	// Desenha as linhas
	//for(i=-4; i<6; i++)
	
	for(i=2; i<12; i++)
	{
		if((i-2)%3==0)
			glLineWidth(3.0 );
			
		glBegin(GL_LINES);      
			glColor3f( PRETO );
			glVertex2f( 3.0 , i );
		  	glVertex2f(  12.0 , i );       
		glEnd();
		glLineWidth(1.0 );    
	}
    	
    	// Desenha as colunas
    	//for(i=-4; i<6; i++)

	for(i=3.5; i<13; i++)
	{	
		if((i-3)%3==0)
			glLineWidth(3.0 );
		// Desenha o poligono  
			glBegin(GL_LINES);      
				glColor3f( PRETO );
				glVertex2f( i, 2 );
			  	glVertex2f( i,  11 );       
			glEnd();  
			glLineWidth(1.0 );  
    }
    if(linha >= 0 && linha < 9 && coluna >= 0 && coluna < 9 )
    	DesenhaCaixaSelecionado(linha+3,10-coluna);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaPoligono
 *  Description:  Fun��o que faz o desenho do POLIGONO
 * =====================================================================================
 */
void DesenhaCaixaSelecionado(GLfloat x1, GLfloat y1)
{	
	//printf("Here");
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transformação corrente
	glLoadIdentity();
	
	glPushMatrix();
	
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	
	glTranslatef(x1,y1,0.0f);
	glLineWidth(3.0 );
	glColor3f ( VERMELHO);

	glBegin(GL_LINES);      
		glVertex2f( 0,0 );
		glVertex2f( 0,1 );   
	glEnd();

	glBegin(GL_LINES);      
		glVertex2f( 0,1 );	
		glVertex2f( 1,1 );	       
	glEnd(); 
	
	glBegin(GL_LINES);      
		glVertex2f( 1,1 );	
		glVertex2f( 1,0 );         
	glEnd(); 

	glBegin(GL_LINES);      
		glVertex2f( 0,0 );
	  	glVertex2f( 1,0 );  		       
	glEnd(); 
	glLineWidth(1.0 );
	
	glPopMatrix();
	glFlush();
}

void DesenhaCaixaErro(GLfloat x1, GLfloat y1)
{	
	//printf("Here");
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transformação corrente
	glLoadIdentity();
	
	glPushMatrix();

	glTranslatef(x1,y1,0.0f);

	glColor3f ( VERMELHO);
	glBegin(GL_QUADS);      
		glVertex2f( 0,0 );	
		glVertex2f( 0,1 );	
		glVertex2f( 1,1 );	
		glVertex2f( 1,0 );         
	glEnd(); 
	
	glPopMatrix();
	glFlush();
}

void DesenhaFudoTabuleiro()
{	
	//printf("Here");
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transformação corrente
	glLoadIdentity();
	
	glPushMatrix();
	
	glTranslatef(3.0f,2.0f,0.0f);
	
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	
	
	//glTranslatef(10.5f,0.0f,0.0f);
	//glTranslatef(5.0,4.0,0.0f);
	
	
	glColor3f ( BRANCO );
	glBegin(GL_QUADS);      
		glVertex2f( 0,0 );	
		glVertex2f( 0,9 );	
		glVertex2f( 9,9 );	
		glVertex2f( 9,0 );    
	glEnd(); 
	
	
	
	glPopMatrix();
	glFlush();
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Desenha
 *  Description:  Fun��o de callback de redesenho da janela de visualiza��o
 * =====================================================================================
 */
void Desenha(void)
{
	// Limpa a janela de visualiza��o com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);

	// Define a Viewport 1
	glViewport(0, 0, largura, altura);
	
	// DESENHA AS LINHAS DA TABELA
	DesenhaTabela();
	
	// DESENHA OS NUMEROS DA MATRIX SUDOKU
	DesenhaNumeros();
	
	if(game_over==1)
		desenhaMenuFimDeJogo();
		
	// Executa os comandos OpenGL 
	glFlush();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  AlteraTamanhoJanela
 *  Description:  Fun��o callback chamada quando o tamanho da janela � alterado
 * =====================================================================================
 */
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{	
	// Evita a divisao por zero
	if(h == 0) h = 1;
	
	// Atualiza as vari�veis
	largura = w;
	altura = h;

	aspecto = (float) largura/altura;	
	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Estabelece a janela de sele��o (esquerda, direita, inferior, 
	// superior) mantendo a propor��o com a janela de visualiza��o
	//gluOrtho2D (-win*aspecto, win*aspecto, -win, win);
	
//	gluOrtho2D(-7.5f,8.5f,-5.5,6.5f);
	gluOrtho2D(0.0f,16.0f,0.0f,12.0f);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Teclado
 *  Description:  Fun��o callback chamada para gerenciar eventos de teclas
 * =====================================================================================
 */
 
void makedelay(int){

	Desenha();
	angulo_rotacao++;
	if(angulo_rotacao<=360)
		glutTimerFunc(0.2,makedelay,1);
	else{
		angulo_rotacao = 0;
		return;
		}
}

void tecladoSudoku(unsigned char key, int x, int y)
{

	SudokuOpr op;
	int aux = 0;
	// 0       1      2
	// RED - GREEN - BLUE
	if(key >='1' && key<= '9')
	{
		if (Csudoku_preenchido.get(linha, coluna) == -1) {
			Csudoku.unmark(linha, coluna);
			
			if (!Csudoku.mark(key - '0', linha, coluna)) {
				DesenhaCaixaErro(linha+3,10-coluna);
				sleep(1);
			}
			
			Csudoku.print();
			
			if (Csudoku.isComplete())
				key = 'p';
		}
	}
	
	if (key == 'h' || key == 'H') {
			Csudoku_preenchido.mark(Csudoku_resolvido.get(linha, coluna), linha, coluna);
			Csudoku_preenchido.print();
			
			if (!Csudoku.mark(Csudoku_preenchido.get(linha, coluna), linha, coluna)) {
				DesenhaCaixaErro(linha+3,10-coluna);
				sleep(1);
			}
			
			if (Csudoku.isComplete())
				key = 'p';
	}
	
	switch(key)
	{	
		case 8://backspace
		case 127://delete
			if (Csudoku_preenchido.get(linha, coluna) == -1) {
				Csudoku.unmark(linha, coluna);
			}
			break;		
		case 'p':
		case 'P':
			op.solve(Csudoku_preenchido, 0);
			Csudoku = Csudoku_preenchido;
			glutTimerFunc(1,makedelay,1);
			angulo_rotacao = 0;
			game_over = 1;
			break;
		case 13:
			if(game_over==1){
				game_over = 0;
				if(box_select_fim==0){
					menu_limit = 3;
					
					glutDisplayFunc(desenhaMenuDificuldade);
					glutKeyboardFunc(menuDificuldadeTecladoHandle);
					glutSpecialFunc(menuTecladoSpecHandle);
					aux = 1;
				}
				else
					exit(EXIT_SUCCESS);
			}
			break;
		case 27:
		case 'q':
		case 'Q':
			exit(0);
			break;
		
	}
	
	/**/
	//if(aux==0)
		DesenhaNumeros();

	glutPostRedisplay();

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GerenciaMouse
 *  Description:  Fun��o callback chamada para gerenciar eventos do mouse
 * =====================================================================================
 */
void GerenciaMouse(int button, int state, int x, int y)
{       
	int i;
	if (button == GLUT_RIGHT_BUTTON ){
	
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		linha 	= (int)((x/du_largura)-3);
		coluna  = (int)((y/du_altura)-1);
		
		//printf("coluna[%d]linha[%d]\n",(int) ((x-405)/70) ,(int) ((y-36)/69) ); 
		printf("coluna[%d]linha[%d]\n", (int)((x/du_largura)-3),(int)((y/du_altura)-1)); 

	}  
	
	glutPostRedisplay();
}

void sudokuTecladoSpecHandle(int key, int x, int y)
{	
	if(key == GLUT_KEY_LEFT)
	{	
		if(game_over==0){
			linha--;
		
			if(linha < 0)
				linha = 8;
		}	
		else
			box_select_fim = ++box_select_fim % 2;
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		if(game_over==0)
			linha = ++linha % 9;
		else
		{
			box_select_fim--;
			if(box_select_fim < 0)
				box_select_fim = 1;
		}
	} 
	else if(key == GLUT_KEY_UP)
	{	
		if(game_over==0){
			coluna--;
			if(coluna < 0)
				coluna = 8;
		}
		else
		{
			box_select_fim--;
			if(box_select_fim < 0)
				box_select_fim = 1;
		}
	}
	else if(key == GLUT_KEY_DOWN)
	{
		if(game_over==0)
			coluna = ++coluna % 9;
		else
			box_select_fim = ++box_select_fim % 2;
	} 
	//printf("%d",key);
   

	glutPostRedisplay();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Inicializa
 *  Description:  Fun��o respons�vel por inicializar par�mentros e vari�veis
 * =====================================================================================
 */
void Inicializa (void)
{   
	// Define a cor de fundo da janela de visualiza��o como branca
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
	//win = 5.5f;
	
	//gluOrtho2D(-5.0f,5.0f,-5.0f,5.0f);
}
 
/****
 *		ROTINAS RELACIONADAS AO MENU INICIAL
 ****/

void escreveMenuTexto(int choos, double x, double y)
{
	char menu[3][10] = {
				"JOGAR",
				"CREDITOS",
				"SAIR"
			 };

	glPushMatrix();

	double delta[] = { 1.5, 1.5, 1.5 };

	glTranslatef(x + largura_bloco / 2.0 - delta[choos], y - altura_bloco, 0.0f);
	glScalef(0.005, 0.005, 0.0);
	glLineWidth(2);

	for (int i = 0; menu[choos][i] != '\0'; ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, menu[choos][i]);

	glPopMatrix();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  desenhaMenu 
 *  Description:  renderiza o menu principal 
 * =====================================================================================
 */

void desenhaMenu()
{
	// Limpa a janela de visualiza��o com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Define a Viewport 1
	glViewport(0, 0, largura, altura);

	glPushMatrix();
	// desenha caixas de diálogo
	for (int i = 0; i < menu_limit; i++) {
		glBegin(GL_QUADS);
			if (i != box_select)
				glColor3f(PRETO);
			else 
				glColor3f(VERMELHO);

			glVertex2f(st_x, st_y);
			glVertex2f(st_x, st_y  - altura_bloco);
			glVertex2f(st_x + largura_bloco, st_y - altura_bloco);
			glVertex2f(st_x + largura_bloco, st_y); 
		glEnd();

		glColor3f(AZUL);
		escreveMenuTexto(i, st_x, st_y);

		glTranslatef(0.0f, - (altura_bloco + space), 0.0f);
	}

	glPopMatrix();
	glFlush();
}

void desenhaMenuFimDeJogo()
{
	// Limpa a janela de visualiza��o com a cor  
	// de fundo definida previamente
	//glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);

	// Define a Viewport 1
	//glViewport(0, 0, largura, altura);
	float st_x = 12.5;
	float st_y = 7.5;
	float largura_bloco = 3.0;
	
	glPushMatrix();
	// desenha caixas de diálogo
	for (int i = 0; i < 2; i++) {
		glBegin(GL_QUADS);
			if (i != box_select_fim)
				glColor3f(PRETO);
			else 
				glColor3f(VERMELHO);

			glVertex2f(st_x, st_y);
			glVertex2f(st_x, st_y  - altura_bloco);
			glVertex2f(st_x + largura_bloco, st_y - altura_bloco);
			glVertex2f(st_x + largura_bloco, st_y); 
		glEnd();

		glColor3f(AZUL);
		escreveMenuTextoFimDeJogo(i, st_x - 2.2, st_y + 0.3);

		glTranslatef(0.0f, - (altura_bloco + space), 0.0f);
	}

	glPopMatrix();
	glFlush();
}

void escreveMenuTextoFimDeJogo(int choos, double x, double y)
{
	char menu[2][10] = {
				"NOVO JOGO",
				"SAIR"
			 };

	glPushMatrix();

	double delta[] = { 1.5, 1.5, 1.5 };

	glTranslatef(x + largura_bloco / 2.0 - delta[choos], y - altura_bloco, 0.0f);
	glScalef(0.003, 0.003, 0.0);
	glLineWidth(2);

	for (int i = 0; menu[choos][i] != '\0'; ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, menu[choos][i]);

	glPopMatrix();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  menuTecladoSpecHandle 
 *  Description:  interação com o menu 
 * =====================================================================================
 */
void menuTecladoSpecHandle(int key, int x, int y)
{
	if (key == GLUT_KEY_UP && box_select > 0) 
		box_select--;
	else  if (key == GLUT_KEY_DOWN && box_select < (menu_limit - 1))
		box_select++;
	
	glutPostRedisplay();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  menuTecladoHandle 
 *  Description:  seleciona um item do menu 
 * =====================================================================================
 */
void menuTecladoHandle(unsigned char key, int x, int y)
{
	if (key == 13) {
		switch (box_select) {
		case 0:
			menu_limit = 3;
			glutDisplayFunc(desenhaMenuDificuldade);
			glutKeyboardFunc(menuDificuldadeTecladoHandle);
			glutSpecialFunc(menuTecladoSpecHandle);
			printf("Dificuldade\n");
			break;
		case 1:
			// mostra os créditos [não implementado]
			break;
		case 2:
			// mensagem de texto bonitinha e sair [não implementado]
			exit(EXIT_SUCCESS);
			break;
		}
	}

	glutPostRedisplay();
}

/****
 *		ROTINAS RELACIONADAS AO MENU DE DIFICULDADE 
 ****/

void escreveMenuDificuldadeTexto(int choos, double x, double y)
{
	char menu[3][10] = {
				"FACIL",
				"MEDIO",
				"DIFICIL" };

	glPushMatrix();

	// não consegui centralizar esse texto ainda..
	double delta[] = { 1., 1., 1. };

	glTranslatef(x + largura_bloco / 2.0 - delta[choos], y - altura_bloco, 0.0f);

	glScalef(0.005, 0.005, 0.0);
	glLineWidth(2);

	for (int i = 0; menu[choos][i] != '\0'; ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, menu[choos][i]);

	glPopMatrix();
}

void desenhaMenuDificuldade()
{

	// Limpa a janela de visualiza��o com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Define a Viewport 1
	glViewport(0, 0, largura, altura);

	glPushMatrix();
	// desenha caixas de diálogo
	for (int i = 0; i < menu_limit; i++) {
		glBegin(GL_QUADS);
			if (i != box_select)
				glColor3f(PRETO);
			else 
				glColor3f(VERMELHO);

			glVertex2f(st_x, st_y);
			glVertex2f(st_x, st_y  - altura_bloco);
			glVertex2f(st_x + largura_bloco, st_y - altura_bloco);
			glVertex2f(st_x + largura_bloco, st_y); 
		glEnd();

		glColor3f(AZUL);
		escreveMenuDificuldadeTexto(i, st_x, st_y);

		glTranslatef(0.0f, - (altura_bloco + space), 0.0f);
	}

	glPopMatrix();
	glFlush();
}


void menuDificuldadeTecladoHandle(unsigned char key, int x, int y)
{
	int dificuldade;

	if (key == 13) {
		switch (box_select) {
		case 0:
			dificuldade = FACIL;
			break;
		case 1:
			// mostra os créditos [não implementado]
			dificuldade = MEDIO;
			break;
		case 2:
			// mensagem de texto bonitinha e sair [não implementado]
			dificuldade = DIFICIL;
			break;
		}

		setaValores(dificuldade);
		glutSpecialFunc(sudokuTecladoSpecHandle);
		glutDisplayFunc(Desenha);
		glutKeyboardFunc(tecladoSudoku);
	}

	glutPostRedisplay();
}

/****
 *		SUDOKU LÓGICA E LÓGICA PESADA	
 ****/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setaValores 
 *  Description:  seleciona um item do menu 
 * =====================================================================================
 */

void setaValores(int dificuldade)
{
	SudokuOpr op(dificuldade);
//	Sudoku tmp;
//	op.randomGen(tmp);
	op.randomGen(Csudoku);
	
	Csudoku_preenchido = Csudoku;
    Csudoku_resolvido  = Csudoku;
  
    op.solve(Csudoku_resolvido, 0);
//	for (int r = 0; r < 9; ++r)
//		for (int c = 0; c < 9; ++c)
//			if (tmp.get(r, c) > 0) {
//				sudoku[r][c] = sudoku_preenchido[r][c] = tmp.get(r, c) + '0';
//			} else
//				sudoku[r][c] = sudoku_preenchido[r][c] = -1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char** argv)
{
	// Init 
	glutInit(&argc, argv);
	
	printf ("Pixel Dimensions: %d x %d \n",/*glutGet (GLUT_SCREEN_WIDTH_MM) / */glutGet (GLUT_SCREEN_WIDTH),
            /*glutGet (GLUT_SCREEN_HEIGHT_MM) /*/ glutGet(GLUT_SCREEN_HEIGHT));
	
	printf ("Pixel Dimensions: %f x %f \n",/*glutGet (GLUT_SCREEN_WIDTH_MM) / */glutGet (GLUT_SCREEN_WIDTH)/16.0,
            /*glutGet (GLUT_SCREEN_HEIGHT_MM) /*/ glutGet(GLUT_SCREEN_HEIGHT)/12.0);
	printf("%d %d", altura,largura);
	    
   	du_largura = glutGet (GLUT_SCREEN_WIDTH)/win_lagura;
    	du_altura = glutGet(GLUT_SCREEN_HEIGHT)/win_altura;
       	
	// Define do modo de opera��o da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
 
	// Especifica a posi��o inicial da janela GLUT
	glutInitWindowPosition(5,5); 
    
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(MAX_X,MAX_Y); 
 
	// Cria a janela passando como argumento o t�tulo da mesma
	glutCreateWindow("Sudoku");
 
	// Registra a fun��o callback de redesenho da janela de visualiza��o
	// glutDisplayFunc(Desenha);
 	glutDisplayFunc(desenhaMenu); 
	// Registra a fun��o callback de redimensionamento da janela de visualiza��o
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a fun��o callback para tratamento das teclas ASCII
	// glutKeyboardFunc (tecladoSudoku);
	glutKeyboardFunc(menuTecladoHandle);
  
	// Registra a fun��o de callback para tratamento de teclas especiais
	//glutSpecialFunc (TecladoEspecial);
	glutSpecialFunc(menuTecladoSpecHandle);
	// Registra a fun��o callback para tratamento do mouse
	glutMouseFunc(GerenciaMouse);  
	
	// ATIVA A JANELA EM FULLSCREEN
	glutFullScreen();
	
	// Chama a fun��o respons�vel por fazer as inicializa��es 
	Inicializa();
 
	// Inicia o processamento e aguarda intera��es do usu�rio
	glutMainLoop();
 
	return 0;
}
