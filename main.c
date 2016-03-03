/*
 * =====================================================================================
 *
 *       Filename:  vertices.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10-12-2015 16:38:25
 *       Revision:  none
 *       Compiler:  gcc main.c -o main -lGL -lGLU -lglut  && ./main
 *       
 *         Author:  GUSTAVO MARQUES (), GUTODISSE AT GMAIL DOT COM
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <sys/ioctl.h>


/*-----------------------------------------------------------------------------
 *  MACROS
 *-----------------------------------------------------------------------------*/
#define COR_V1 c1[0],c1[1],c1[2]
#define COR_V2 c2[0],c2[1],c2[2]
#define COR_V3 c3[0],c3[1],c3[2]
#define COR_V4 c4[0],c4[1],c4[2]

/*-----------------------------------------------------------------------------
 *  CONSTANTES
 *-----------------------------------------------------------------------------*/

// TELA
#define MAX_X 600
#define MAX_Y 600

  
// 
#define X1 0
#define X2 6
#define X3 7
#define X4 5
#define Y1 8
#define Y2 8
#define Y3 9
#define Y4 5

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
char sudoku[9][9];
float win, aspecto;
int largura, altura;

float largura_tela, altura_tela;
float win_lagura =  16.0;
float win_altura = 12.0;


int tr_x=0, tr_y=0, tr_z=0; // TRANSLATE

float c1[3]={PRETO}, 
      c2[3]={AMARELO},
      c3[3]={BRANCO},
      c4[3]={VERMELHO};

float p_scale=1;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  renderStrokeFontLetter
 *  Description:  Função que recebe a fonte e um texto por parâmetro para ser exibido na
 *		    tela usando fonte de linhas
 * =====================================================================================
 */
void Escreve(float x, float y, char value)
{
	glPushMatrix();
	
	//glLoadIdentity(); // reinicializa as transformações  
	glTranslatef(3.2+x,2.1+y,0);	/* posiciona com base na matrix */
	glScalef(0.008, 0.008, 0.008);  /* diminui o tamanho do fonte	*/
	glLineWidth(2); 		/* define a espessura da linha 	*/
	//temp = x+48; 			/* CONVERSÃO ASCII 		*/
	glutStrokeCharacter(GLUT_STROKE_ROMAN,value);
	
	glPopMatrix();
}

void DesenhaNumeros()
{
	int x,y;
	char temp=1;
	glColor3f(PRETO);
	
	
	for(x=0;x<9;x++)
		for(y=0;y<9;y++)
			if(sudoku[x][8-y] != -1)
				Escreve(x,y,sudoku[x][8-y]);
	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaPoligono
 *  Description:  Função que faz o desenho do tabuleiro
 * =====================================================================================
 */
void DesenhaTabela()
{
	int i;
	
	// TRANSLATE                		
	glTranslatef(tr_x, tr_y, tr_z);
	tr_x = tr_y = tr_z = 0;

	// ESCALA
	glScalef(p_scale, p_scale, p_scale);
	p_scale = 1;
	
	// Desenha as linhas
	//for(i=-4; i<6; i++)
	for(i=2; i<12; i++)
	{
		glBegin(GL_LINES);      
			glColor3f( PRETO );
			glVertex2f( 3.0 , i );
		  	glVertex2f(  12.0 , i );       
		glEnd();    
	}
    	
    	// Desenha as colunas
    	//for(i=-4; i<6; i++)
    	for(i=3.5; i<13; i++)
	{
		// Desenha o poligono  
			glBegin(GL_LINES);      
				glColor3f( PRETO );
				glVertex2f( i, 2 );
			  	glVertex2f( i,  11 );       
			glEnd();    
    	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DesenhaPoligono
 *  Description:  Função que faz o desenho do POLIGONO
 * =====================================================================================
 */
void DesenhaPoligono()
{
	// TRANSLATE                		
	glTranslatef(tr_x, tr_y, tr_z);
	tr_x = tr_y = tr_z = 0;

	// ESCALA
	glScalef(p_scale, p_scale, p_scale);
	p_scale = 1;

	// Desenha o poligono  
	glBegin(GL_POLYGON);      
		
		glColor3f ( COR_V1);
		glVertex2f( X1,Y1 );
	  	
		glColor3f ( COR_V2);	
		glVertex2f( X2,Y2 );
	 	
		glColor3f ( COR_V3);	
		glVertex2f( X3,Y3 );

		glColor3f ( COR_V4);	
		glVertex2f( X4,Y4 );             
	glEnd();    
    
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Desenha
 *  Description:  Função de callback de redesenho da janela de visualização
 * =====================================================================================
 */
void Desenha(void)
{
	// Limpa a janela de visualização com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);

	// Define a Viewport 1
	glViewport(0, 0, largura, altura);
	// Desenha a casa na Viewport 1                        
	//DesenhaPoligono();
	
	// DESENHA AS LINHAS DA TABELA
	DesenhaTabela();
	
	// DESENHA OS NUMEROS DA MATRIX SUDOKU
	DesenhaNumeros();
	
	// Executa os comandos OpenGL 
	glFlush();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  AlteraTamanhoJanela
 *  Description:  Função callback chamada quando o tamanho da janela é alterado
 * =====================================================================================
 */
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{	printf("here\n");
	// Evita a divisao por zero
	if(h == 0) h = 1;
	
	// Atualiza as variáveis
	largura = w;
	altura = h;

	aspecto = (float) largura/altura;	
	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Estabelece a janela de seleção (esquerda, direita, inferior, 
	// superior) mantendo a proporção com a janela de visualização
	//gluOrtho2D (-win*aspecto, win*aspecto, -win, win);
	
//	gluOrtho2D(-7.5f,8.5f,-5.5,6.5f);
	gluOrtho2D(0.0f,16.0f,0.0f,12.0f);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TecladoEspecial
 *  Description:  Função callback chamada para gerenciar eventos de teclas especiais
 * =====================================================================================
 */
void TecladoEspecial (int key, int x, int y) 
{
	switch(key)
	{
		case GLUT_KEY_PAGE_UP:
			p_scale = 1.5;
		break;
		case GLUT_KEY_PAGE_DOWN:
			p_scale = 0.5;
		break;
		case GLUT_KEY_LEFT:
			tr_x = -1;
		break;
		case GLUT_KEY_RIGHT:
			tr_x = 1;
		break;
	}
	glutPostRedisplay();	
}	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Teclado
 *  Description:  Função callback chamada para gerenciar eventos de teclas
 * =====================================================================================
 */
void Teclado (unsigned char key, int x, int y)
{
	// 0       1      2
	// RED - GREEN - BLUE
	if( key>='0' && key<= '9')
	{
		sudoku[linha][coluna] = key;
	}
	switch(key)
	{	
		case 127:
			sudoku[linha][coluna] = -1;
		break;
		case 27:
		case 'q':
		case 'Q':
			exit(0);
		break;
	}
	DesenhaNumeros();
	glutPostRedisplay();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GerenciaMouse
 *  Description:  Função callback chamada para gerenciar eventos do mouse
 * =====================================================================================
 */
void GerenciaMouse(int button, int state, int x, int y)
{       
	int i;
	if (button == GLUT_RIGHT_BUTTON ){
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		linha 	= (int)((x/largura_tela)-3);
		coluna  = (int)((y/altura_tela)-1);
		
		//printf("coluna[%d]linha[%d]\n",(int) ((x-405)/70) ,(int) ((y-36)/69) ); 
		printf("coluna[%d]linha[%d]\n", (int)((x/largura_tela)-3),(int)((y/altura_tela)-1)); 

	}   

	/*
	if (button == GLUT_LEFT_BUTTON || button ==GLUT_RIGHT_BUTTON) 
	{
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity(); 
	    gluOrtho2D (-win*aspecto, win*aspecto, -win, win);
	    glutPostRedisplay();
	}
	*/
	//glutPostRedisplay();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Inicializa
 *  Description:  Função responsável por inicializar parâmentros e variáveis
 * =====================================================================================
 */
void Inicializa (void)
{   
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	win = 5.5f;
	
	//gluOrtho2D(-5.0f,5.0f,-5.0f,5.0f);
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
    
    largura_tela = glutGet (GLUT_SCREEN_WIDTH)/win_lagura;
    altura_tela = glutGet(GLUT_SCREEN_HEIGHT)/win_altura;
	
	
       	
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
 
	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5); 
    
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(MAX_X,MAX_Y); 
 
	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Sudoku");
 
	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);
  
	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a função callback para tratamento das teclas ASCII
	glutKeyboardFunc (Teclado);
  
	// Registra a função de callback para tratamento de teclas especiais
	glutSpecialFunc (TecladoEspecial);

	// Registra a função callback para tratamento do mouse
	glutMouseFunc(GerenciaMouse);  
	
	// ATIVA A JANELA EM FULLSCREEN
	glutFullScreen();
	
	// Chama a função responsável por fazer as inicializações 
	Inicializa();
 
	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();
 
	return 0;
}
