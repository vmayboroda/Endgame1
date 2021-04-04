#include <stdlib.h>
#include "arcanoid.h"
#include <math.h>

void moveBall(ball *b)
{
    vector2d *bonus = mulVector2d(*b->direction, b->speed);
    b->position = addVector2d(*b->position, *bonus);
}
void ricochet(ball *b, int wall)
{
    double x = 0;
    double y = 0;
    switch (wall)
    {
        case 0: // x = 0 
            b->direction->x *= -1;
            break;
        case 1: // y = x
            x = b->direction->x;
            y = b->direction->y;
            b->direction->y = -x;
            b->direction->x = -y;
            break;
        case 2: // y = 0
            b->direction->y *= -1;
            break;
        case 3: // y = -x
            x = b->direction->x;
            y = b->direction->y;
            b->direction->y = x;
            b->direction->x = y;
            break;
    }
}
void spawnBall(t_player *player)
{
    for (int i = 0; i < 3; i++)
    {
        if (!player->balls[i]->alive)
        {
            int dirX = rand() % 20 - 10;
            int dirY = -10;
            double length = sqrt(dirX * dirX + dirY * dirY);
            player->balls[i]->direction->x = (double)dirX/length;
            player->balls[i]->direction->y = (double)dirY/length;
            player->balls[i]->speed = .05;
            player->balls[i]->alive = true;
            player->balls[i]->position->x = player->platform->position->x;
            player->balls[i]->position->y = player->platform->position->y;
            player->balls[i]->stashedPosition->x = player->platform->position->x;
            player->balls[i]->stashedPosition->y = player->platform->position->y;
            player->balls[i]->position->y--;
            player->balls[i]->lastPlatformTouch = 0;
            break;
        }
    }
}
