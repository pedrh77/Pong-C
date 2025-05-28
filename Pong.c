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
time_t tempoInicial;
int tempoDecorrido = 0;
bool jogoPausado = false;
int limitePontos = 3;
float fundoR = 0.0f;
float fundoG = 0.0f;
float fundoB = 0.0f;

enum Dificuldade
{
    FACIL = 0,
    MEDIO = 1,
    DIFICIL = 2,
    SELECAODIF = 3
} dificuldadeAtual = SELECAODIF;

enum ModoJogo
{
    SELECAOMOD = 0,
    COMPUTADOR = 1,
    CONTRA = 2
} modoJogoAtual = SELECAOMOD;

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

    desenharTexto(-0.15f, 0.7f, GLUT_BITMAP_TIMES_ROMAN_24, "PING PONG");

    if (modoJogoAtual == SELECAOMOD)
    {
        desenharTexto(-0.45f, 0.3f, GLUT_BITMAP_HELVETICA_18, "ESCOLHA O MODO DE JOGO:");
        desenharTexto(-0.45f, 0.1f, GLUT_BITMAP_HELVETICA_18, "1 - CONTRA COMPUTADOR");
        desenharTexto(-0.45f, -0.1f, GLUT_BITMAP_HELVETICA_18, "2 - CONTRA OUTRO JOGADOR");
    }
    else if (modoJogoAtual == COMPUTADOR && dificuldadeAtual == SELECAODIF)
    {
       
        desenharTexto(-0.35f, 0.3f, GLUT_BITMAP_HELVETICA_18, "SELECIONE A DIFICULDADE:");
        desenharTexto(-0.3f, 0.1f, GLUT_BITMAP_HELVETICA_18, "F - FACIL");
        desenharTexto(-0.3f, -0.1f, GLUT_BITMAP_HELVETICA_18, "M - MEDIO");
        desenharTexto(-0.3f, -0.3f, GLUT_BITMAP_HELVETICA_18, "D - DIFICIL");
    }
    else
    {
       
        desenharTexto(-0.3f, 0.3f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE 'ESPACO' PARA INICIAR O JOGO");
    }

    desenharTexto(-0.4f, -0.7f, GLUT_BITMAP_HELVETICA_12, "PRESSIONE 'P' PARA PAUSAR DURANTE O JOGO");
    desenharTexto(-0.4f, -0.8f, GLUT_BITMAP_HELVETICA_12, "PRESSIONE 'BACKSPACE' PARA VOLTAR AO MENU");
    desenharTexto(-0.4f, -0.9f, GLUT_BITMAP_HELVETICA_12, "PRESSIONE 'ESC' PARA SAIR");

    pontosJogador1 = 0;
    pontosJogador2 = 0;

    glutSwapBuffers();
}

void desenharJogo()
{
    glLoadIdentity();
    glClearColor(fundoR, fundoG, fundoB, 1.0);
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

    char cronometro[50];
    sprintf(cronometro, "Tempo de Jogo: %d s", tempoDecorrido);
    desenharTexto(0.5f, 0.9f, GLUT_BITMAP_HELVETICA_18, cronometro);

    char pontuacao[50];
    sprintf(pontuacao, "Jogador 1: %d   Jogador 2: %d", pontosJogador1, pontosJogador2);
    desenharTexto(-0.4f, 0.9f, GLUT_BITMAP_HELVETICA_18, pontuacao);

    if (jogoPausado)
    {
        glColor3f(1.0, 0.0, 0.0);
        desenharTexto(-0.3f, 0.0f, GLUT_BITMAP_HELVETICA_18, "PAUSADO - Pressione 'P' para continuar");
    }

    glutSwapBuffers();
}
void desenharFIM()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0.0);

    char pontuacao[90];
    sprintf(pontuacao, "Jogador 1: %d x Jogador 2: %d", pontosJogador1, pontosJogador2);

    desenharTexto(-0.2f, 0.6f, GLUT_BITMAP_TIMES_ROMAN_24, "PONTUAÇÃO FINAL:");
    desenharTexto(-0.2f, 0.4f, GLUT_BITMAP_HELVETICA_18, pontuacao);

    if (pontosJogador1 > pontosJogador2)
    {
        desenharTexto(-0.2f, -0.0f, GLUT_BITMAP_HELVETICA_18, "Jogador 1 VENCEU!");
    }
    else if (pontosJogador2 > pontosJogador1)
    {
        desenharTexto(-0.2f, -0.0f, GLUT_BITMAP_HELVETICA_18, "Jogador 2 VENCEU!");
    }

    desenharTexto(-0.2f, -0.2f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE BACKSPACE PARA MENU");
    desenharTexto(-0.2f, -0.4f, GLUT_BITMAP_HELVETICA_18, "PRESSIONE ESC PARA SAIR");

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

void mudarCorFundo()
{
    fundoR = (float)rand() / RAND_MAX;
    fundoG = (float)rand() / RAND_MAX;
    fundoB = (float)rand() / RAND_MAX;
}


void mouse(int botao, int estado, int x, int y)
{
    if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN)
    {
        mudarCorFundo();
        glutPostRedisplay(); 
    }
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
            if (pontosJogador2 >= limitePontos)
            {
                estadoAtual = FIM;
                glutPostRedisplay();
                return;
            }
            resetarBola();
        }
        else if (bolaX > proporcaoTela)
        {
            pontosJogador1++;
            if (pontosJogador1 >= limitePontos)
            {
                estadoAtual = FIM;
                glutPostRedisplay();
                return;
            }
            resetarBola();
        }

        if (teclaW && raquete1Y < 0.8f)
            raquete1Y += 0.02f;
        if (teclaS && raquete1Y > -0.8f)
            raquete1Y -= 0.02f;

        if (modoJogoAtual == COMPUTADOR)
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
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualizarJogo, 0);
}

void pressionarTecla(unsigned char tecla, int x, int y) //unsigned começa de 0 a tecla.
{
    if (estadoAtual == MENU)
    {
        if (modoJogoAtual == SELECAOMOD) 
        {
            switch (tecla)
            {
            case '1':
                modoJogoAtual = COMPUTADOR;
                jogoPausado = false;
                resetarBola();
                tempoInicial = time(NULL);
                tempoDecorrido = 0;
                glutTimerFunc(0, atualizarJogo, 0);
                break;
            case '2':
                modoJogoAtual = CONTRA;
                jogoPausado = false;
                resetarBola();
                tempoInicial = time(NULL);
                tempoDecorrido = 0;
                glutTimerFunc(0, atualizarJogo, 0);
                break;
            case 27: // esc
                exit(0);
                break;
            }
        }
        else if (dificuldadeAtual == SELECAODIF) // escolher dificuldade
        {
            switch (tecla)
            {
            case 'f':
            case 'F':
                dificuldadeAtual = FACIL;
                estadoAtual = JOGANDO;
                jogoPausado = false;
                resetarBola();
                tempoInicial = time(NULL);
                tempoDecorrido = 0;
                glutTimerFunc(0, atualizarJogo, 0);
                break;
            case 'm':
            case 'M':
                dificuldadeAtual = MEDIO;
                estadoAtual = JOGANDO;
                jogoPausado = false;
                resetarBola();
                tempoInicial = time(NULL);
                tempoDecorrido = 0;
                glutTimerFunc(0, atualizarJogo, 0);
                break;
            case 'd':
            case 'D':
                dificuldadeAtual = DIFICIL;
                estadoAtual = JOGANDO;
                jogoPausado = false;
                resetarBola();
                tempoInicial = time(NULL);
                tempoDecorrido = 0;
                glutTimerFunc(0, atualizarJogo, 0);
                break;
            }
        }
        else
        {
            if (tecla == 32) // ESPAÇO
            {
                estadoAtual = JOGANDO;
                jogoPausado = false;
                resetarBola();
                tempoInicial = time(NULL);
                tempoDecorrido = 0;
                glutTimerFunc(0, atualizarJogo, 0);
                
            }
            else if (tecla == 8) // BACKSPACE
            {
                estadoAtual = MENU;
                jogoPausado = false;
                modoJogoAtual = SELECAOMOD;
                dificuldadeAtual = SELECAODIF;
            }
            else if (tecla == 27) // ESC
            {
                exit(0);
            }
        }
    }
    else if (estadoAtual == JOGANDO)
    {
        switch (tecla)
        {
        case 'p':
        case 'P':
            jogoPausado = !jogoPausado;
            break;
        case 'w':
        case 'W':
            teclaW = 1;
            break;
        case 's':
        case 'S':
            teclaS = 1;
            break;
        case 8: // BACKSPACE
            estadoAtual = MENU;
            jogoPausado = false;
            modoJogoAtual = SELECAOMOD;
            dificuldadeAtual = SELECAODIF;
            break;
        case 27: // ESC
            exit(0);
            break;
        }
    }
    if (tecla == 8) // BACKSPACE
    {
        estadoAtual = MENU;
        jogoPausado = false;
        modoJogoAtual = SELECAOMOD;
        dificuldadeAtual = SELECAODIF;
    }
    else if (tecla == 27) // ESC
    {
        exit(0);
    }
}

void soltarTecla(unsigned char tecla, int x, int y)
{
    if (tecla == 'w' || tecla == 'W')
        teclaW = 0;
    if (tecla == 's' || tecla == 'S')
        teclaS = 0;
}

void teclaEspecialPressionada(int tecla, int x, int y) // NAO TEM NO ASCII TEMOS QUE USAR GLUT
{
    if (tecla == GLUT_KEY_UP)
        setaCima = 1;
    if (tecla == GLUT_KEY_DOWN)
        setaBaixo = 1;
}

void teclaEspecialSolta(int tecla, int x, int y) // NAO FUNCIONA NO ASCII TEMOS QUE USAR GLUT
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
    srand(time(NULL)); //ex: a bola começar em lugares aleatorios

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Pong Ping");

    glutDisplayFunc(desenharTela);
    glutKeyboardFunc(pressionarTecla);
    glutKeyboardUpFunc(soltarTecla); //como fizemos raquete, precisava dos 2
    glutSpecialFunc(teclaEspecialPressionada); //seta-F1-F2-ENTER-BACKSPACE
    glutSpecialUpFunc(teclaEspecialSolta);
    glutReshapeFunc(redimensionarTela);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}