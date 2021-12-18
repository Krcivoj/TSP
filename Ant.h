#include "City.h"
#include <vector>

using namespace std;

extern double alpha, beta, Q, rho;

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

    static void evaporatePheromone();
};