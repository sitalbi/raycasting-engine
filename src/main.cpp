#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
#define PI 3.141592653589793238
#define mapWidth 10
#define mapHeight 10
#define screenWidth 1280
#define screenHeight 720
#define show2DView false
#define heightCheckMultiplier 0.8


float playerX, playerY; // player coordinates
float playerDirX, playerDirY, playerAngle; //player rotation
float speed; // player speed

int mapSize = mapWidth * mapHeight;
int map[] =
{ 1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,1,0,0,1,1,1,
  1,0,0,0,0,0,0,1,1,1,
  1,0,0,1,0,0,0,0,1,1,
  1,0,0,1,0,0,1,0,0,1,
  1,0,0,1,0,0,0,1,0,1,
  1,1,1,1,1,1,1,1,1,1
};

void draw2DPlayer() {
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(playerX * mapSize, playerY * mapSize);
    glEnd();
}

/**
*  Implementation of the DDA algorithm to calculate and draw raycasts.
*  With the help of: https://lodev.org/cgtutor/raycasting.html
*/
void drawRays() {
    int rayNumber = 120;
    for (int i = -60; i < 60; i++) {
        float rayAngle = playerAngle + (float)i / 120;
        if (rayAngle < 0) {
            playerAngle += 2 * PI;
        }
        if (playerAngle > 2 * PI) {
            playerAngle -= 2 * PI;
        }

        float rayDirX = cos(rayAngle);
        float rayDirY = -sin(rayAngle);

        float rayX = playerX;
        float rayY = playerY;

        //ray map coordinates at each step
        int mapX = rayX;
        int mapY = rayY;

        //length of ray from current position to next x or y-side
        float sideDistX;
        float sideDistY;

        //length of ray from one x or y-side to next x or y-side
        float deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        float deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        bool hit = false;
        int side; //horizontal or vertical wall hit?

        //initialize step and calculate initial sideDists
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (rayX - (float)mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = ((float)mapX + 1.0 - rayX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (rayY - (float)mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = ((float)mapY + 1.0 - rayY) * deltaDistY;
        }

        //perform DDA
        while (!hit) {
            //jump to next map square, either in x-direction, or in y-direction
            if (sideDistX < sideDistY)
            {
                mapX += stepX;
                sideDistX += deltaDistX;
                side = 0;
            }
            else
            {
                mapY += stepY;
                sideDistY += deltaDistY;
                side = 1;
            }

            //Check if the ray is inside the map
            if (mapX >= 0 && mapX < 8 && mapY >= 0 && mapY < 8) {
                if (map[mapX + (mapY * mapHeight)] == 1) {  //Check if the ray has hit a wall
                    hit = true;
                }
            }
            else {
                hit = true;
            }
        }

        // Calculate the distance between the rayStarting point and the hit point
        float fDistance = (side == 0) ? (mapX - rayX + (1 - stepX) / 2) / rayDirX
            : (mapY - rayY + (1 - stepY) / 2) / rayDirY;
        fDistance *= mapSize;

        if (show2DView) {
            glLineWidth(1);
            glColor3f(1, 0, 0);
            glBegin(GL_LINES);
            glVertex2i(rayX * mapSize, rayY * mapSize);
            glVertex2i(rayX * mapSize + fDistance * rayDirX, rayY * mapSize + fDistance * rayDirY);
            glEnd();
        }

        // Draw 3D walls ===========================================================================
        //
        float startingPoint = 10;
        if (show2DView) {
            startingPoint = (screenWidth / 2) + 10;
        }

        // Fisheye effect correction
        float correctedAngle = playerAngle - rayAngle;
        if (correctedAngle < 0) correctedAngle += 2 * PI;
        if (correctedAngle > 2 * PI) correctedAngle -= 2 * PI;
        fDistance = fDistance * cos(correctedAngle);

        float heightCheck = screenHeight*heightCheckMultiplier;
        int lineWidth = sqrt(mapSize);
        if (show2DView) {
            heightCheck = screenHeight / 2;
            lineWidth = sqrt(mapSize);
        }

        float lineHeight = (mapSize * heightCheck) / fDistance;
        if (lineHeight > heightCheck) lineHeight = heightCheck;
        float lineOffset = (screenHeight / 2) - lineHeight / 2;

        // Color to make depth more apparent
        if (side == 0) {
            glColor3f(1, 0, 0);
        }
        else if (side == 1) {
            glColor3f(0.8, 0, 0);
        }

        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glVertex2i(rayNumber * lineWidth + startingPoint, lineOffset);
        glVertex2i(rayNumber * lineWidth + startingPoint, lineHeight + lineOffset);
        glEnd();

        rayNumber--;
    }
}


void drawTileMap() {
    for (int j = 0; j < mapHeight; j++) {
        for (int i = 0; i < mapWidth; i++) {
            if (map[i + (j * mapHeight)] == 1) { // choose the tile color depending on the map
                glColor3f(1, 1, 1); //white
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
        playerAngle += 0.1;
        if (playerAngle < 0) {
            playerAngle += 2 * PI;
        }
        playerDirX = cos(playerAngle);
        playerDirY = -sin(playerAngle);
    }
    if (key == 'd') {
        playerAngle -= 0.1;
        if (playerAngle > 2 * PI) {
            playerAngle -= 2 * PI;
        }
        playerDirX = cos(playerAngle);
        playerDirY = -sin(playerAngle);
    }
    if (key == 'z') {
        playerX += speed * playerDirX / mapSize;
        playerY += speed * playerDirY / mapSize;
    }
    if (key == 's') {
        playerX -= speed * playerDirX / mapSize;
        playerY -= speed * playerDirY / mapSize;
    }

    glutPostRedisplay();
}

void initialization() {
    glColor3f(0, 0, 0);
    gluOrtho2D(0, screenWidth, screenHeight, 0);
    playerX = 2.2;
    playerY = 2.2;
    playerAngle = PI / 3;
    speed = 5;
    playerDirX = cos(playerAngle);
    playerDirY = -sin(playerAngle);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (show2DView) {
        drawTileMap();
        drawRays();
        draw2DPlayer();
    }
    else {
        drawRays();
    }
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("raycasting-engine");

    initialization();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyBoardCheck);

    glutMainLoop();
    return 0;
}
