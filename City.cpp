#include "City.h"
#include <cmath>

using namespace  std;



City::City(double x, double y){
    this->x = x;
    this->y = y;
}

bool City::operator == (City c){
    return x == c.x && y == c.y;
}

double d(City c1, City c2){
    return sqrt(pow(c1.x - c2.x, 2) +
                pow(c1.y - c2.y, 2));
}