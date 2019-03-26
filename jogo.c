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
#include <time.h>
#define nCores 3
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <FreeImage.h>
#define xStep 1  //incremento no eixo x
#define yStep 1  //incremento no eixo y
#define maxY 125 //valor máximo do eixo Y
#define maxX 105 //valor máximo do eixo X
#define nblocos 53
#define nmacas 5
#define nremedio 5

time_t start, end;
int endMinutes = 0;
int endSecondsPattern = 10;
int endSeconds = 10;
/* fim-teste-cronometro */

/* ranking */
int maxPoints = 200;
/* fim-ranking */


// int minutes = 0;
// int seconds = 0;
// int milliSecondsSinceStart;
/* fim-teste-cronometro */
int audio = 1;
static GLuint texturasObjeto[26];
static GLuint texture = 0;
int auxiliar[nblocos];
int aux1[nblocos];
int diff[nblocos];
float percentual[nblocos];
int subindo = 0;
int ladoPrecionado = 0;
int esquerdaPrecionado = 0;
int backupTy = 0;
int auxTx = 0;
int colidiu[nblocos];
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
struct objeto blocos[nblocos], tri, blocosAux[nblocos], maca[nmacas], remedio[nremedio];
int i = 0;
int timer = 0;
int macaPega[nmacas], remedioPega[nremedio];
void CriarMenu();
void Janela(int opcao);

void incrementaTempo(int inc){
  if (endSeconds + inc >= 60) {
      endMinutes++;
      endSeconds = (endSeconds + inc) % 60;
  }else{
    endSeconds += inc; 
  }
}

void decrementaTempo(int inc){
  if (endSeconds - inc < 0) {
      endMinutes--;
      endSeconds = 60 + (endSeconds - inc);
  }else{
    endSeconds -= inc; 
  }
}

void detectaColisaoMaca(){
  int k = 0;

   for(k = 0; k < nmacas; k++)
   {
     if(macaPega[k] == 1)
      continue;

      if(((maca[k].x1 - animaX) == tri.x2)  && (tri.y2 <= maca[k].y1)){
        macaPega[k] = 1;
        incrementaTempo(30);
        continue;
      }

      if(((maca[k].x2 - animaX) == tri.x1)  && (tri.y2 <= remedio[k].y1)){
        macaPega[k] = 1;
        incrementaTempo(30);
        continue;
      }

       if(((maca[k].x2 - animaX) >= tri.x2) && ((maca[k].x1 - animaX) <= tri.x2)){
         if((tri.y2 <= maca[k].y1)){
            macaPega[k] = 1;
            incrementaTempo(30);
         }
      }

   }

}

void desenhaMacas(){
  int j = 0;
  
  glBindTexture(GL_TEXTURE_2D, texturasObjeto[12]);

  for( j = 0; j < nmacas; j++)
  {
      if(macaPega[j] == 1)
        continue;

      //glColor3f(0, 1, 0);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(maca[j].x1 - animaX, maca[j].y1);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(maca[j].x2 - animaX, maca[j].y1);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(maca[j].x2 - animaX, maca[j].y2);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(maca[j].x1 - animaX, maca[j].y2);
      glEnd();
      glDisable(GL_TEXTURE_2D);
  }
  
}

void desenhaRemedio(){
  int j = 0;
  
  glBindTexture(GL_TEXTURE_2D, texturasObjeto[13]);

  for( j = 0; j < nremedio; j++)
  {
      if(remedioPega[j] == 1)
        continue;

      //glColor3f(0, 1, 0);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(remedio[j].x1 - animaX, remedio[j].y1);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(remedio[j].x2 - animaX, remedio[j].y1);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(remedio[j].x2 - animaX, remedio[j].y2);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(remedio[j].x1 - animaX, remedio[j].y2);
      glEnd();
      glDisable(GL_TEXTURE_2D);
  }
  
}

void detectaColisaoRemedio(){
  int k = 0;

   for(k = 0; k < nremedio; k++)
   {
     if(remedioPega[k] == 1)
      continue;
      
      if(((remedio[k].x1 - animaX) == tri.x2) && (tri.y2 <= remedio[k].y1)){
        remedioPega[k] = 1;
        decrementaTempo(30);
        continue;
      }

      if(((remedio[k].x2 - animaX) == tri.x1) && (tri.y2 <= remedio[k].y1)){
        remedioPega[k] = 1;
        decrementaTempo(30);
        continue;
      }

      if(((remedio[k].x2 - animaX) >= tri.x2) && ((remedio[k].x1 - animaX) <= tri.x2)){
        if((tri.y2 <= remedio[k].y1)){
          remedioPega[k] = 1;
          decrementaTempo(30);
          continue;
        }
      }

   
   }

}

void drawText(void *font, char *string)
{
  // Exibe caractere a caractere
  while (*string)
    glutBitmapCharacter(font, *string++);
}

int countPlataformsPainted(){
  int count = 0;
  int i;

  for(i = 0; i < nblocos; i++)
    if(percentual[i] >= 100)
      count++;

  return count;
}

float getPoints(int platformsPainted){
  return (maxPoints * platformsPainted) / nblocos;
}

int round(double number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

void loserWindow()
{
    char stringPoints[50];
    acabou = 1;
    
    sprintf(stringPoints, "%s %d %s", "Voce conseguiu", round(getPoints(countPlataformsPainted())), "pontos.");

    glutPostRedisplay();
    glutSwapBuffers();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 0, 0);
    glRasterPos2f(72, 80);

    drawText(GLUT_BITMAP_TIMES_ROMAN_24, "Tempo esgotado. Que pena... ");

        glRasterPos2f(50, 73);

     drawText(GLUT_BITMAP_TIMES_ROMAN_24, " Você jamais vai morrer de fome.  Se tivesse ficado na comunidade, não estaria. [...]");

    glRasterPos2f(20, 65);
     
      drawText(GLUT_BITMAP_TIMES_ROMAN_24, " Se tivesse ficado lá, teria morrido de outras fomes. Teria vivido uma vida com fome de sentimentos, de cores, de amor.");
    glRasterPos2f(74, 60);

    drawText(GLUT_BITMAP_TIMES_ROMAN_24, stringPoints);

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

void fallWindow()
{
  char stringPoints[50];
  acabou =1; 

  sprintf(stringPoints, "%s %d %s", "Voce conseguiu", round(getPoints(countPlataformsPainted())), "pontos.");

  glutPostRedisplay();
  glutSwapBuffers();

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1, 0, 0);
  glRasterPos2f(75, 70);

  drawText(GLUT_BITMAP_TIMES_ROMAN_24, "Voce caiu da plataforma.");

  glRasterPos2f(74, 60);

  drawText(GLUT_BITMAP_TIMES_ROMAN_24, stringPoints);

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
  char timeString[6];
  int diffAux;
  time(&end);
  diffAux = (int) difftime(end, start);

  diffAux = (endMinutes * 60 + endSeconds) - diffAux;

  sprintf(timeString, "%0d:%c%d", (diffAux / 60), ((diffAux % 60) < 10) ? '0' : 255, (diffAux % 60));

  glBegin(GL_LINE_LOOP);
  glColor3f(1, 1, 1);
  glVertex2f(162, 116);
  glVertex2f(183, 116);
  glVertex2f(183, 121);
  glVertex2f(162, 121);
  glEnd();

  glRasterPos2f(163, 117);
  drawText(GLUT_BITMAP_TIMES_ROMAN_24, "Tempo:");

  glColor3f(1, 1, 1);
  if (!(diffAux / 60))
  {
    if (!(diffAux % 60))
      glutDisplayFunc(loserWindow);
    if ((diffAux % 60) <= 30)
    {
      glBegin(GL_LINE_LOOP);
      glColor3f(1, 0, 0);
      glVertex2f(162, 116);
      glVertex2f(183, 116);
      glVertex2f(183, 121);
      glVertex2f(162, 121);
      glEnd();
      glColor3f(1, 0, 0);
    }
  }

  glRasterPos2f(175, 117);
  drawText(GLUT_BITMAP_TIMES_ROMAN_24, timeString);
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
      printf("\nentrou aq");
  }
  return colidiu;
}

void carregarImagens(void)
{
  glGenTextures(25, texturasObjeto);
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
  carregarTextura(texturasObjeto[12], "imagem/sprite_15.png");
  carregarTextura(texturasObjeto[13], "imagem/sprite_16.png");
  carregarTextura(texturasObjeto[14], "imagem/tutorial.png");
  carregarTextura(texturasObjeto[15], "imagem/tutorial2.png");
  carregarTextura(texturasObjeto[16], "imagem/sprite_17.png");
  carregarTextura(texturasObjeto[17], "imagem/sprite_18.png");
  carregarTextura(texturasObjeto[18], "imagem/sprite_19.png");
  carregarTextura(texturasObjeto[19], "imagem/sprite_20.png");
  carregarTextura(texturasObjeto[20], "imagem/sprite_21.png");
  carregarTextura(texturasObjeto[21], "imagem/sprite_22.png");
  carregarTextura(texturasObjeto[22], "imagem/sprite_23.png");
  carregarTextura(texturasObjeto[23], "imagem/sprite_24.png");
  carregarTextura(texturasObjeto[24], "imagem/sprite_25.png");
  carregarTextura(texturasObjeto[25], "imagem/sprite_26.png");
  carregarTextura(texturasObjeto[7], "imagem/sprite_00.png");
}

void Inicializa(void)
{

  FreeImage_Initialise(true);

  paredes = coresParedes[i];
  fundo = coresFundo[i];
  objeto = coresObjeto[i];
  
  maca[0].x1 = 60;
  maca[0].x2 = 70;
  maca[0].y1 = 70;
  maca[0].y2 = 80;

  maca[1].x1 = 220;
  maca[1].x2 = 230;
  maca[1].y1 = 95;
  maca[1].y2 = 105;

  maca[2].x1 = 480;
  maca[2].x2 = 490;
  maca[2].y1 = 80;
  maca[2].y2 = 90;  
  
  maca[3].x1 = 835;
  maca[3].x2 = 845;
  maca[3].y1 = 80;
  maca[3].y2 = 90;  
  
  maca[4].x1 = 1010;
  maca[4].x2 = 1020;
  maca[4].y1 = 65;
  maca[4].y2 = 75;  
  
  remedio[0].x1 = 70;
  remedio[0].x2 = 80;
  remedio[0].y1 = 40;
  remedio[0].y2 = 50;
  
  
  remedio[1].x1 = 290;
  remedio[1].x2 = 300;
  remedio[1].y1 = 35;
  remedio[1].y2 = 45;
  
  remedio[2].x1 = 560;
  remedio[2].x2 = 570;
  remedio[2].y1 = 20;
  remedio[2].y2 = 30;
  
  
  remedio[3].x1 = 980;
  remedio[3].x2 = 990;
  remedio[3].y1 = 35;
  remedio[3].y2 = 45;
  
  
  remedio[4].x1 = 710;
  remedio[4].x2 = 720;
  remedio[4].y1 = 20;
  remedio[4].y2 = 30;
  
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
  
  
  blocos[7].x1 = 135;
   blocos[7].x2 = 165;
   blocos[7].y1 = 30;
   blocos[7].y2 = 35; //valores chatos

   blocos[8].x1 = 130;
   blocos[8].x2 = 155;
   blocos[8].y1 = 75;
   blocos[8].y2 = 80;
   
   blocos[9].x1 = 175;
   blocos[9].x2 = 195;
   blocos[9].y1 = 45;
   blocos[9].y2 = 50;

   blocos[10].x1 = 175;
   blocos[10].x2 = 190;
   blocos[10].y1 = 90;
   blocos[10].y2 = 95; //valores chatos

   blocos[11].x1 = 220;
   blocos[11].x2 = 235;
   blocos[11].y1 = 90;
   blocos[11].y2 = 95;
   
   blocos[12].x1 = 250;
   blocos[12].x2 = 270;
   blocos[12].y1 = 60;
   blocos[12].y2 = 65;
   
   blocos[13].x1 = 280;
   blocos[13].x2 = 300;
   blocos[13].y1 = 30;
   blocos[13].y2 = 35; 

   blocos[14].x1 = 200;
   blocos[14].x2 = 225;
   blocos[14].y1 = 60;
   blocos[14].y2 = 65;
   
//////////////////////////////////////////////////

  blocos[15].x1 = 270;
  blocos[15].x2 = 290;
  blocos[15].y1 = 75;
  blocos[15].y2 = 80;
  
  blocos[16].x1 = 310;
  blocos[16].x2 = 335;
  blocos[16].y1 = 45;
  blocos[16].y2 = 50;
  
  blocos[17].x1 = 335;
  blocos[17].x2 = 355;
  blocos[17].y1 = 60;
  blocos[17].y2 = 65;
  
  blocos[18].x1 = 370;
  blocos[18].x2 = 390;
  blocos[18].y1 = 50;
  blocos[18].y2 = 45;
  
  blocos[19].x1 = 405;
  blocos[19].x2 = 425;
  blocos[19].y1 = 60;
  blocos[19].y2 = 65;
  
  blocos[20].x1 = 440;
  blocos[20].x2 = 460;
  blocos[20].y1 = 45;
  blocos[20].y2 = 50;
  
  blocos[21].x1 = 470;
  blocos[21].x2 = 490;
  blocos[21].y1 = 60;
  blocos[21].y2 = 65;
  
  blocos[22].x1 = 500;
  blocos[22].x2 = 520;
  blocos[22].y1 = 75;
  blocos[22].y2 = 80;
  
  blocos[23].x1 = 530;
  blocos[23].x2 = 550;
  blocos[23].y1 = 90;
  blocos[23].y2 = 95;
  
  blocos[24].x1 = 560;
  blocos[24].x2 = 580;
  blocos[24].y1 = 20;
  blocos[24].y2 = 15;
  
  blocos[25].x1 = 590;
  blocos[25].x2 = 620;
  blocos[25].y1 = 30;
  blocos[25].y2 = 35;
  
  blocos[26].x1 = 630;
  blocos[26].x2 = 650;
  blocos[26].y1 = 45;
  blocos[26].y2 = 50;
  
  blocos[27].x1 = 665;
  blocos[27].x2 = 680;
  blocos[27].y1 = 30;
  blocos[27].y2 = 35;
  
  blocos[28].x1 = 675;
  blocos[28].x2 = 700;
  blocos[28].y1 = 60;
  blocos[28].y2 = 65;
  
  blocos[29].x1 = 695;
  blocos[29].x2 = 720;
  blocos[29].y1 = 15;
  blocos[29].y2 = 20;
  
  blocos[30].x1 = 725;
  blocos[30].x2 = 740;
  blocos[30].y1 = 45;
  blocos[30].y2 = 50;
  
  blocos[31].x1 = 750;
  blocos[31].x2 = 760;
  blocos[31].y1 = 60;
  blocos[31].y2 = 65;
  
  blocos[32].x1 = 770;
  blocos[32].x2 = 780;
  blocos[32].y1 = 60;
  blocos[32].y2 = 65;
  
  blocos[33].x1 = 795;
  blocos[33].x2 = 810;
  blocos[33].y1 = 45;
  blocos[33].y2 = 50;
  
  blocos[34].x1 = 820;
  blocos[34].x2 = 830;
  blocos[34].y1 = 60;
  blocos[34].y2 = 65;
  
  blocos[35].x1 = 835;
  blocos[35].x2 = 850;
  blocos[35].y1 = 75;
  blocos[35].y2 = 80;
  
  blocos[36].x1 = 835;
  blocos[36].x2 = 860;
  blocos[36].y1 = 15;
  blocos[36].y2 = 20;
  
  blocos[37].x1 = 870;
  blocos[37].x2 = 890;
  blocos[37].y1 = 30;
  blocos[37].y2 = 35;
  
  blocos[38].x1 = 900;
  blocos[38].x2 = 910;
  blocos[38].y1 = 45;
  blocos[38].y2 = 50;
  
  blocos[39].x1 = 930;
  blocos[39].x2 = 940;
  blocos[39].y1 = 45;
  blocos[39].y2 = 50;
  
  blocos[40].x1 = 960;
  blocos[40].x2 = 970;
  blocos[40].y1 = 45;
  blocos[40].y2 = 50;
  
  blocos[41].x1 = 980;
  blocos[41].x2 = 990;
  blocos[41].y1 = 30;
  blocos[41].y2 = 35;
  
  blocos[42].x1 = 1000;
  blocos[42].x2 = 1010;
  blocos[42].y1 = 45;
  blocos[42].y2 = 50;
  
  blocos[43].x1 = 1015;
  blocos[43].x2 = 1020;
  blocos[43].y1 = 60;
  blocos[43].y2 = 65;
  
  blocos[44].x1 = 1030;
  blocos[44].x2 = 1040;
  blocos[44].y1 = 60;
  blocos[44].y2 = 65;
  
  blocos[45].x1 = 1060;
  blocos[45].x2 = 1070;
  blocos[45].y1 = 60;
  blocos[45].y2 = 65;
  
  blocos[46].x1 = 1090;
  blocos[46].x2 = 1100;
  blocos[46].y1 = 60;
  blocos[46].y2 = 65;
  
  blocos[47].x1 = 1110;
  blocos[47].x2 = 1130;
  blocos[47].y1 = 30;
  blocos[47].y2 = 35;
  
  blocos[48].x1 = 1145;
  blocos[48].x2 = 1160;
  blocos[48].y1 = 45;
  blocos[48].y2 = 50;
  
  blocos[49].x1 = 1160;
  blocos[49].x2 = 1185;
  blocos[49].y1 = 30;
  blocos[49].y2 = 35;
  
  blocos[50].x1 = 1200;
  blocos[50].x2 = 1215;
  blocos[50].y1 = 45;
  blocos[50].y2 = 50;
  
  blocos[51].x1 = 1230;
  blocos[51].x2 = 1250;
  blocos[51].y1 = 60;
  blocos[51].y2 = 65;
  
  blocos[52].x1 = 1260;
  blocos[52].x2 = 1270;
  blocos[52].y1 = 75;
  blocos[52].y2 = 80;
  
  blocos[53].x1 = 1255;
  blocos[53].x2 = 1280;
  blocos[53].y1 = 30;
  blocos[53].y2 = 35;
  

  // Define a cor de fundo da janela de visualização como branco

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  x1 = 10;
  x2 = 19;
  x3 = 19;
  x4 = 10;
  y4 = 50;
  y2 = 50;
  y3 = 65;
  y5 = 65;

  glClearColor(fundo.r, fundo.g, fundo.b, 0.0f);

  // Define a janela de visualização 2D
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, maxX, 0.0, maxY);
  glMatrixMode(GL_MODELVIEW); //para n manipular a matriz de projeção
  carregarImagens();
}

void desenhaTriangulo()
{

  if(Tx == 0 && auxTx > 0){
    auxTx = 0;
    Tx+= xStep;
    countDir++;
    tri.x1 = x1 + Tx;
    tri.x2 = x2 + Tx;
  }else{
    tri.x1 = x1 + Tx - auxTx;
    tri.x2 = x2 + Tx - auxTx;
  }
  printf("\n Tx = %d", x1); 
  printf("\n auxtx = %d", x2); 
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

  detectaColisaoMaca();
  detectaColisaoRemedio();
}

void desenhaFundos()
{
  //DESENHA FUNDO DO JOGO
  glBindTexture(GL_TEXTURE_2D, texturasObjeto[16]);
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

  glBindTexture(GL_TEXTURE_2D, texturasObjeto[17]);
  
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

  glBindTexture(GL_TEXTURE_2D, texturasObjeto[18]);
  
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

    glBindTexture(GL_TEXTURE_2D, texturasObjeto[19]);
    
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

  glBindTexture(GL_TEXTURE_2D, texturasObjeto[20]);
    
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
  
  glBindTexture(GL_TEXTURE_2D, texturasObjeto[21]);

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
  
    glBindTexture(GL_TEXTURE_2D, texturasObjeto[22]);
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

    glBindTexture(GL_TEXTURE_2D, texturasObjeto[23]);
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

 glBindTexture(GL_TEXTURE_2D, texturasObjeto[24]);
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

    glBindTexture(GL_TEXTURE_2D, texturasObjeto[25]);
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

  if(Ty <= -135)
       glutDisplayFunc(fallWindow);

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

    // printf("\n percentual = %f", percentual[x]);
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

    if (!subindo && !flag)
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
  desenhaMacas();
  desenhaRemedio();
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

void CriarMenu()
{
    int menu;
    menu = glutCreateMenu(Janela);
    glutAddMenuEntry("Desligar som", 0);
    glutAddMenuEntry("Ligar som", 1);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Janela(int opcao)
{
    switch(opcao){
        case 0:
           if(audio == 1)
                system("killall mpg123"); 
           audio = 0; 
        break;
        case 1:
           if(audio == 0)
                system("mpg123 Girls.mp3 &");
           audio = 1;
        break;
   }
    
    glutPostRedisplay();
}


void Teclado(unsigned char key, int x, int y)
{
  if (key == 27)
  {
    system("killall mpg123");
    exit(0);
  }
}

void mouseKeys(int btn, int state, int x, int y)
{
  int j;
  if (btn == GLUT_LEFT_BUTTON)
  {
    if ((x >= 594 && x <= 739) && (y >= 543 && y <= 579))
    {
      Tx = 0;
      Ty = 0;
      animaX = 0;
      countDir = 0;
      pulando = 0;
      animaX = 0;
      acabou = 0;
      count = 0;
      subindo = 0;
      ladoPrecionado = 0;
      esquerdaPrecionado = 0;
      i = 0;
      
      for(j = 0; j < nblocos; j++)
      {
        auxiliar[j] = 0;
        aux1[j] = 0;
      }
      
      time(&start);
      glutDisplayFunc(Desenha);
      return;
    }
  }
}

// Programa Principal
int main(int argc, char **argv)
{
  time(&start);
  // system("mpg123 Girls.mp3 &");
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(5, 5); // Especifica a posição inicial da janela GLUT
  glutInitWindowSize(400, 400); // Especifica o tamanho inicial em pixels da janela GLUT
  glutCreateWindow("mario");
   CriarMenu();
  glutDisplayFunc(Desenha);
  glutReshapeFunc(AlteraTamanhoJanela); // Registra a função callback de redimensionamento da janela de visualização
  glutKeyboardFunc(Teclado);
  glutSpecialFunc(TeclasEspeciais);
  glutMouseFunc(mouseKeys);
  glutFullScreen();
  //Registra a função callback que será chamada a cada intervalo de tempo
  glutTimerFunc(150, Anima, 1);

  Inicializa();
   glutMainLoop();
}
