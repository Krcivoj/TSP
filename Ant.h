#include "City.h"
#include <vector>

using namespace std;

class Ant
{
    public:
    vector<City> visited;

    static vector<City> cities;
    static vector< vector<double> > pheromone;

    Ant();
    Ant(City);
    
    void next();
    void updatePheromone();
    double pathLength();

    static void evaporatePheromone(double);
};