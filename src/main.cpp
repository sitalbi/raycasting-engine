#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
#define PI 3.141592653589793238
#define mapWidth 8
#define mapHeight 8
#define screenWidth 1024
#define screenHeight 512


float playerX, playerY; // player coordinates
float playerDirX, playerDirY, playerAngle; //player rotation
float speed; // player speed

int mapSize = mapWidth * mapHeight;
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
    glVertex2i(playerX * mapSize, playerY * mapSize);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(playerX* mapSize, playerY* mapSize);
    glVertex2i(playerX * mapSize + playerDirX*5, playerY* mapSize +playerDirY*5);
    glEnd();
}

void drawRays() {
    //Implementation of the DDA algorithm
    
    float rayDirX = playerDirX;
    float rayDirY = playerDirY;

    float rayX = (playerX);
    float rayY = (playerY);

    //which box of the map we're in
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

    bool hit = false; //was there a wall hit?
    int side; //was a NS or a EW wall hit?
        
    //calculate step and initial sideDist
    if (rayDirX < 0)
    {
        stepX = -1;
        sideDistX = (rayX - (float)mapX) * deltaDistX;
    }
    else
    {
        stepX = 1;
        sideDistX = ((float)mapX - rayX) * deltaDistX;
    }
    if (rayDirY < 0)
    {
        stepY = -1;
        sideDistY = (rayY - (float)mapY) * deltaDistY;
    }
    else
    {
        stepY = 1;
        sideDistY = ((float)mapY - rayY) * deltaDistY;
    }

    float fDistance = 0;
    //perform DDA
    do {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY)
        {
            mapX += stepX;
            fDistance += sideDistX;
            sideDistX += deltaDistX;
            side = 0;
        }
        else
        {
            mapY += stepY;
            fDistance += sideDistY;
            sideDistY += deltaDistY;
            side = 1;
        }
        
        //Check if ray has hit a wall
        if (mapX >= 0 && mapX < mapSize && mapY >= 0 && mapY < mapSize) {
            if (map[mapX + (mapY * mapHeight)] == 1) {
                hit = true;
            }
        }
        /*else {
            glBegin(GL_POLYGON);
            glVertex2i((int)(mapX / 64) * mapSize + 1, (int)(mapY / 64) * mapSize + 1);
            glVertex2i((int)(mapX / 64) * mapSize + 1, ((int)(mapY / 64) * mapSize) + mapSize - 1);
            glVertex2i(((int)(mapX / 64) * mapSize) + mapSize - 1, ((int)(mapY / 64) * mapSize) + mapSize - 1);
            glVertex2i(((int)(mapX / 64) * mapSize) + mapSize - 1, (int)(mapY / 64) * mapSize + 1);
            glEnd();
        }*/
    } while (!hit);

    //float sideDist = side == 1 ? sideDistX : sideDistY;
    
    //float wallDistance = sqrt(pow(mapX - playerX,2)+ pow(mapY - playerY, 2));

    glLineWidth(3);
    glColor3f(250, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(rayX*mapSize, rayY*mapSize);
    glVertex2f((rayX*mapSize)+rayDirX*fDistance, (rayY*mapSize+rayDirY*fDistance));
    glEnd();

}

void drawTileMap() {
    for (int j = 0; j < mapHeight; j++) {
        for (int i = 0; i < mapWidth; i++) {
            if (map[i + (j * mapHeight)]==1) { // choose the tile color depending on the map
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
        playerDirX = cos(playerAngle)*5;
        playerDirY = sin(playerAngle)*5;
    }
    if (key == 'd') {
        playerAngle += 0.1;
        if (playerAngle > 2 * PI) {
            playerAngle = 0;
        }
        playerDirX = cos(playerAngle)*5;
        playerDirY = sin(playerAngle)*5;
    }
    if (key == 'z') {
        playerX += playerDirX/mapSize;
        playerY += playerDirY/mapSize;
    }
    if (key == 's') {
        playerX -= playerDirX/mapSize;
        playerY -= playerDirY/mapSize;
    }
    
    glutPostRedisplay();
}

void initialization() {
    glColor3f(0,0,0);
    gluOrtho2D(0, screenWidth, screenHeight, 0);
    playerX = 2;
    playerY = 2;
    speed = 3;
    playerDirX = cos(playerAngle)*5;
    playerDirY = sin(playerAngle)*5;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawTileMap();
    drawPlayer();
    //drawRays();
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