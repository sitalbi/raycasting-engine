#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/glu.h>


float playerX, playerY; // player coordinates

void drawPlayer() {
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(playerX, playerY);
    glEnd();
}

void initialization() {
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 512, 0);
    playerX = 100;
    playerY = 100;
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    drawPlayer();
    glutSwapBuffers();
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("raycasting-engine");

    initialization();
    glutDisplayFunc(display);

    glutMainLoop();
}