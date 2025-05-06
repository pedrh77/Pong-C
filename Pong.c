#include <GL/glut.h>
#include <string.h>


int windowWidth = 600;
int windowHeight = 600;

void RenderizarTextoCentralizado(float y, void *font, const char *str)
{
    int larguraTexto = 0;
    for (int i = 0; str[i]; i++)
        larguraTexto += glutBitmapWidth(font, str[i]);

    float x = -(larguraTexto / (float)windowWidth); // cálculo ajustado
    glRasterPos2f(x, y);

    for (int i = 0; str[i]; i++)
        glutBitmapCharacter(font, str[i]);
}

void Menu()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    RenderizarTextoCentralizado(0.6f, GLUT_BITMAP_TIMES_ROMAN_24, "PONG");
    RenderizarTextoCentralizado(0.2f, GLUT_BITMAP_TIMES_ROMAN_24, "1. Jogar contra Computador");
    RenderizarTextoCentralizado(0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "2. Jogar contra outro Jogador");
    RenderizarTextoCentralizado(-0.3f, GLUT_BITMAP_HELVETICA_18, "ESC. Sair");
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Ping Pong");

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); 

    glutReshapeFunc(reshape);
    glutDisplayFunc(Menu);
    glutMainLoop();

    return 0;
}
