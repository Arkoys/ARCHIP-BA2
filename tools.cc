// Architecture b2
#include <vector>
#include <cmath>
#include "tools.h"

using namespace std;


//link on the same uid // self link node // same thing
bool same_thing(unsigned int thing1, unsigned int thing2)
{
	if (thing1==thing2)
	{
		return true;
	}
	return false;
}

double distance(Point A, Point B)
{
	return (sqrt((A.y-B.y)*(A.y-B.y)+(A.x-B.x)*(A.x-B.x)));
}


double scalarProduct(Vecteur A, Vecteur B)
{
	return A.x*B.x+A.y+B.y;
}


double norme(Vecteur A)
{
	return sqrt((A.x)*(A.x)+(A.y)*(A.y));
}


bool overlapBetweenCircleSegment(Cercle cercle, Segment segment)
{
	Vecteur segmentToCircle = {cercle.centre.x - segment.start.x,
							  cercle.centre.y - segment.start.y};
	Vecteur segmentVector = {segment.end.x - segment.start.x,
							segment.end.y - segment.start.y};

	double segmentLength (distance(segment.start,segment.end));
	double distScal (scalarProduct(segmentVector, segmentToCircle)/segmentLength);

	if (distScal < 0 or distScal > segmentLength) return false;

	double distTest ( sqrt( pow(norme(segmentToCircle),2) - pow(distScal,2) ) );
	double distCercleTest (distTest - cercle.rayon);

	if (distCercleTest > 0) return false;

	return true;
};
