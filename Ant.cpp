#include "Ant.h"
#include "City.h"
#include <cmath>
#include<iostream>
#include <algorithm>

using namespace  std;

double alpha, beta_, Q, rho;

Ant::Ant(){}

Ant::Ant(City c){
    visited.push_back(c);
}

void Ant::next(){
    double sum = 0;
    for (auto city : cities){
        if (find(visited.begin(), visited.end(), city) == visited.end()) {
            int i, j;
            for (int k = 0; k < cities.size(); k++){
                if(cities[k] == visited.back())
                    i = k;
                if(cities[k] == city)
                    j = k;
            }
            if(i < j)swap(i,j);
            sum += pow(pheromone[i][j],alpha) * pow(1 / d(visited.back(),city), beta_);
        }
    }
    double p = ((double) rand() / (RAND_MAX));
    double p2 = p;
    p *= sum;
    double sum2 = 0;
    for (auto city : cities){
        if (find(visited.begin(), visited.end(), city) == visited.end()) {
            int i, j;
            for (int k = 0; k < cities.size(); k++){
                if(cities[k] == visited.back())
                    i = k;
                if(cities[k] == city)
                    j = k;
            }
            if(i < j)swap(i,j);
            sum2 += (pow(pheromone[i][j],alpha) * pow(1 / d(visited.back(),city), beta_));
            if(p - sum2 <= 0){
                visited.push_back(city);
                return;
            }
        }
    }
    throw runtime_error("Next node not selected");
}

void Ant::updatePheromone(){
    int n = visited.size();
    if(n < 2)return;
    int a,b;
    for(int i = 0; i < n; i++){
        double l = d(visited[i], visited[(i+1)%n]);
        for (int k = 0; k < cities.size(); k++){
            if(cities[k] == visited[i])
                a = k;
            if(cities[k] == visited[(i+1)%n])
                b = k;
        }
        if(a < b)swap(a,b);
        pheromone[a][b] += Q / l;
    }
}

double Ant::pathLength(){
    double l = 0;
    int n = visited.size();
    for(int i = 0; i < n; i++){
        l += d(visited[i], visited[(i+1)%n]);
    }
    return l;
}

void Ant::evaporatePheromone(){
    for(auto& row : pheromone){
        for(auto& element : row){
            element *= (1 - rho);
        }
    }
}

vector<City> Ant::cities = vector<City>();
vector< vector<double> > Ant::pheromone = vector< vector<double> >();
