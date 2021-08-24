
// A program to generate an approximate solution to the Traveling Salesman Problem using Simulated Annealing

// The neighborhood function for SA looks at a case where for two vertices indexed x and y (i) the path from x to y is inverted (ii) x and y swap places
// or (iii) y is inserted after x

#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <bits/stdc++.h>

#include "include/TSPSolver.h"

#define WIDTH 1000
#define HEIGHT 1000
#define POINTS 1000

std::mt19937 getGenerator(){
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


std::vector<sf::Vector2f*>* generatePoints(int n){
    std::vector<sf::Vector2f* >* points = new std::vector<sf::Vector2f*>();

    std::mt19937 generator = getGenerator();
    std::uniform_int_distribution<int> UWidth(0, WIDTH);
    std::uniform_int_distribution<int> UHeight(0, WIDTH);


    for(int i = 0; i < n; i++){
        points->push_back(new sf::Vector2f(UWidth(generator), UHeight(generator)));
    }

    return points;
}


std::vector<sf::CircleShape*> getCircles(std::vector<sf::Vector2f*> *points){
    std::vector<sf::CircleShape*> circles;
    for(int i = 0 ; i < POINTS; i++){
        circles.push_back(new sf::CircleShape(3));
        circles[i]->setPosition((*points)[i]->x, (*points)[i]->y);
        circles[i]->setFillColor(sf::Color::Red);
    }

    return circles;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Travelling Salesman with SA");
    std::vector<sf::Vector2f*>* points = generatePoints(POINTS);
    std::vector<sf::CircleShape*> circles = getCircles(points);
    TSPSolver* solver= new TSPSolver(*points, POINTS, 1000, 0.98);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        *points = solver->getNextIteration();

        if(solver->getIteration() % 1000 != 0)
            continue;

        window.clear();

        circles = getCircles(points);
        for(int i = 0; i < POINTS; i++){
            // Draw the path
            sf::Vertex line[] = {
                sf::Vertex(*(*points)[i]),
                sf::Vertex(*(*points)[( i + 1 ) % POINTS])
            };
            window.draw(*circles[i]);
            window.draw(line, 2, sf::Lines);
        }
        window.display();

        if(solver->getIteration() % 10000 == 0)
            std::cout<<"Iteration "<<solver->getIteration()<<" Cost: "<<sqrt(solver->getCost(*points))<<"\n";

    }

    return 0;
}
