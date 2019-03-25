////*****************************************************
// UNIVASF- Universidade Federal do Vale do São Francisco
// Disciplina: Computação Gráfica
// Docente: Jorge Cavalcanti
// Discentes: Djeysi Kathleen Alves, Victor Gabriel Ferreira Barbosa, Jorge Gomes .
// Semestre: 2018.2
//
// labirinto:
//*****************************************************

/* teste-cronometro */
// #include "GLFW/glfw3.h"
int endMinutes = 0;
int endSeconds = 40;
int minutes = 0;
int seconds = 0;
/* fim-teste-cronometro */

#define nCores 3
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <FreeImage.h>
#define xStep 1  //incremento no eixo x
#define yStep 1  //incremento no eixo y
#define maxY 125 //valor máximo do eixo Y
#define maxX 105 //valor máximo do eixo X
#define nblocos 7
static GLuint texturasObjeto[8];
static GLuint texture = 0;
int auxiliar[nblocos] = {0, 0};
int aux1[nblocos];
int diff[nblocos];
float percentual[nblocos];
int subindo = 0;
int ladoPrecionado = 0;
int esquerdaPrecionado = 0;
int backupTy = 0;
int auxTx = 0;
int colidiu[nblocos] = {0, 0};
int countDir = 0;
// Variáveis que guardam a translação que será aplicada
// sobre a casinha
int Tx = 0;
int Ty = 0;
int pulando;
int animaX = 0;
int acabou = 0;
int count = 0;

// Variáveis que guardam os valores mínimos de x e y da
// casinha
GLfloat minX;
GLfloat minY;

// Variáveis que guardam a largura e altura da janela
GLfloat windowXmin, windowXmax;
GLfloat windowYmin, windowYmax;

//Variáveis que guardam os vértices do triangulo
int x1, x2, x3, x4;

int y4, y2, y3, y5;

struct cor
{
  float r;
  float g;
  float b;
};

struct objeto
{
  int x1, x2, y1, y2;
};

struct cor fundo;
struct cor paredes;
struct cor objeto;
struct cor coresParedes[nCores];
struct cor coresFundo[nCores];
struct cor coresObjeto[nCores];
struct objeto blocos[nblocos], tri, blocosAux[nblocos];
int i = 0;
int timer = 0;

void drawText(void *font, char *string)
{
  // Exibe caractere a caractere
  while (*string)
    glutBitmapCharacter(font, *string++);
}

void loserWindow()
{
  glutPostRedisplay();
  glutSwapBuffers();

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1, 0, 0);
  glRasterPos2f(70, 55);

  drawText(GLUT_BITMAP_TIMES_ROMAN_24, "Tempo esgotado. Que pena...");

  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
  glVertex2f(80.0, 30.0);
  glVertex2f(100.0, 30.0);
  glVertex2f(100.0, 36.0);
  glVertex2f(80.0, 36.0);
  glEnd();

  glColor3f(1, 1, 1);
  glRasterPos2f(81, 32);
  drawText(GLUT_BITMAP_9_BY_15, "Tente novamente");
}

void time()
{
  int milliSecondsSinceStart, diff;
  char timeString[6];

  milliSecondsSinceStart = glutGet(GLUT_ELAPSED_TIME);
  seconds = milliSecondsSinceStart / 1000;
  diff = endSeconds - seconds;
  minutes = seconds / 60;
  seconds %= 60;
  milliSecondsSinceStart %= 1000;

  printf("%d:%d:%d\n", (diff / 60), (diff % 60), milliSecondsSinceStart);

  sprintf(timeString, "%d:%d", (diff / 60), (diff % 60), milliSecondsSinceStart);

  glBegin(GL_LINE_LOOP);
  glColor3f(1, 1, 1);
  glVertex2f(173, 116);
  glVertex2f(183, 116);
  glVertex2f(183, 121);
  glVertex2f(173, 121);
  glEnd();

  glColor3f(1, 1, 1);
  if (!(diff / 60)){
    if(!(diff % 60))
      glutDisplayFunc(loserWindow);
    if((diff % 60) <= 30){
      glBegin(GL_LINE_LOOP);
      glColor3f(1, 0, 0);
      glVertex2f(173, 116);
      glVertex2f(183, 116);
      glVertex2f(183, 121);
      glVertex2f(173, 121);
      glEnd();
      glColor3f(1, 0, 0);
    }
  }

  glRasterPos2f(175, 117);
  drawText(GLUT_BITMAP_TIMES_ROMAN_24, timeString);
  // drawText(GLUT_BITMAP_9_BY_15, "OI");

}

void DesenhaTextoStroke()
{
  char *string;
  char aux[12];
  sprintf(aux, "%d", timer);
  string = &aux[0];
  glPushMatrix();
  glColor3f(0, 0, 1);
  glTranslatef(105, 105, 0);
  //  glScalef(0.09f,-0.08f,0);
  // Exibe caractere a caractere
  while (*string)
  {
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *string++);
  }
  glPopMatrix();
  timer++;
}

void Anima(int value)
{
  animaX += xStep;
  //Ty += yStep;

  // Redesenha a casinha em outra posição
  glutPostRedisplay();
  glutTimerFunc(150, Anima, 1);
}

void carregarTextura(GLuint texture, const char *filename)
{

  FIBITMAP *pImage = FreeImage_Load(FIF_PNG, filename, PNG_DEFAULT);
  int nWidth = FreeImage_GetWidth(pImage);
  int nHeight = FreeImage_GetHeight(pImage);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(pImage));

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

  FreeImage_Unload(pImage);
}

int detectaColisao(struct objeto linha)
{
  //     printf("\naq = %d\n",tri.y2);
  //     printf("\nlinha = %d\n", linha.y2);
  int colidiu = 0;
  // printf("\ncolidiu2 = %d\n", colidiu);
  if (linha.y2 == tri.y2 && tri.x2 >= linha.x1 && tri.x1 <= linha.x2)
  {
    tri.y1 = (y3 - y4) + linha.y2;
    Ty = tri.y1 - y3;
    colidiu = 1;
    //      printf("\n colidiu\n");
  }
  else
  {
    colidiu = 0;
  }
  if (tri.y1 >= linha.y1 && tri.y2 < linha.y2 && tri.x2 >= linha.x1 && tri.x1 <= linha.x2)
  {
    subindo = 0;
    //  printf("\nentrou aq");
  }
  return colidiu;
}

void carregarImagens(void)
{
  glGenTextures(11, texturasObjeto);
  carregarTextura(texturasObjeto[0], "imagem/sprite_05.png");
  carregarTextura(texturasObjeto[1], "imagem/sprite_06.png");
  carregarTextura(texturasObjeto[2], "imagem/sprite_07.png");
  carregarTextura(texturasObjeto[3], "imagem/sprite_08.png");
  carregarTextura(texturasObjeto[4], "imagem/sprite_09.png");
  carregarTextura(texturasObjeto[5], "imagem/sprite_10.png");
  carregarTextura(texturasObjeto[6], "imagem/sprite_01.png");
  carregarTextura(texturasObjeto[8], "imagem/sprite_11.png");
  carregarTextura(texturasObjeto[9], "imagem/sprite_12.png");
  carregarTextura(texturasObjeto[10], "imagem/sprite_13.png");
  carregarTextura(texturasObjeto[11], "imagem/sprite_14.png");
  carregarTextura(texturasObjeto[7], "imagem/sprite_00.png");
}

void Inicializa(void)
{

  FreeImage_Initialise(true);

  paredes = coresParedes[i];
  fundo = coresFundo[i];
  objeto = coresObjeto[i];

  blocos[0].x1 = 0;
  blocos[0].x2 = 30;
  blocos[0].y1 = 15;
  blocos[0].y2 = 20; //valores chatos

  blocos[1].x1 = 40;
  blocos[1].x2 = 60;
  blocos[1].y1 = 45;
  blocos[1].y2 = 50;

  blocos[2].x1 = 50;
  blocos[2].x2 = 75;
  blocos[2].y1 = 60;
  blocos[2].y2 = 65;

  blocos[3].x1 = 55;
  blocos[3].x2 = 95;
  blocos[3].y1 = 30;
  blocos[3].y2 = 35; //valores chatos

  blocos[4].x1 = 80;
  blocos[4].x2 = 95;
  blocos[4].y1 = 75;
  blocos[4].y2 = 80;

  blocos[5].x1 = 105;
  blocos[5].x2 = 115;
  blocos[5].y1 = 60;
  blocos[5].y2 = 65;

  blocos[6].x1 = 115;
  blocos[6].x2 = 130;
  blocos[6].y1 = 45;
  blocos[6].y2 = 50; //valores chatos

  // Define a cor de fundo da janela de visualização como branco

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  x1 = 0;
  x2 = 14;
  x3 = 14;
  x4 = 0;
  y4 = 20;
  y2 = 20;
  y3 = 35;
  y5 = 35;

  glClearColor(fundo.r, fundo.g, fundo.b, 0.0f);

  // Define a janela de visualização 2D
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, maxX, 0.0, maxY);
  glMatrixMode(GL_MODELVIEW); //para n manipular a matriz de projeção
  carregarImagens();
}

void desenhaTriangulo()
{
  tri.x1 = x1 + Tx - auxTx;
  tri.x2 = x2 + Tx - auxTx;

  //if(!colidiu){
  tri.y1 = y3 + Ty;
  tri.y2 = y4 + Ty;
  //}
  glColor3f(0, 0, 0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(tri.x1, tri.y2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(tri.x2, tri.y2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(tri.x2, tri.y1);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(tri.x1, tri.y1);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  for (i = 0; i < nblocos; i++)
  {
    colidiu[i] = detectaColisao(blocosAux[i]);
  }
}

void desenhaFundos()
{
  //DESENHA FUNDO DO JOGO
  glBindTexture(GL_TEXTURE_2D, texturasObjeto[6]);
  glColor3f(0, 0, 0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(115 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(115 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(115 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(230 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(230 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(115 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(230 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(345 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(345 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(230 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(345 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(460 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(460 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(345 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(460 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(575 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(575 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(460 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(575 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(690 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(690 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(575 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(690 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(805 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(805 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(690 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(805 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(920 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(920 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(805 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(920 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1035 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1035 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(920 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1035 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1150 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1150 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1035 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1150 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1265 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1265 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1150 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1265 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1380 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1380 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1265 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1380 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1495 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1495 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1380 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1495 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1610 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1610 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1495 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1610 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1725 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1725 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1610 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1725 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1840 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1840 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1725 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1840 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1955 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1955 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1840 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(1955 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(2070 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(2070 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1955 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(2070 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(2185 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(2185 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(2070 - animaX, 115);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(2185 - animaX, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(2300 - animaX, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(2300 - animaX, 115);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(2185 - animaX, 115);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{

  //1 mudança
  int flag = 0;
  int x = 0;
  coresParedes[0].r = 0;
  coresParedes[0].g = 0; //parede preto.
  coresParedes[0].b = 0;

  coresFundo[0].r = 1;
  coresFundo[0].g = 1; //fundo branco.
  coresFundo[0].b = 1;

  coresObjeto[0].r = 1;
  coresObjeto[0].g = 0; //objeto vermelho
  coresObjeto[0].b = 0;

  paredes = coresParedes[0];
  fundo = coresFundo[0];
  objeto = coresObjeto[0];

  //Tx += xStep;
  // Muda para o sistema de coordenadas do modelo
  glMatrixMode(GL_MODELVIEW);
  // Inicializa a matriz de transformação corrente
  glLoadIdentity();

  //Limpa a janela de visualização com a cor de fundo especificada
  glClear(GL_COLOR_BUFFER_BIT);

  //desenha fundos
  desenhaFundos();

  glBindTexture(GL_TEXTURE_2D, texturasObjeto[countDir]);

  //Desenha blocos.
  for (x = 0; x < nblocos; x++)
  {
    blocosAux[x].x1 = blocos[x].x1 - animaX;
    blocosAux[x].x2 = blocos[x].x2 - animaX;
    blocosAux[x].y1 = blocos[x].y1;
    blocosAux[x].y2 = blocos[x].y2;

    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex2f(blocosAux[x].x1, blocos[x].y1);
    glVertex2f(blocosAux[x].x1, blocos[x].y2);
    glVertex2f(blocosAux[x].x2, blocos[x].y2);
    glVertex2f(blocosAux[x].x2, blocos[x].y1);
    glEnd();

    if (colidiu[x])
    {
      aux1[x] = tri.x2;
      if (aux1[x] >= auxiliar[x])
      {
        auxiliar[x] = aux1[x];
        diff[x] = blocosAux[x].x2 - auxiliar[x];
      }
      else
      {
        auxiliar[x] = blocosAux[x].x2 - diff[x];
      }
    }
    else
    {
      if (aux1[x] != 0)
        auxiliar[x] = blocosAux[x].x2 - diff[x];
    }

    if (auxiliar[x] >= blocosAux[x].x2)
      auxiliar[x] = blocosAux[x].x2;

    if (auxiliar[x] != 0)
    {
      glColor3f(1, 0, 0);
      glBegin(GL_QUADS);
      glVertex2f(blocosAux[x].x1, blocos[x].y1);
      glVertex2f(blocosAux[x].x1, blocos[x].y2);
      glVertex2f(auxiliar[x], blocos[x].y2);
      glVertex2f(auxiliar[x], blocos[x].y1);
      glEnd();

      percentual[x] = 100 - (100 * (blocosAux[x].x2 - auxiliar[x]) / (blocosAux[x].x2 - blocosAux[x].x1));
    }
    else
    {
      percentual[x] = 0;
    }

    printf("\n percentual = %f", percentual[x]);
  }

  glPushMatrix();

  glTranslatef(1, 1, 0.0f);

  if (subindo)
  {
    //printf("%d", Ty - backupTy);

    if ((Ty - backupTy) <= 60)
    {
      Ty += 15;
      count += xStep;
      //glutPostRedisplay();
    }
    else
    {
      if (ladoPrecionado == 1)
      {
        Tx += 45;
        ladoPrecionado = 0;
      }
      if (esquerdaPrecionado == 1)
      {
        Tx -= 45;
        esquerdaPrecionado = 0;
      }
      subindo = 0;
      countDir = 10;
    }
  }
  else
  {
    ladoPrecionado = 0;
    esquerdaPrecionado = 0;

    for (int i = 0; i < nblocos; i++)
    {
      if (colidiu[i])
      {
        flag = 1;
        break;
      }
    }

    if (Ty > (backupTy + 8) && !subindo && !flag)
    {
      Ty -= 15;
      count += xStep;
    }
    else
    {
      pulando = 0;
      if (!flag)
        Ty = backupTy;
      auxTx = animaX - count;
    }
  }

  time();

  //desenhaFundos();
  desenhaTriangulo();
  // DesenhaTextoStroke();
  glutSwapBuffers();
}

// Função callback chamada para gerenciar eventos do teclado
// para teclas especiais, tais como F1, PgDn e Home
void TeclasEspeciais(int key, int x, int y)
{
  int i;
  if (acabou == 1)
  {
    if (key != GLUT_KEY_HOME)
    {
      return;
    }
    else
    {
      acabou = 0;
    }
  }

  if (key == GLUT_KEY_UP && !pulando)
  {
    pulando = 1;
    subindo = 1;
    backupTy = 0;
    if (countDir <= 2)
    {
      countDir = 8;
    }
    else
    {
      countDir = 9;
    }
  }

  if (key == GLUT_KEY_RIGHT)
  {
    countDir++;

    if (countDir > 2)
      countDir = 0;

    if (subindo)
    {
      ladoPrecionado = 1;
      Tx += 20;
      subindo = 0;
    }
    else
    {
      Tx += xStep;
    }
  }

  if (key == GLUT_KEY_LEFT)
  {
    if (countDir <= 2)
    {
      countDir = 3;
    }
    else
    {
      countDir++;
      if (countDir > 5)
        countDir = 3;
    }
    if (subindo)
    {
      ladoPrecionado = 1;
      Tx -= 20;
      subindo = 0;
    }
    else
    {
      Tx -= xStep;
    }
  }

  if (key == 116)
    system("killall mpg123");

  glutPostRedisplay();
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
  GLsizei largura, altura;

  // Evita a divisao por zero
  if (h == 0)
    h = 1;

  // Atualiza as variáveis
  largura = w;
  altura = h;

  // Especifica as dimensões da Viewport
  glViewport(0, 0, largura, altura);

  // Inicializa o sistema de coordenadas de projeção
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Estabelece a janela de seleção (esquerda, direita, inferior,
  // superior) mantendo a proporção com a janela de visualização
  if (largura <= altura)
  {
    gluOrtho2D(0.0f, 105.0f, 0.0f * altura / largura, 125.0f * altura / largura);
    windowXmin = 0.0f;
    windowXmax = 105.0f;
    windowYmin = 0.0f * altura / largura;
    windowYmax = 125.0f * altura / largura;
  }
  else
  {
    gluOrtho2D(0.0f * largura / altura, 105.0f * largura / altura, 0.0f, 125.0f);
    windowXmin = 0.0f * largura / altura;
    windowXmax = 105.0f * largura / altura;
    windowYmin = 0.0f;
    windowYmax = 125.0f;
  }
}

void Teclado(unsigned char key, int x, int y)
{
  if (key == 27)
  {
    system("killall mpg123");
    exit(0);
  }
}

// Programa Principal
int main(int argc, char **argv)
{

  int milliSecondsSinceStart = glutGet(GLUT_ELAPSED_TIME);

  endSeconds = (endSeconds + endMinutes * 60) - seconds;

  system("mpg123 Girls.mp3 &");
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(5, 5); // Especifica a posição inicial da janela GLUT
  glutInitWindowSize(400, 400); // Especifica o tamanho inicial em pixels da janela GLUT
  glutCreateWindow("mario");
  glutDisplayFunc(Desenha);
  glutReshapeFunc(AlteraTamanhoJanela); // Registra a função callback de redimensionamento da janela de visualização
  glutKeyboardFunc(Teclado);
  glutSpecialFunc(TeclasEspeciais);
  glutFullScreen();
  // Registra a função callback que será chamada a cada intervalo de tempo
  glutTimerFunc(150, Anima, 1);

  Inicializa();
  glutMainLoop();
}
