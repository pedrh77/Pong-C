#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float bolaX = 0, bolaY = 0, direcaoBolaX = 0.01, direcaoBolaY = 0.01;
float raquete1Y = 0, raquete2Y = 0;
int teclaW = 0, teclaS = 0, setaCima = 0, setaBaixo = 0;
int pontosJogador1 = 0, pontosJogador2 = 0;
int larguraJanela = 800, alturaJanela = 600;
float proporcaoTela = 800.0 / 600.0;
bool modoSelecionado = false;
bool modoContraComputador = false;

enum EstadoJogo
{
    MENU,
    JOGANDO
} estadoAtual = MENU;

void desenharTexto(float x, float y, void *fonte, const char *texto)
{
    glRasterPos2f(x, y);
    while (*texto)
    {
        glutBitmapCharacter(fonte, *texto++);
    }
}

void desenharMenu()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0.0);

    desenharTexto(-0.2f, 0.6f, GLUT_BITMAP_TIMES_ROMAN_24, "PING PONG");
    desenharTexto(-0.4f, 0.2f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE ENTER PARA COMECAR");
    desenharTexto(-0.4f, 0.0f, GLUT_BITMAP_HELVETICA_18, "1 - Jogar contra o Computador");
    desenharTexto(-0.4f, -0.2f, GLUT_BITMAP_HELVETICA_18, "2 - Jogar contra outro Jogador");
    desenharTexto(-0.4f, -0.4f, GLUT_BITMAP_HELVETICA_12, "ESC - Sair do jogo");

    glutSwapBuffers();
}

void desenharJogo()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    float posRaqueteX = 0.85f * proporcaoTela;
    float tamanhoBola = 0.02f * (proporcaoTela < 1 ? proporcaoTela : 1);

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
    glVertex2f(-posRaqueteX, raquete1Y + 0.2);
    glVertex2f(-posRaqueteX + 0.05, raquete1Y + 0.2);
    glVertex2f(-posRaqueteX + 0.05, raquete1Y - 0.2);
    glVertex2f(-posRaqueteX, raquete1Y - 0.2);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(posRaqueteX - 0.05, raquete2Y + 0.2);
    glVertex2f(posRaqueteX, raquete2Y + 0.2);
    glVertex2f(posRaqueteX, raquete2Y - 0.2);
    glVertex2f(posRaqueteX - 0.05, raquete2Y - 0.2);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(bolaX - tamanhoBola, bolaY + tamanhoBola);
    glVertex2f(bolaX + tamanhoBola, bolaY + tamanhoBola);
    glVertex2f(bolaX + tamanhoBola, bolaY - tamanhoBola);
    glVertex2f(bolaX - tamanhoBola, bolaY - tamanhoBola);
    glEnd();

    char pontuacao[50];
    sprintf(pontuacao, "Jogador 1: %d   Jogador 2: %d", pontosJogador1, pontosJogador2);
    desenharTexto(-0.25f, 0.9f, GLUT_BITMAP_HELVETICA_18, pontuacao);

    glutSwapBuffers();
}

void desenharTela()
{
    if (estadoAtual == MENU)
        desenharMenu();
    else
        desenharJogo();
}

float direcaoAleatoria()
{
    return (rand() % 2 == 0 ? 1 : -1) * (0.005f + ((float)rand() / RAND_MAX) * 0.01f);
}

void resetarBola()
{
    bolaX = 0;
    bolaY = 0;
    direcaoBolaX = direcaoAleatoria();
    direcaoBolaY = direcaoAleatoria();
}

void atualizarJogo(int valor)
{
    if (estadoAtual == JOGANDO)
    {
        bolaX += direcaoBolaX;
        bolaY += direcaoBolaY;

        if (bolaY > 1.0 || bolaY < -1.0)
            direcaoBolaY = -direcaoBolaY;

        float posRaqueteX = 0.85f * proporcaoTela;

        if (bolaX < -posRaqueteX + 0.05 && bolaX > -posRaqueteX &&
            bolaY < raquete1Y + 0.2 && bolaY > raquete1Y - 0.2)
        {
            direcaoBolaX = -direcaoBolaX;
        }
        if (bolaX > posRaqueteX - 0.05 && bolaX < posRaqueteX &&
            bolaY < raquete2Y + 0.2 && bolaY > raquete2Y - 0.2)
        {
            direcaoBolaX = -direcaoBolaX;
        }

        if (bolaX < -proporcaoTela)
        {
            pontosJogador2++;
            resetarBola();
        }
        else if (bolaX > proporcaoTela)
        {
            pontosJogador1++;
            resetarBola();
        }

        if (teclaW && raquete1Y < 0.8f)
            raquete1Y += 0.02f;
        if (teclaS && raquete1Y > -0.8f)
            raquete1Y -= 0.02f;

        if (modoContraComputador)
        {
            if (raquete2Y + 0.05f < bolaY)
                raquete2Y += 0.01f;
            else if (raquete2Y - 0.05f > bolaY)
                raquete2Y -= 0.01f;
        }
        else
        {
            if (setaCima && raquete2Y < 0.8f)
                raquete2Y += 0.02f;
            if (setaBaixo && raquete2Y > -0.8f)
                raquete2Y -= 0.02f;
        }

        glutPostRedisplay();
        glutTimerFunc(16, atualizarJogo, 0);
    }
}
void pressionarTecla(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
    case 27:
        exit(0);
        break;
    case '1':
        modoContraComputador = true; 
        estadoAtual = JOGANDO;
        resetarBola();
        glutTimerFunc(0, atualizarJogo, 0);
        break;
    case '2':
        modoContraComputador = false; 
        estadoAtual = JOGANDO;
        resetarBola();
        glutTimerFunc(0, atualizarJogo, 0);
        break;
    case 'w':
    case 'W':
        teclaW = 1;
        break;
    case 's':
    case 'S':
        teclaS = 1;
        break;
    case 8:
        estadoAtual = MENU;
        break;
    }
}
void soltarTecla(unsigned char tecla, int x, int y)
{
    if (tecla == 'w' || tecla == 'W')
        teclaW = 0;
    if (tecla == 's' || tecla == 'S')
        teclaS = 0;
}

void teclaEspecialPressionada(int tecla, int x, int y)
{
    if (tecla == GLUT_KEY_UP)
        setaCima = 1;
    if (tecla == GLUT_KEY_DOWN)
        setaBaixo = 1;
}

void teclaEspecialSolta(int tecla, int x, int y)
{
    if (tecla == GLUT_KEY_UP)
        setaCima = 0;
    if (tecla == GLUT_KEY_DOWN)
        setaBaixo = 0;
}

void redimensionarTela(int largura, int altura)
{
    larguraJanela = largura;
    alturaJanela = altura;
    proporcaoTela = (float)largura / (float)altura;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-proporcaoTela, proporcaoTela, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Pong Ping");

    glutDisplayFunc(desenharTela);
    glutKeyboardFunc(pressionarTecla);
    glutKeyboardUpFunc(soltarTecla);
    glutSpecialFunc(teclaEspecialPressionada);
    glutSpecialUpFunc(teclaEspecialSolta);
    glutReshapeFunc(redimensionarTela);

    glutMainLoop();
    return 0;
}
