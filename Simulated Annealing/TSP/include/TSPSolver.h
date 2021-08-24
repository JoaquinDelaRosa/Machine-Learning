#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include<SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <random>

class TSPSolver
{
    private:
        std::vector<sf::Vector2f*> points;
        int N;
        int currentIter = 0;

        double temperature;
        double alpha;

        std::mt19937 generator;


    public:
        TSPSolver(std::vector<sf::Vector2f*> points, int N, double temperature, double alpha = 0.9995);
        std::vector<sf::Vector2f*> getNextIteration();

        double getCost(std::vector<sf::Vector2f*> points);
        int getIteration();

        void setTemperature(double temp);

    private:
        std::vector<sf::Vector2f*> getSample(std::vector<sf::Vector2f*> points);

        std::vector<sf::Vector2f*> swap(std::vector<sf::Vector2f*> points, int x, int y);
        std::vector<sf::Vector2f*> inverse(std::vector<sf::Vector2f*> points, int x, int y);
        std::vector<sf::Vector2f*> insert(std::vector<sf::Vector2f*> points, int x, int y);

};

#endif // TSPSOLVER_H
