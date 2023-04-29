#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
#define PI 3.141592653589793238


float playerX, playerY; // player coordinates
float playerDirX, playerDirY, playerAngle; //player rotation
float speed; // player speed

int mapX = 8, mapY = 8, mapSize;
int map[] =
{ 1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,1,1,
  1,0,0,0,1,1,1,1,
  1,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,1,
  1,0,1,0,1,0,0,1,
  1,0,0,0,1,0,0,1,
  1,1,1,1,1,1,1,1
};

void drawPlayer() {
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(playerX, playerY);
    glEnd();

    /*glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(playerX, playerY);
    glVertex2i(playerX+playerDirX, playerY+playerDirY*5);
    glEnd();*/
}

void drawRays() {
    //Implementation of the DDA algorithm
    

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
            // drawing the tile ("+-1" to have a grid gap)
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
    if (key == 'q') {
        playerAngle -= 0.1;
        if (playerAngle < 0) {
            playerAngle += 2 * PI;
        }
        playerDirX = cos(playerAngle) * 5;
        playerDirY = sin(playerAngle) * 5;
    }
    if (key == 'd') {
        playerAngle += 0.1;
        if (playerAngle > 2 * PI) {
            playerAngle = 0;
        }
        playerDirX = cos(playerAngle) * 5;
        playerDirY = sin(playerAngle) * 5;
    }
    if (key == 'z') {
        playerX += playerDirX;
        playerY += playerDirY;
    }
    if (key == 's') {
        playerX -= playerDirX;
        playerY -= playerDirY;
    }
    
    glutPostRedisplay();
}

void initialization() {
    glColor3f(0,0,0);
    gluOrtho2D(0, 1024, 512, 0);
    playerX = 100;
    playerY = 100;
    speed = 5;
    playerDirX = cos(playerAngle) * 5;
    playerDirY = sin(playerAngle) * 5;
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    drawTileMap();
    drawPlayer();
    drawRays();
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