#include <math.h>
#include "arcanoid.h"

double distanceBetweenPoints(vector2d p1, vector2d p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double minimumDistance(vector2d point, vector2d corner1, vector2d corner2)
{
    double A = point.x - corner1.x;
    double B = point.y - corner1.y;
    double C = corner2.x - corner1.x;
    double D = corner2.y - corner1.y;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    int param = -1;

    if (len_sq != 0) 
        param = dot / len_sq;

    double xx, yy;

    if (param < 0) 
    {
        xx = corner1.x;
        yy = corner1.y;
    }
    else if (param > 1) 
    {
        xx = corner2.x;
        yy = corner2.y;
    }
    else 
    {
        xx = corner1.x + param * C;
        yy = corner1.y + param * D;
    }

    double dx = point.x - xx;
    double dy = point.y - yy;
    return sqrt(dx * dx + dy * dy);
}
