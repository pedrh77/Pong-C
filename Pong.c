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
time_t tempoInicial;
int tempoDecorrido = 0;
const float velocidade_bola = 0.01f;
bool jogoPausado = false;
int limitePontos = 3; // pode mudar para outro valor q os jogafores desejarem

enum Dificuldade {
    FACIL = 0,
    MEDIO = 1,
    DIFICIL = 2
};

enum Dificuldade dificuldadeAtual = MEDIO;

enum EstadoJogo
{
    MENU,
    JOGANDO,
    FIM
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
    desenharTexto(-0.4f, 0.0f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE 1 PARA JOGAR CONTRA COMPUTADOR");
    desenharTexto(-0.4f, -0.2f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE 2 PARA JOGAR CONTRA USUARIO");
    desenharTexto(-0.4f, -0.4f, GLUT_BITMAP_HELVETICA_12, "PRESSIONE ESC PARA SAIR");

    pontosJogador1 = 0;
    pontosJogador2 = 0;

    glutSwapBuffers();
}

void desenharJogo()
{
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    float posRaqueteX = 0.85f * proporcaoTela;
    float tamanhoBola = 0.02f * (proporcaoTela < 1 ? proporcaoTela : 1);

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_QUADS); //RAQUETE 1
    glVertex2f(-posRaqueteX, raquete1Y + 0.2);
    glVertex2f(-posRaqueteX + 0.05, raquete1Y + 0.2);
    glVertex2f(-posRaqueteX + 0.05, raquete1Y - 0.2);
    glVertex2f(-posRaqueteX, raquete1Y - 0.2);
    glEnd();

    
    glBegin(GL_QUADS); //RAQUETE 2
    glVertex2f(posRaqueteX - 0.05, raquete2Y + 0.2);
    glVertex2f(posRaqueteX, raquete2Y + 0.2);
    glVertex2f(posRaqueteX, raquete2Y - 0.2);
    glVertex2f(posRaqueteX - 0.05, raquete2Y - 0.2);
    glEnd();

    glBegin(GL_QUADS); //BOLINHA
    glVertex2f(bolaX - tamanhoBola, bolaY + tamanhoBola);
    glVertex2f(bolaX + tamanhoBola, bolaY + tamanhoBola);
    glVertex2f(bolaX + tamanhoBola, bolaY - tamanhoBola);
    glVertex2f(bolaX - tamanhoBola, bolaY - tamanhoBola);
    glEnd();

    char cronometro[50];
    sprintf(cronometro, "Tempo: %d s", tempoDecorrido);
    desenharTexto(0.6f, 0.9f, GLUT_BITMAP_HELVETICA_18, cronometro);

    char pontuacao[50];
    sprintf(pontuacao, "Jogador 1: %d   Jogador 2: %d", pontosJogador1, pontosJogador2);
    desenharTexto(-0.25f, 0.9f, GLUT_BITMAP_HELVETICA_18, pontuacao);

    if (jogoPausado)
    {
    glColor3f(1.0, 0.0, 0.0);
    desenharTexto(-0.1f, 0.0f, GLUT_BITMAP_HELVETICA_18, "PAUSADO - Pressione 'P' para continuar");
    }

    glutSwapBuffers();
}

void desenharFIM()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0.0);

    char pontuacaoj1[50];
    sprintf(pontuacaoj1, "Jogador 1: %d", pontosJogador1);
    char pontuacaoj2[50];
    sprintf(pontuacaoj2, "Jogador 2: %d", pontosJogador2);


    desenharTexto(-0.2f, 0.6f, GLUT_BITMAP_TIMES_ROMAN_24, "PONTUAÇÃO FINAL:");
    desenharTexto(-0.2f, 0.4f, GLUT_BITMAP_HELVETICA_18, pontuacaoj1);
    desenharTexto(-0.2f, 0.2f, GLUT_BITMAP_HELVETICA_18, pontuacaoj2);
    desenharTexto(-0.2f, 0.0f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE BACKSPACE PARA MENU");
    desenharTexto(-0.2f, -0.2f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE ESC PARA SAIR");
    

    glutSwapBuffers();
}

void desenharTela()
{
    if (estadoAtual == MENU)
        desenharMenu();
    else if (estadoAtual == FIM)
        desenharFIM();
    else
        desenharJogo();
}


float direcaoAleatoria()
{
    return (rand() % 2 == 0) ? 1.0f : -1.0f;
}

void resetarBola()
{
    bolaX = 0;
    bolaY = 0;

    float velocidade;

    switch (dificuldadeAtual)
    {
        case FACIL:
            velocidade = 0.007f;
            break;
        case MEDIO:
            velocidade = 0.01f;
            break;
        case DIFICIL:
            velocidade = 0.015f;
            break;
        default:
            velocidade = 0.01f;
    }

    direcaoBolaX = direcaoAleatoria() * velocidade;
    direcaoBolaY = direcaoAleatoria() * velocidade;
}


void atualizarJogo(int valor)
{
    if (estadoAtual == JOGANDO && !jogoPausado)
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
           if (pontosJogador2 >= 3){
            estadoAtual = FIM;
            }
        }        
        else if (bolaX > proporcaoTela)
        {
            pontosJogador1++;
            resetarBola();
            if (pontosJogador1 >= 3){
            estadoAtual = FIM;
            }
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

        tempoDecorrido = (int)(time(NULL) - tempoInicial);
        glutPostRedisplay();
        glutTimerFunc(16, atualizarJogo, 0);
    }
}
void pressionarTecla(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
    case 27: //ESC
        exit(0);
        break;
    case '1':
        modoContraComputador = true; 
        estadoAtual = JOGANDO;
        resetarBola();
        tempoInicial = time(NULL);
        tempoDecorrido = 0;
        glutTimerFunc(0, atualizarJogo, 0);
        break;
    case '2':
        modoContraComputador = false; 
        estadoAtual = JOGANDO;
        resetarBola();
        tempoInicial = time(NULL);
        tempoDecorrido = 0;
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
    case 8: //BACKSPACE
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

void teclaEspecialPressionada(int tecla, int x, int y) //NAO TEM NO ASCII TEMOS QUE USAR GLUT
{
    if (tecla == GLUT_KEY_UP) 
        setaCima = 1;
    if (tecla == GLUT_KEY_DOWN)
        setaBaixo = 1;
}

void teclaEspecialSolta(int tecla, int x, int y) //NAO FUNCIONA NO ASCII TEMOS QUE USAR GLUT
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
