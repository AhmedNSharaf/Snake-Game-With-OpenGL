#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

#define MAX 60

#define EASY_FPS 5
#define HARD_FPS 10

void initGrid(int, int);
void drawSnake();
void drawFood();
void drawSpecialFood();
void random(int &, int &);
void loadHighScore();
void saveHighScore();

extern int score;
extern int FPS;
extern int highScore;
extern bool gameOver;
extern short snakDirection;
extern bool isHardMode;

extern int snakLenth;
extern int posX[60];
extern int posY[60];
extern bool food;
extern bool specialFood;
extern int foodX;
extern int foodY;
extern int specialFoodX;
extern int specialFoodY;
extern int gridx;
extern int gridy;

#endif // GAME_H_INCLUDED
