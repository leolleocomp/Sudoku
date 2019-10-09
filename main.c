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
#define AZUL       0,0,1
#define BRANCO     1,1,1
#define AMARELO    1,1,0
#define PRETO      0,0,0

#define HINT 1
#define RESOLVER 2
#define NOVO_JOGO 3
#define SAIR 4

/*-----------------------------------------------------------------------------
 *  VARIAVEIS
 *-----------------------------------------------------------------------------*/
int linha=0, coluna=0;
char sudoku[9][9], sudoku_preenchido[9][9];
Sudoku Csudoku, Csudoku_preenchido, Csudoku_resolvido;

float win, aspecto;
int largura, altura;
int game_over = 0;
int menu;
float angulo_rotacao = 0;

float du_largura, du_altura;
float win_lagura =  16.0;
float win_altura = 12.0;

int tr_x=0, tr_z=0; // TRANSLATE
double tr_y;

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


void setaValores(int dificuldade);
void desenhaMenuDificuldade(void);
void desenhaMenu();
void desenhaCreditos();
void menuDificuldadeTecladoHandle(unsigned char key, int x, int y);
void menuTecladoHandle(unsigned char key, int x, int y);
void creditosTecladoHandle(unsigned char key, int x, int y);
void creditosDelay(int value);
void menuTecladoSpecHandle(int key, int x, int y);
void escreveMenuDificuldadeTexto(int choos, double x, double y);
void DesenhaFudoTabuleiro();
void desenhaMenuFimDeJogo();
void escreveMenuTextoFimDeJogo(int choos, double x, double y);
void createGLUTMenus();
void DesenhaCaixaSelecionado(GLfloat x1, GLfloat y1);
void Desenha(void);
void DesenhaNumeros();
void tecladoSudoku(unsigned char key, int x, int y);
void DesenhaCaixaErro(GLfloat x1, GLfloat y1);

/****
 *		FUNÇÕES AUXILIARES
 ****/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  makedelay
 *  Description:  Função que faz a animação final
 * =====================================================================================
 */
 
void makedelay(int){

	Desenha();
	angulo_rotacao++;

	if(angulo_rotacao <= 360)
		glutTimerFunc(0.2,makedelay,1);
	else {
		angulo_rotacao = 0;
		return;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  novoJogo
 *  Description:  Função que inicia um novo jogo
 * =====================================================================================
 */
 
void novoJogo(){
	
	glutDisplayFunc(desenhaMenuDificuldade);
	glutKeyboardFunc(menuDificuldadeTecladoHandle);
	glutSpecialFunc(menuTecladoSpecHandle);
	game_over = 0;
	linha = 0;
	coluna = 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  autoCompletar
 *  Description:  Função que resolve o Soduku
 * =====================================================================================
 */
 
void autoCompletar(){
	
	SudokuOpr op;

	op.solve(Csudoku_preenchido, 0);
	Csudoku = Csudoku_preenchido;
	glutTimerFunc(1,makedelay,1);
	angulo_rotacao = 0;
	game_over = 2;
	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isComplete
 *  Description:  Função que verifica se o tabuleiro foi preenchido
 * =====================================================================================
 */
 
void isComplete(){
	SudokuOpr op;
	op.solve(Csudoku_preenchido, 0);
	Csudoku = Csudoku_preenchido;
	glutTimerFunc(1,makedelay,1);
	angulo_rotacao = 0;
	game_over = 1;
	
	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hintNumero
 *  Description:  Função que preenchi a casa selecionada com o valor válido
 * =====================================================================================
 */
 void hintNumero(){
	
	Csudoku_preenchido.mark(Csudoku_resolvido.get(linha, coluna), linha, coluna);
	Csudoku_preenchido.print();
			
	if (!Csudoku.mark(Csudoku_preenchido.get(linha, coluna), linha, coluna)) {
		DesenhaCaixaErro(linha+3,10-coluna);
		sleep(1);
	}
	
	if (Csudoku.isComplete())
		isComplete();
}

/****
 *		FUNÇÕES DE ALTERAÇÃO DO TAMANHO DA TELA
 ****/

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
	
	gluOrtho2D(0.0f,16.0f,0.0f,12.0f);
}

/****
 *		FUNÇÃO DE MOUSE
 ****/ 
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GerenciaMouse
 *  Description:  Função callback chamada para gerenciar eventos do mouse
 * =====================================================================================
 */
void GerenciaMouse(int button, int state, int x, int y)
{       
	int i;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		linha 	= (int)((x/du_largura)-3);
		coluna  = (int)((y/du_altura)-1);
		
		printf("coluna[%d]linha[%d]\n", (int)((x/du_largura)-3),(int)((y/du_altura)-1)); 

	}  
	
	glutPostRedisplay();
}
 
/****
 *		ROTINAS RELACIONADAS AO MENU INICIAL
 ****/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  escreveMenuTexto
 *  Description:  Função que desenah o texto do menu inicial
 * =====================================================================================
 */
 
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
 *         Name:  desenhaMenuFimDeJogo
 *  Description:  Função que desenah o menu do fim do jogo
 * =====================================================================================
 */
void desenhaMenuFimDeJogo()
{
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


/****
 *		ROTINAS RELACIONADAS AO MENU DE DIFICULDADE 
 ****/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  escreveMenuDificuldadeTexto
 *  Description:  Função que desenha o texto do menu dificuldade
 * =====================================================================================
 */
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


/****
 *		ROTINAS RELACIONADAS AOS CRÉDITOS 
 ****/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  creditosDelay
 *  Description:  Função que faz a animação dos créditos
 * =====================================================================================
 */
 void creditosDelay(int value)
{
	tr_y += .01;
	if (tr_y > 0.) return;
	glutPostRedisplay();
	glutTimerFunc(12.5, creditosDelay, 1);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printStrokeString
 *  Description:  Função que recebe uma string e desenah na tela
 * =====================================================================================
 */
 void printStrokeString(char *s)
{

	for (int i = 0; s[i] != '\0'; ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  desenhaCreditos
 *  Description:  Função que desenha o texto dos créditos
 * =====================================================================================
 */
 void desenhaCreditos()
{
	char author[3][50] = {
		"Gustavo Marques",
		"Jardel Lima",
		"Leonardo Cavalcante do Prado"
	},
	
	email[3][50] = {
		"gutodisse@gmail.com",
		"jardelribeiro.lima@gmail.com",
		"leolleo.comp@gmail.com"
	};
	
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, largura, altura);

	glPushMatrix();
	glTranslatef(0,tr_y,0);

	glPushMatrix();

	glTranslatef(4,8.8,0);
	glScalef(8,1,0);

	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.7f, 0.7f);
		glVertex2f(0,0);
		glVertex2f(1,0);
		glColor3f(0.4, 0.4, 0.4);
		glVertex2f(1,1);
		glVertex2f(0,1);
	glEnd();

	glPopMatrix();

	glPushMatrix();
		glTranslatef(win_lagura / 2. - 1.5, win_altura / 2. + 3, 0);
		glScalef(0.005, 0.005, 0);
		glLineWidth(4);
		glColor3f(AZUL);
		printStrokeString("Creditos");	
	glPopMatrix();


	for (int k = 0; k < 3; ++k) {
		glPushMatrix();
		glTranslatef(win_lagura / 2. - 3.5, win_altura / 2. - k, 0);
		glLineWidth(2);
		glScalef(0.002, 0.002, 0.0);
		glColor3f(AZUL);
		printStrokeString(author[k]);
		printStrokeString(", ");
		glColor3f(VERMELHO);
		printStrokeString(email[k]);
		glPopMatrix();
	}

	glPushMatrix();
		glTranslatef(win_lagura / 2. - 1.25, win_altura / 2. - 3, 0);
		glScalef(0.002, 0.002, 0);
		glLineWidth(2);
		glColor3f(AZUL);
		printStrokeString("ESC - voltar");	
	glPopMatrix();
	glPopMatrix();
	
	glFlush();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Teclado
 *  Description:  Função callback chamada para gerenciar eventos de teclas da tela de 
 *				  créditos
 * =====================================================================================
 */
void creditosTecladoHandle(unsigned char key, int x, int y)
{
	if (key == 27) { // esc
		glutDisplayFunc(desenhaMenu);	
		glutKeyboardFunc(menuTecladoHandle);
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
	op.randomGen(Csudoku);
	Csudoku_preenchido = Csudoku;
    Csudoku_resolvido  = Csudoku;
    op.solve(Csudoku_resolvido, 0);

}

/****
 *		FUNÇÕES DO MENU DO MOUSE
 ****/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  processMenuEvents
 *  Description:  Função que processa os eventos do menu do mouse
 * =====================================================================================
 */
void processMenuEvents(int option) {

	switch (option) {
		case HINT :
			if(game_over==0)
				hintNumero();
			break;
		case RESOLVER :
			if(game_over==0)
				autoCompletar();
			break;
		case NOVO_JOGO :
				novoJogo();
			break;
		case SAIR :
			exit(0);
			break;
	}
	
	DesenhaNumeros();
	glutPostRedisplay();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createGLUTMenus
 *  Description:  Função que cria o menu do mouse
 * =====================================================================================
 */
void createGLUTMenus() {

		// create the menu and
		// tell glut that "processMenuEvents" will
		// handle the events
		menu = glutCreateMenu(processMenuEvents);

		//add entries to our menu
		glutAddMenuEntry("Hint",HINT);
		glutAddMenuEntry("Resolver",RESOLVER);
		glutAddMenuEntry("Novo Jogo",NOVO_JOGO);
		glutAddMenuEntry("Sair",SAIR);

		// attach the menu to the right button
		glutAttachMenu(GLUT_RIGHT_BUTTON);

}

/****
 *		FUNÇÕES DE DESENHO
 ****/
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaTabela
 *  Description:  Fun��o que faz o desenho do tabuleiro
 * =====================================================================================
 */
void DesenhaTabela()
{
	int i;
	
	DesenhaFudoTabuleiro();
	// TRANSLATE    
	tr_x = tr_y = tr_z = 0;            		
	glTranslatef(tr_x, tr_y, tr_z);
	//tr_x = tr_y = tr_z = 0;

	// ESCALA
	glScalef(p_scale, p_scale, p_scale);
	p_scale = 1;
	
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	
	// Desenha as linhas
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
 *         Name:  DesenhaCaixaSelecionado
 *  Description:  Função que faz o desenho da caixa da casa selecionada
 * =====================================================================================
 */
void DesenhaCaixaSelecionado(GLfloat x1, GLfloat y1)
{	
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaCaixaErro
 *  Description:  Funçaoo que desenha a caixa de erro quando a valor a ser inserido já exite 
 *				  Na linha, coluna ou quadrado 3x3
 * =====================================================================================
 */
void DesenhaCaixaErro(GLfloat x1, GLfloat y1)
{	
	if((linha  < 0 || linha >= 9) || (coluna < 0 || coluna >= 9))
		return;
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaFudoTabuleiro
 *  Description:  Funçaoo que Desenha o fundo Brando do Tabuleiro
 * =====================================================================================
 */
void DesenhaFudoTabuleiro()
{	
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transformação corrente
	glLoadIdentity();
	
	glPushMatrix();
	
	glTranslatef(3.0f,2.0f,0.0f);
	
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	
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

/****
 *		FUNÇÕES DE DESENHO DE TEXTOS
 ****/
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  escreveMenuTextoFimDeJogo
 *  Description:  Função que desenha o texto do menu do fim do jogo
 * =====================================================================================
 */
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
 *         Name:  Escreve
 *  Description:  Fun��o que recebe o caracter e a posição que o mesmo vais ser exibido na
 *		    	  tela usando fonte de linhas
 * =====================================================================================
 */
void Escreve(float x, float y, char value)
{
	glPushMatrix();
	glRotatef(angulo_rotacao,0.0f,0.0f,1.0f);
	glTranslatef(3.2+x,2.1+y,0);	/* posiciona com base na matrix */
	glScalef(0.008, 0.008, 0.008);  /* diminui o tamanho do fonte	*/
	glLineWidth(2); 		/* define a espessura da linha 	*/
	glutStrokeCharacter(GLUT_STROKE_ROMAN,value);
	
	glPopMatrix();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  legenda
 *  Description:  Fun��o que recebe o texto que vai ser exebido como legenda no parte
 *				  Inferior da tela
 * =====================================================================================
 */
void legenda(char str[])
{
	  char *c;
	  c =str;
	  
	  glPushMatrix();
	  glTranslatef(0.1,0.2,0.0);
	  glScalef(0.002, 0.002, 0.002); 
  	  glColor3f( PRETO );
  	   glLineWidth( 2.0 );
  		
	  for (int i=0; c[i]!='\0'; i++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , c[i]);
	  }
	  glPopMatrix();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sudokuNOme
 *  Description:  Fun��o que recebe o escreve o nome Sudoku no canto superior esquerdo 
 *				  Da tela.
 * =====================================================================================
 */
void sudokuNome()
{
	  char *c;
	  c ="Sudoku";
	  
	  glPushMatrix();
	  glTranslatef(0.1,11.2,0.0);
	  glScalef(0.005, 0.005, 0.005); 
  	  glColor3f( PRETO );
  	   glLineWidth( 3.0 );
  		
	  for (int i=0; c[i]!='\0'; i++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , c[i]);
	  }
	  glPopMatrix();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  vcGanhou
 *  Description:  Funçaoo que recebe o texto que vai ser exebido quando o usuário 
 *				  Completar o Soduku
 * =====================================================================================
 */
void vcGanhou(char str[])
{
	  char *c;
	  c =str;
	  
	  glPushMatrix();
	  glTranslatef(3.0,1.0,0.0);
	  glScalef(0.008, 0.008, 0.008); 
	  glLineWidth(3.0 );
  	  glColor3f( VERMELHO );
  		
	  for (int i=0; c[i]!='\0'; i++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , c[i]);
	  }
	  
	  glLineWidth(1.0 );
	  glPopMatrix();
	  
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaNumeros
 *  Description:  Funçaoo que recebe escreve os numeros no tabuleiro
 * =====================================================================================
 */
void DesenhaNumeros()
{
	int x,y;
	char temp=1;
	
	for(x=0;x<9;x++)
		for(y=0;y<9;y++)
			if(sudoku[x][8-y] != -1) {
				if (Csudoku_preenchido.get(x, 8 - y) > 0)
					glColor3f(PRETO);
				else
					glColor3f(AZUL);

				Escreve(x, y, Csudoku.get(x, 8 - y) < 0 ? ' ' : Csudoku.get(x, 8 - y) + '0');
			}
}

/****
 *		FUNÇÕES HANDLE DE TECLAS ESPECIAIS
 ****/
 
 /* 
 * ===  FUNCTION  ======================================================================
 *         Name:  menuTecladoSpecHandle 
 *  Description:  interação com o menu inicial e de dificuldades
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
 *         Name:  sudokuTecladoSpecHandle
 *  Description:  Função callback chamada para gerenciar eventos de teclas especiais
 * =====================================================================================
 */
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

	glutPostRedisplay();
}

/****
 *		FUNÇÕES HANDLE DE TECLAS
 ****/
 
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
			break;
		case 1:
			tr_y = -10;
			glutDisplayFunc(desenhaCreditos);
			glutKeyboardFunc(creditosTecladoHandle);
			glutTimerFunc(1,creditosDelay,1);
			//tr_y = 0;
			// mostra os créditos [não implementado]
			break;
		case 2:
			// mensagem de texto bonitinha e sair [não implementado]
			exit(EXIT_SUCCESS);
			break;
		}
	}
	if (key == 27 || key == 'q' ||key == 'Q')
		exit(EXIT_SUCCESS);

	glutPostRedisplay();
}
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  menuDificuldadeTecladoHandle
 *  Description:  Função callback chamada para gerenciar eventos de teclas do menu de 
 *				  dificuldades
 * =====================================================================================
 */
 void menuDificuldadeTecladoHandle(unsigned char key, int x, int y)
{
	int dificuldade;

	if (key == 27) {
		glutDisplayFunc(desenhaMenu);
		glutKeyboardFunc(menuTecladoHandle);
		glutSpecialFunc(menuTecladoSpecHandle);
	}
	
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
		
		createGLUTMenus();
		setaValores(dificuldade);
		glutSpecialFunc(sudokuTecladoSpecHandle);
		glutDisplayFunc(Desenha);
		glutKeyboardFunc(tecladoSudoku);
	}
	else if (key == 'q' ||key == 'Q')
		exit(EXIT_SUCCESS);

	glutPostRedisplay();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Teclado
 *  Description:  Função callback chamada para gerenciar eventos de teclas
 * =====================================================================================
 */
void tecladoSudoku(unsigned char key, int x, int y)
{

	SudokuOpr op;
	int aux = 0;

	if(key >='1' && key<= '9')
	{
		if (Csudoku_preenchido.get(linha, coluna) == -1) {
			Csudoku.unmark(linha, coluna);
			
			if (!Csudoku.mark(key - '0', linha, coluna)) {
				DesenhaCaixaErro(linha+3,10-coluna);
				sleep(1);
			}
			
			Csudoku.print();
			
			if (Csudoku.isComplete()){
				isComplete();
			}
				
		}
	}
	
	if (key == 'h' || key == 'H') {
			Csudoku_preenchido.mark(Csudoku_resolvido.get(linha, coluna), linha, coluna);
			Csudoku_preenchido.print();
			
			if (!Csudoku.mark(Csudoku_preenchido.get(linha, coluna), linha, coluna)) {
				DesenhaCaixaErro(linha+3,10-coluna);
				sleep(1);
			}
			
			if (Csudoku.isComplete()){
				isComplete();
			}
				
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
			if(game_over == 0){
				op.solve(Csudoku_preenchido, 0);
				Csudoku = Csudoku_preenchido;
				glutTimerFunc(1,makedelay,1);
				angulo_rotacao = 0;
				game_over = 2;
			}
			break;
		case 13://Enter
			if(game_over==1 || game_over == 2){
				game_over = 0;
				if(box_select_fim==0){
					menu_limit = 3;
					
					glutDisplayFunc(desenhaMenuDificuldade);
					glutKeyboardFunc(menuDificuldadeTecladoHandle);
					glutSpecialFunc(menuTecladoSpecHandle);
					
				}
				else
					exit(EXIT_SUCCESS);
			}
			break;
		case 27://Esc
			glutDisplayFunc(desenhaMenuDificuldade);
			glutKeyboardFunc(menuDificuldadeTecladoHandle);
			glutSpecialFunc(menuTecladoSpecHandle);
			break;
		case 'q':
		case 'Q':
			exit(0);
			break;
		
	}
	
	DesenhaNumeros();
	glutPostRedisplay();

}

/****
 *		FUNÇÕES DE DESENHO DE TELA
 ****/
 
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
	
	sudokuNome();	
	
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  desenhaMenuDificuldade
 *  Description:  Função que desenha o menu de dificuldades
 * =====================================================================================
 */
 void desenhaMenuDificuldade()
{
	glutDestroyMenu(menu);

	// Limpa a janela de visualiza��o com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Define a Viewport 1
	glViewport(0, 0, largura, altura);
	
	sudokuNome();	

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
	
	sudokuNome();	
	
	// DESENHA AS LINHAS DA TABELA
	DesenhaTabela();
	
	// DESENHA OS NUMEROS DA MATRIX SUDOKU
	DesenhaNumeros();
	
	if(game_over==1 || game_over == 2){
		desenhaMenuFimDeJogo();

		if(game_over == 1)
			vcGanhou("Voce Conseguiu!!!");
	}
		
	legenda("Atalhos do Teclado: 'q' - SAIR | 'Esc'- voltar  |  'p' - Resolver Sudoku  |  'h' - Hint.");

	// Executa os comandos OpenGL 
	glFlush();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Inicializa
 *  Description:  Fun��o respons�vel por inicializar par�mentros e vari�veis
 * =====================================================================================
 */
void Inicializa (void)
{   
	glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
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
	
	printf ("Pixel Dimensions: %d x %d \n",glutGet (GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	printf ("Pixel Dimensions: %f x %f \n", glutGet (GLUT_SCREEN_WIDTH)/16.0, glutGet(GLUT_SCREEN_HEIGHT)/12.0);

	printf("%d %d", altura,largura);
	    
   	du_largura = glutGet (GLUT_SCREEN_WIDTH)/win_lagura;
    	du_altura = glutGet(GLUT_SCREEN_HEIGHT)/win_altura;
       	
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
 
	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5); 
    
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(MAX_X,MAX_Y); 
 
	// Cria a janela passando como argumento o t�tulo da mesma
	glutCreateWindow("Sudoku");
 
	// Registra a função callback de redesenho da janela de visualiza��o
 	glutDisplayFunc(desenhaMenu); 
	
	// Registra a função callback de redimensionamento da janela de visualiza��o
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a função callback para tratamento das teclas ASCII
	glutKeyboardFunc(menuTecladoHandle);
  
	// Registra a função de callback para tratamento de teclas especiais
	glutSpecialFunc(menuTecladoSpecHandle);

	// Registra a função callback para tratamento do mouse
	glutMouseFunc(GerenciaMouse);  
	
	// ATIVA A JANELA EM FULLSCREEN
	glutFullScreen();
	
	// Chama a função responsável por fazer as inicializa��es 
	Inicializa();
 
	// Inicia o processamento e aguarda intera��es do usu�rio
	glutMainLoop();
		printf("by\n");
	return 0;
}
