#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

#define MAX 60

void initGrid(int, int);
void drawGrid();
void unit(int, int);
void drawSnack();
void drawFood();
void drawSpecialFood(); // ✅ دالة رسم الطعام الخاص (النجمة)
void random(int&, int&);

#endif // GAME_H_INCLUDED
