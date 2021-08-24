#include <iostream>
#include <random>
#include <chrono>
#include <math.h>

// This is the function we are trying to optimize. We assume. for now, a function where the concept of a "neighborhood" is easily defined.
float func(float x){
    if(x <= 0)
        return 0;

    return (cos(exp(x))) / (1 - exp(x));
}

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

float getSample(float x, int iterations, std::mt19937 generator){
    // Get a sample using the Metropolitan-Hastings algorithm
    std::normal_distribution<double> NormalDistribution;
    std::uniform_real_distribution<double> UniformDistribution;


    for(int i = 0; i < iterations; i++){
        float candidate = x + NormalDistribution(generator);
        float acceptance = 0.5;

        float u = UniformDistribution(generator);

        if(u <= acceptance){
            x = candidate;
        }
    }

    return x;
}

void simulatedAnnealing(int iterations, float T, float alpha){
    std::mt19937 generator = getGenerator();
    std::uniform_real_distribution<double> UniformDistribution(0, 1);

    float input = 1.0;

    for(int i = 0; i < iterations; i++){

        float sample = getSample(input, 1, generator);
        float oldvalue = func(input);
        float newvalue = func(sample);

        float p = 1;
        // Oldvalue is worse than the new value
        if(newvalue <= oldvalue)
            p = exp((newvalue - oldvalue) / (T));

        float u = UniformDistribution(generator);

        if(u <= p){
            input = sample ;
        }

        T *= alpha;
    }

    std::cout<<input<<" f:"<<func(input)<<"\n";
}


int main()
{
    simulatedAnnealing(1000000, 100, 0.95);
    return 0;
}
