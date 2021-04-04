#include <stdlib.h>
#include "arcanoid.h"

vector2i *addVector2i(vector2i v1, vector2i v2)
{
    vector2i *sum = malloc(sizeof(vector2i));
    sum->x = v1.x + v2.x;
    sum->y = v1.y + v2.y;
    return sum;
}
vector2i *subVector2i(vector2i v1, vector2i v2)
{
    vector2i *sub = malloc(sizeof(vector2i));
    sub->x = v1.x - v2.x;
    sub->y = v1.y - v2.y;
    return sub;
}
vector2i *mulVector2i(vector2i v1, int num)
{
    vector2i *mul = malloc(sizeof(vector2i));
    mul->x = v1.x * num;
    mul->y = v1.y * num;
    return mul;
}
vector2i *divVector2i(vector2i v1, int num)
{
    vector2i *div = malloc(sizeof(vector2i));
    div->x = v1.x / num;
    div->y = v1.y / num;
    return div;
}
