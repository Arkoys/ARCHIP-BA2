// Architecture b2
#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H
#include <iostream>
#include <cmath>
#include "constantes.h"

struct Point
{
    double x;
    double y;
};

struct Segment
{
    Point start;
    Point end;
};

struct Cercle
{
    Point centre;
    double rayon;
};

struct Vecteur
{
    double x;
    double y;
};


// Prototypes
double norme(Vecteur A);
double scalarProduct(Vecteur A, Vecteur B);
double distance(Point A, Point B);
bool overlapBetweenCircleSegment(Cercle cercle, Segment segment);


#endif
