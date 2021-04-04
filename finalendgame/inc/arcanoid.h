#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define standardSize 41
#define smallSize 21
#define bigSize 61

#define TIMEOUT 1

typedef struct vector2d
{
    double x;
    double y;
} vector2d;

typedef struct vector2i
{
    int x;
    int y;
} vector2i;

typedef struct block 
{
    vector2i *start;
    vector2i *end;
    clock_t lastHit;
    int health;
    int stashedHealth;
    int gift; // 0 - None, 1 - additional ball, 2 - empower, 3 - extra life, 4 - platformPlus, 5 - platformMinus
} block;

typedef struct ball
{
    vector2d *position;
    vector2i *stashedPosition;
    vector2d *direction;
    bool alive;
    double speed;
    double lastPlatformTouch;
} ball;

typedef struct cell
{
    vector2i *position;
} cell;

typedef struct display 
{
    int length;
    int start;
    int finish;
    wchar_t *arr;
} display;

typedef struct t_platform
{
    vector2i *stashedPosition;
    vector2d *position;
    int length;
    int effectId; // 0 - None, 1 - short platform, 2 - long platform
    int speed;
    clock_t buffTimer;
} t_platform;

typedef struct t_player
{
    int health;
    t_platform *platform;
    char *message;
    int points;
    int power;
    ball *balls[3];
    block *blocks[100];
    clock_t messageFade; 
} t_player;

vector2i *positionDoubleToInt(vector2d);
vector2d *positionIntToDouble(vector2i);
char *numberToString(int);

vector2d *addVector2d(vector2d, vector2d);
vector2d *subVector2d(vector2d, vector2d);
vector2d *mulVector2d(vector2d, double);
vector2d *divVector2d(vector2d, double);

vector2i *addVector2i(vector2i, vector2i);
vector2i *subVector2i(vector2i, vector2i);
vector2i *mulVector2i(vector2i, int);
vector2i *divVector2i(vector2i, int);

double distanceBetweenPoints(vector2d, vector2d);
double minimumDistance(vector2d, vector2d, vector2d);

void moveBall(ball*);
void ricochet(ball*, int);
void spawnBall(t_player*);
void checkForCollisionBlock(t_player*, ball*, block*);
int checkForCollisionBorder(t_player*, ball*, int, int);
void checkForPlatformCollision(t_player*, ball*);
wchar_t custom(int);



int menu(WINDOW*);
int info(WINDOW*);
void drawStart(WINDOW*, int, int, bool);
void drawInfo(WINDOW*, int, int, bool);
void drawQuit(WINDOW*, int, int, bool);
void outputInfo(WINDOW*, int, int);
void drawBlock(block*);
