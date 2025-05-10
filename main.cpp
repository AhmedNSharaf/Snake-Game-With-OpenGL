#include <GL/gl.h>
#include <GL/glut.h>
#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <cstdio>
#include <ctime>

#define COLUMNS 40
#define ROWS 40
int FPS = 5;

void timer_callback(int);
void display_callback();
void reshape_callback(int, int);
void keyboard_callback(int, int, int);
void normal_keyboard_callback(unsigned char key, int x, int y);

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    srand(time(NULL));
    initGrid(COLUMNS, ROWS);
    loadHighScore();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(500, 150);
    glutCreateWindow("Snake");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutTimerFunc(0, timer_callback, 0);
    glutSpecialFunc(keyboard_callback);
    glutKeyboardFunc(normal_keyboard_callback);
    init();
    glutMainLoop();
    return 0;
}

void display_callback()
{
    if (gameState == MENU) {
        drawMenu();
        return;
    } else if (gameState == INSTRUCTIONS) {
        drawInstructions();
        return;
    } else if (gameState == PLAYING) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawSnake();
        drawFood();
        drawSpecialFood();

        float x = 1.0;
        float y = ROWS - 1;

        // Score
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(x, y);
        char scoreStr[50];
        sprintf(scoreStr, "Score: %d  ", score);
        for (char *c = scoreStr; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        x += strlen(scoreStr) * 0.6;

        // High Score
        glColor3f(1.0, 1.0, 0.0);
        glRasterPos2f(x, y);
        char highStr[50];
        sprintf(highStr, "High: %d  ", highScore);
        for (char *c = highStr; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        x += strlen(highStr) * 0.6;

        // Mode
        if (isHardMode)
            glColor3f(1.0, 0.0, 0.0);  // Red
        else
            glColor3f(0.0, 1.0, 0.0);  // Green

        glRasterPos2f(x, y);
        char modeStr[50];
        sprintf(modeStr, "Mode: %s  ", isHardMode ? "Hard" : "Easy");
        for (char *c = modeStr; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        x += strlen(modeStr) * 0.6;

        // Instruction note
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(x, y);
        char infoStr[] = "Click C To Change The Mode";
        for (char *c = infoStr; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

        glutSwapBuffers();
    }

    // Game over logic
    if (gameOver && gameState == PLAYING) {
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }

        char _score[10];
        itoa(score, _score, 10);
        char text[100] = "Your Score: ";
        strcat(text, _score);
        strcat(text, "\r\nHigh Score: ");
        char high[10];
        itoa(highScore, high, 10);
        strcat(text, high);
        strcat(text, "\r\nReplay?");
        int result = MessageBox(NULL, text, "GAME OVER", MB_YESNO);
        if (result == IDYES) {
            resetGame();
            gameState = PLAYING;
        } else {
            gameState = MENU;
            selectedMenuItem = 0;
            exit(0);
        }
    }
}

void reshape_callback(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int)
{
    if (gameState == PLAYING)
        glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer_callback, 0);
}

void keyboard_callback(int key, int, int)
{
    if (gameState == MENU) {
        switch (key) {
        case GLUT_KEY_UP:
            selectedMenuItem = (selectedMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
            break;
        case GLUT_KEY_DOWN:
            selectedMenuItem = (selectedMenuItem + 1) % NUM_MENU_ITEMS;
            break;
        }
        glutPostRedisplay();
    } else if (gameState == PLAYING) {
        switch (key) {
        case GLUT_KEY_UP:
            if (snakDirection != DOWN)
                snakDirection = UP;
            break;
        case GLUT_KEY_DOWN:
            if (snakDirection != UP)
                snakDirection = DOWN;
            break;
        case GLUT_KEY_RIGHT:
            if (snakDirection != LEFT)
                snakDirection = RIGHT;
            break;
        case GLUT_KEY_LEFT:
            if (snakDirection != RIGHT)
                snakDirection = LEFT;
            break;
        }
    }
}

void normal_keyboard_callback(unsigned char key, int x, int y)
{
    if (gameState == MENU || gameState == INSTRUCTIONS) {
        if (key == 13) { // Enter
            if (gameState == MENU) {
                switch (selectedMenuItem) {
                case 0:
                    isHardMode = false;
                    FPS = EASY_FPS;
                    resetGame();
                    gameState = PLAYING;
                    break;
                case 1:
                    isHardMode = true;
                    FPS = HARD_FPS;
                    resetGame();
                    gameState = PLAYING;
                    break;
                case 2:
                    gameState = INSTRUCTIONS;
                    break;
                case 3:
                    exit(0);
                    break;
                }
                glutPostRedisplay();
            } else if (gameState == INSTRUCTIONS) {
                gameState = MENU;
                glutPostRedisplay();
            }
        }
    } else if (gameState == PLAYING) {
        switch (key) {
        case 'c':
        case 'C':
            isHardMode = !isHardMode;
            FPS = isHardMode ? HARD_FPS : EASY_FPS;
            break;
        case 'r':
        case 'R':
            if (gameOver)
                resetGame();
            break;
        }
    }
}
