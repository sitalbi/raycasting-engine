#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/glu.h>


float playerX, playerY; // player coordinates
float speed; // player speed

int mapX = 8, mapY = 8, mapSize;
int map[] =
{ 1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,1,1,
  1,0,0,0,1,1,1,1,
  1,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,1,
  1,0,0,0,1,0,0,1,
  1,0,0,0,1,0,0,1,
  1,1,1,1,1,1,1,1
};

void drawPlayer() {
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(playerX, playerY);
    glEnd();
}

void drawTileMap() {
    for (int j = 0; j < mapY; j++) {
        for (int i = 0; i < mapX; i++) {
            if (map[i + (j * mapY)]==1) { // choose the tile color depending on the map
                glColor3f(250, 250, 250); //white
            }
            else {
                glColor3f(0.3, 0.3, 0.3); //grey
            }
            // drawing the tile ("+-1" to have a grid gap
            glBegin(GL_POLYGON);
            glVertex2i(i * mapSize + 1, j * mapSize + 1);
            glVertex2i(i * mapSize + 1, (j * mapSize) + mapSize - 1);
            glVertex2i((i * mapSize) + mapSize - 1, (j * mapSize) + mapSize - 1);
            glVertex2i((i * mapSize) + mapSize - 1, j * mapSize + 1);
            glEnd();
        }
    }
}

void keyBoardCheck(unsigned char key, int x, int y) {
    if (key == 'z') playerY -= speed;
    if (key == 's') playerY += speed;
    if (key == 'q') playerX -= speed;
    if (key == 'd') playerX += speed;
    glutPostRedisplay();
}

void initialization() {
    glColor3f(0,0,0);
    gluOrtho2D(0, 1024, 512, 0);
    playerX = 100;
    playerY = 100;
    speed = 5;
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    drawTileMap();
    drawPlayer();
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    mapSize = mapX * mapY;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("raycasting-engine");

    initialization();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyBoardCheck);

    glutMainLoop();
}