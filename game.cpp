#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include "game.h"
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

extern int score;
extern int FPS;

int gridx, gridy;
int snakLenth = 5;
bool food = true;
bool specialFood = false;
int foodX, foodY;
int specialFoodX, specialFoodY;
short snakDirection = RIGHT;
extern bool gameOver;

int posX[60] = {20, 20, 20, 20, 20}, posY[60] = {20, 19, 18, 17, 16};

void initGrid(int x, int y)
{
    gridx = x;
    gridy = y;
}

void drawGrid()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // اللون الرمادي
    glClear(GL_COLOR_BUFFER_BIT);
}

void unit(int x, int y)
{
    if (x == 0 || y == 0 || x == gridx - 1 || y == gridy - 1)
    {
        glLineWidth(0.0);
        glColor3f(0.0, 1.0, 0.0);

        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + 1, y);
        glVertex2f(x + 1, y + 1);
        glVertex2f(x, y + 1);
        glEnd();
    }
}

void drawCircle(double cx, double cy, double r, int num_segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++)
    {
        double theta = 2.0 * 3.1415926 * double(i) / double(num_segments);
        double x = r * cos(theta);
        double y = r * sin(theta);
        glVertex2d(cx + x, cy + y);
    }
    glEnd();
}

void drawStar(float cx, float cy, float radius, int npoints)
{
    float angle = 2.0 * 3.1415926 / npoints;
    glBegin(GL_POLYGON); // رسم شكل النجمة باستخدام خطوط متعددة
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
    drawCircle(foodX + 0.5, foodY + 0.5, 0.5, 100); // دائرة حمراء للطعام

    glColor3f(0.4, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex2d(foodX + 0.45, foodY + 1.0);   // ساق التفاحة
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
        glColor3f(1.0, 1.0, 0.0); // اللون الأصفر
        drawStar(specialFoodX + 0.5, specialFoodY + 0.5, 0.6, 5); // رسم النجمة
    }
}

void drawSnack()
{
    for (int i = snakLenth - 1; i > 0; i--)
    {
        posX[i] = posX[i - 1];
        posY[i] = posY[i - 1];
    }
    if (snakDirection == UP)
        posY[0]++;
    else if (snakDirection == DOWN)
        posY[0]--;
    else if (snakDirection == RIGHT)
        posX[0]++;
    else if (snakDirection == LEFT)
        posX[0]--;

    for (int i = 0; i < snakLenth; i++)
    {
        if (i == 0)
            glColor3f(0.0, 1.0, 0.0); // رأس الثعبان أخضر
        else
            glColor3f(0.0, 0.0, 1.0); // جسم الثعبان أزرق
        glRectd(posX[i], posY[i], posX[i] + 1, posY[i] + 1);
    }

    if (posX[0] == 0 || posX[0] == gridx - 1 || posY[0] == 0 || posY[0] == gridy - 1)
        gameOver = true;

    if (posX[0] == foodX && posY[0] == foodY)
    {
        score++;
        if (score % 3 == 0)
            FPS++;
        snakLenth++;
        if (snakLenth > MAX)
            snakLenth = MAX;
        food = true;
    }

    if (specialFood && posX[0] == specialFoodX && posY[0] == specialFoodY)
    {
        score += 5;
        snakLenth += 2;
        if (score % 3 == 0)
            FPS++;
        specialFood = false;
    }
}

void random(int &x, int &y)
{
    int _maxX = gridx - 2;
    int _maxY = gridy - 2;
    int _min = 1;

    srand(time(NULL));
    x = _min + rand() % (_maxX - _min);
    y = _min + rand() % (_maxY - _min);
}
