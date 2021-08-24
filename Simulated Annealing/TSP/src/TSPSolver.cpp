

#include "../include/TSPSolver.h"

#include "SFML/Graphics.hpp"
#include "bits/stdc++.h"
#include "random"
#include "chrono"

#define TEMP_MIN 0.99

std::mt19937 getGen(){
    std::random_device rd;

    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );


    std::mt19937 generator(seed);
    return generator;
}

double getDistance(sf::Vector2f* first, sf::Vector2f* second){
    double dx = (second->x - first->x);
    double dy = (second->y - first->y);

    return dx * dx + dy * dy;
}


TSPSolver::TSPSolver(std::vector<sf::Vector2f*> points, int N,  double temperature, double alpha)
{
    this->points = points;
    this->N = N;

    this->generator = getGen();
    this->temperature = temperature;
    this->alpha = alpha;
}

std::vector<sf::Vector2f*> TSPSolver::swap(std::vector<sf::Vector2f*> points, int x, int y){

    std::vector<sf::Vector2f*> sample;

    for(int i = 0; i < this->N; i ++){
        if(i == x){
            sample.push_back(points[y]);
        }
        else if(i == y){
            sample.push_back(points[x]);
        }
        else{
            sample.push_back(points[i]);
        }
    }

    return sample;
}


std::vector<sf::Vector2f*> TSPSolver::inverse(std::vector<sf::Vector2f*> points, int x, int y){

    std::vector<sf::Vector2f*> sample;

    int min = std::min(x ,y);
    int max = std::max(x, y);

    for(int i = 0; i < this->N; i++){
        sample.push_back(points[i]);
    }

    for(int i = min ; i <= max; i++){
        sample[i] = points[max - (i - min)];
    }

    return sample;
}

std::vector<sf::Vector2f*> TSPSolver::insert(std::vector<sf::Vector2f*> points, int x, int y){

    std::vector<sf::Vector2f*> sample;

    int min = std::min(x, y);
    int max = std::max(x, y);

    for(int i = 0; i < min; i++){
        sample.push_back(points[i]);
    }

    sample.push_back(points[max]);

    for(int i = min; i < this->N; i++){
        if(i != max)
            sample.push_back(points[i]);
    }


    return sample;
}

std::vector<sf::Vector2f*> TSPSolver::getSample(std::vector<sf::Vector2f*> points){

    std::uniform_int_distribution<int> UniformDistribution(0, N - 1);
    int x = UniformDistribution(this->generator);
    int y = UniformDistribution(this->generator);

    std::vector<sf::Vector2f*> inverted = inverse(points, x, y);
    std::vector<sf::Vector2f*> swapped = inverse(points, x, y);
    std::vector<sf::Vector2f*> inserted = inverse(points, x, y);

    double invertedCost = getCost(inverted);
    double swappedCost = getCost(swapped);
    double insertedCost = getCost(inserted);

    double desired = std::min(std::min(invertedCost, swappedCost), insertedCost);

    if(desired == invertedCost)
        return inverted;
    else if(desired == insertedCost)
        return inserted;
    return swapped;

}


std::vector<sf::Vector2f*> TSPSolver::getNextIteration(){

    this->currentIter++;
    std::uniform_real_distribution<double> UniformDistribution(0, 1);

    std::vector<sf::Vector2f*> sample = getSample(points);

    double oldvalue = this->getCost(points);
    double newvalue = this->getCost(sample);

    double p = 1;
    // Oldvalue is worse than the new value
    if(oldvalue <= newvalue)
        p = exp(- (newvalue - oldvalue) / (this->temperature));

    double u = UniformDistribution(this->generator);

    if(u <= p){
        this->points = sample;
    }

    if(this->currentIter % (this->N + 1) == 0)
        this->temperature = std::max (TEMP_MIN, this->temperature * this->alpha);

    return this->points;
}

double TSPSolver::getCost(std::vector<sf::Vector2f*> points){
    double l = 0;
    for(int i = 0 ;i < this->N; i++){
        l += getDistance(points[i], points[(i + 1) % this->N]);
    }
    return l;
}

int TSPSolver::getIteration(){
    return this->currentIter;
}

void TSPSolver::setTemperature(double temp){
    this->temperature = temp;
}
