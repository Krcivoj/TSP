#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Ant.h"
#include "City.h"

using namespace std;

double nearestNeighbours(vector<City> cities)
{
    City c1 = cities.back();
    City c = c1;
    cities.pop_back();
    int n = cities.size();
    double l = 0;
    double min;
    int k;
    for(int i = 0; i < n; i++)
    {
        min = __DBL_MAX__;
        for(int j = 0; j < cities.size(); j++)
        {
            if(d(c,cities[j]) < min)
            {
                min = d(c,cities[j]);
                k = j;
            }
        }
        l += min;
        c = cities[k];
        cities.erase(cities.begin() + k);
    }
    l += d(c,c1);
    return l;
}

int main() {
    srand (time(NULL));
    vector<Ant> ants;
    vector<City> bestPath;
    double bestLength = INT_MAX;
    
    string fileName = "../Oliver30.csv";
    //string fileName = "../4x4grid.csv";
    int max_iterations = 2500;
    alpha = 1;
    beta = 5;
    Q = 100;
    rho = 0.5;

    ifstream infile(fileName);   
    string line;
    while(getline(infile, line)) 
    {
        stringstream linestream(line);
        int val1;
        int val2;
        char tab;         
        linestream >> val1>> tab >> val2;
        Ant::cities.push_back( City(val1, val2));
        //cout << val1 << "," << val2 << "\n";
    }
    int m = Ant::cities.size();
    double C = nearestNeighbours(Ant::cities);
    for(int i = 0; i < m; i++)
    {
        vector<double> row;
        for(int j = 0; j < i; j++)
        {
            row.push_back(m/C);
            //row.push_back(8);
        }
        Ant::pheromone.push_back(row);
    }
    cout<<Ant::pheromone[1][0]<<endl;
    for( int i = 0; i < max_iterations ; i++)
    {
        ants.clear();
        for (auto city : Ant::cities)
            ants.push_back(Ant(city));
        for( int j = 0; j < Ant::cities.size(); j++)
        {
            for( auto& ant : ants)
            {
                ant.next();
            }
        }
        for( auto& ant : ants)
        {
            ant.updatePheromone();
            if(ant.pathLength() < bestLength)
            {
                bestLength = ant.pathLength();
                bestPath = ant.visited;
            }
        }
        Ant::evaporatePheromone();
        if(i % 100 == 0){
            cout<<"============================="<< i<< "=================="<<endl;
            cout<<Ant::pheromone[1][0]<<endl;
            cout<<Ant::pheromone[15][0]<<endl;
            cout<<bestLength<<endl;
        }
    }
    cout<<"============================="<< max_iterations<< "=================="<<endl;
    cout<<Ant::pheromone[1][0]<<endl;
    cout<<Ant::pheromone[15][0]<<endl;
    cout<<bestLength<<endl;
    for(auto city : bestPath)
    {
        cout<<'('<<city.x<<", "<< city.y <<")->";
    }
    return 0;
}