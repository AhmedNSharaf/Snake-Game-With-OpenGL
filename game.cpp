#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#include <cstdio>
#include "game.h"
#pragma comment(lib, "winmm.lib")

int posX[60] = {20, 20, 20, 20, 20}, posY[60] = {20, 19, 18, 17, 16};
int snakLenth = 5;
int foodX, foodY;
int specialFoodX, specialFoodY;
int gridx, gridy;

int score = 0;
int highScore = 0;
bool food = true;
bool specialFood = false;
bool gameOver = false;
bool isHardMode = false;
short snakDirection = RIGHT;

void initGrid(int x, int y)
{
    gridx = x;
    gridy = y;
}

void drawCircle(double cx, double cy, double r, int num_segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++)
    {
        double theta = 2.0 * 3.1415926 * i / num_segments;
        double x = r * cos(theta);
        double y = r * sin(theta);
        glVertex2d(cx + x, cy + y);
    }
    glEnd();
}

void drawStar(float cx, float cy, float radius, int npoints)
{
    float angle = 2.0 * 3.1415926 / npoints;
    glBegin(GL_POLYGON);
    for (int i = 0; i < npoints; i++)
    {
        float x = cx + radius * cos(i * angle);
        float y = cy + radius * sin(i * angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawFood()
{
    if (food)
        random(foodX, foodY);
    food = false;

    glColor3f(1.0, 0.0, 0.0);
    drawCircle(foodX + 0.5, foodY + 0.5, 0.5, 100);

    glColor3f(0.4, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex2d(foodX + 0.45, foodY + 1.0);
    glVertex2d(foodX + 0.55, foodY + 1.0);
    glVertex2d(foodX + 0.55, foodY + 0.85);
    glVertex2d(foodX + 0.45, foodY + 0.85);
    glEnd();
}

void drawSpecialFood()
{
    if (!specialFood && score % 5 == 0 && score != 0)
    {
        random(specialFoodX, specialFoodY);
        specialFood = true;
    }
    if (specialFood)
    {
        glColor3f(1.0, 1.0, 0.0);
        drawStar(specialFoodX + 0.5, specialFoodY + 0.5, 0.6, 5);
    }
}

void drawSnake()
{
    for (int i = snakLenth - 1; i > 0; i--)
    {
        posX[i] = posX[i - 1];
        posY[i] = posY[i - 1];
    }

    if (snakDirection == UP) posY[0]++;
    else if (snakDirection == DOWN) posY[0]--;
    else if (snakDirection == RIGHT) posX[0]++;
    else if (snakDirection == LEFT) posX[0]--;

    for (int i = 0; i < snakLenth; i++)
    {
        glColor3f(i == 0 ? 0.0 : 0.0, i == 0 ? 1.0 : 0.8, i == 0 ? 0.0 : 0.8);
        glRectd(posX[i], posY[i], posX[i] + 1, posY[i] + 1);
    }

    if (posX[0] == 0 || posX[0] == gridx - 1 || posY[0] == 0 || posY[0] == gridy - 1)
        gameOver = true;

    for (int i = 1; i < snakLenth; i++)
        if (posX[0] == posX[i] && posY[0] == posY[i])
            gameOver = true;

    if (posX[0] == foodX && posY[0] == foodY)
    {
        score++;
        snakLenth = snakLenth < MAX ? snakLenth + 1 : MAX;
        food = true;
    }

    if (specialFood && posX[0] == specialFoodX && posY[0] == specialFoodY)
    {
        score += 5;
        snakLenth = snakLenth + 2 < MAX ? snakLenth + 2 : MAX;
        specialFood = false;
        PlaySound(TEXT("star.wav"), NULL, SND_ASYNC);
    }
}

void random(int &x, int &y)
{
    int _maxX = gridx - 2;
    int _maxY = gridy - 2;
    int _min = 1;

    x = _min + rand() % (_maxX - _min);
    y = _min + rand() % (_maxY - _min);
}

void loadHighScore()
{
    FILE *file = fopen("highscore.txt", "r");
    if (file)
    {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void saveHighScore()
{
    FILE *file = fopen("highscore.txt", "w");
    if (file)
    {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}
