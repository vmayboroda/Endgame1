#include <stdlib.h>
#include "arcanoid.h"

vector2i *positionDoubleToInt(vector2d coordinates)
{
    vector2i *precise = malloc(sizeof(vector2i));
    precise->x = (int) coordinates.x;
    precise->y = (int) coordinates.y;
    return precise;
}
vector2d *positionIntToDouble(vector2i coordinates)
{
    vector2d *precise = malloc(sizeof(vector2d));
    precise->x = (double) coordinates.x;
    precise->y = (double) coordinates.y;
    return precise;
}
int mx_pow(int a, int b)
{
    int result = 1;
    for (int i = 0; i < b; i++)
        result *= a;
    return result;
}

char *numberToString(int n)
{
    int a = n;
    int count = 0;
    while (a > 0)
    {
        a /= 10;
        count++;
    }
    if (count == 0) return "0";
    char *result = malloc(count);
    a = n;

    for (int i = 0; i < count; i++)
    {
        int temp = mx_pow(10, count - i - 1);
        result[i] = (n / temp) % 10 + 48;
    }
    return result;
}
