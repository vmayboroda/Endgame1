#include <stdlib.h>
#include "arcanoid.h"

vector2d *addVector2d(vector2d v1, vector2d v2)
{
    vector2d *sum = malloc(sizeof(vector2d));
    sum->x = v1.x + v2.x;
    sum->y = v1.y + v2.y;
    return sum;
}
vector2d *subVector2d(vector2d v1, vector2d v2)
{
    vector2d *sub = malloc(sizeof(vector2d));
    sub->x = v1.x - v2.x;
    sub->y = v1.y - v2.y;
    return sub;
}
vector2d *mulVector2d(vector2d v1, double num)
{
    vector2d *mul = malloc(sizeof(vector2d));
    mul->x = v1.x * num;
    mul->y = v1.y * num;
    return mul;
}
vector2d *divVector2d(vector2d v1, double num)
{
    vector2d *div = malloc(sizeof(vector2d));
    div->x = v1.x / num;
    div->y = v1.y / num;
    return div;
}
