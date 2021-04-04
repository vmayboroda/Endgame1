#include <stdlib.h>
#include "arcanoid.h"
#include <math.h>

void hit(t_player *player, block *bl)
{
    bl->lastHit = 400;
    bl->health -= player->power;
    player->points += 100 * (bl->health < 0 ? player->power + bl->health : player->power);
    if (bl->health <= 0)
    {
        player->messageFade = clock();
        bl->health = 0;
        switch (bl->gift)
        {
            case 1:
                player->message = "You've received an additional ball.";
                spawnBall(player);
                break;
            case 2:
                player->message = "You've received a ball power up.";
                for (int i = 0; i < 3; i++)
                {
                    player->power++; 
                }
                break;
            case 3:
                player->message = "You've received a health point.";
                player->health++;
            case 4:
                player->message = "You've received a platform size bonus.";
                player->platform->effectId = 2;
                player->platform->length = bigSize;
                player->platform->buffTimer = 30;
                break;
            case 5:
                player->message = "You've received a platform size debuff.";
                player->platform->effectId = 1;
                player->platform->length = smallSize;
                player->platform->buffTimer = 30;
                break;
            default:
                break;
        }
    }
}


void drawBlock(block *bl)
{
    for (int x = bl->start->x + 1; x < bl->end->x; x += 2)
    {
        for (int y = bl->start->y; y <= bl->end->y; y++)
        {
            char *collection[] = { "\U0001F578", "\U0001F578", "\U0001F578", "\U0001F578", "\U0001F578" };
            attron(COLOR_PAIR(bl->health + 1));
            mvprintw(y, x, bl->health > 0 ? collection[bl->health - 1] : " ");
            attroff(COLOR_PAIR(bl->health + 1));
        }
    }
    for (int y = bl->start->y; y <= bl->end->y; y++)
    {
        attron(COLOR_PAIR(bl->health + 1));
        mvprintw(y, bl->start->x, "{");
        mvprintw(y, bl->end->x, "}");
        attroff(COLOR_PAIR(bl->health + 1));
    }
}
void checkForCollisionBlock(t_player *player, ball *b, block *bl)
{
    if (((int) b->position->y == (int) bl->start->y || 
         (int) b->position->y == (int) bl->end->y) && 
                b->position->x >= bl->start->x && 
                b->position->x <= bl->end->x)
    {
        ricochet(b, ((int) b->position->x == bl->start->x &&
                        (int) b->position->y == bl->start->y) || 
                    ((int) b->position->x == bl->end->x && 
                        (int) b->position->y == bl->end->y) ? 1 : 
                    ((int) b->position->x == bl->end->x &&
                        (int) b->position->y == bl->start->y) || 
                    ((int) b->position->x == bl->start->x && 
                        (int) b->position->y == bl->end->y) ? 3 : 
                        2);
        hit(player, bl);
        drawBlock(bl);
    }
    else if (((int) b->position->x == (int) bl->end->x ||
              (int) b->position->x == (int) bl->start->x) && 
              (int) b->position->y > bl->start->y && 
              (int) b->position->y < bl->end->y)
    {
        ricochet(b, 0);
        hit(player, bl);
        drawBlock(bl);
    }
}
int checkForCollisionBorder(t_player *player, ball *b, int maxX, int maxY)
{
    if ((b->position->x < 3 && b->direction->x < 0) ||
        (b->position->x > maxX - 5 && b->direction->x > 0))
        ricochet(b, 0);
    else if (b->position->y < 3 && b->direction->y < 0)
        ricochet(b, 2);
    else if ((int)b->position->y == maxY - 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (player->balls[i] == b)
            {
                player->balls[i]->alive = false;
                player->power = 1;
                for (int j = 0; j < 3; j++)
                {
                    if (player->balls[j]->alive) return 0;
                }
                player->health--;
                if (player->health == 0)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        player->balls[j]->alive = false;
                    }
                    return 1;
                }
                spawnBall(player);
            }
        }
    }
    return 0;
}
void checkForPlatformCollision(t_player *player, ball *b)
{
    if (b->lastPlatformTouch) return;

    if ((int)b->position->y == (int)player->platform->position->y && 
        fabs(b->position->x - player->platform->position->x) <= player->platform->length / 2 + 1)
    {
        double startDegree = atan(b->direction->y/b->direction->x);
        double hitDegrees = 90 * (b->position->x - player->platform->position->x) / player->platform->length - 45;
        double endDegree = 2 * hitDegrees - startDegree;

        double endDirX = cos(endDegree / 180 * M_PI);
        double endDirY = sin(endDegree / 180 * M_PI);
        b->direction->x = endDirX;
        b->direction->y = endDirY;
        b->lastPlatformTouch = true;
    }
}
