#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Creature 
{
public:
    int speed;
    Creature(int speed) : speed(speed) {}

    Creature reproduce() 
    {
        int childSpeed = speed + (rand() % 3 - 1);
        return Creature(childSpeed);
    }
};

void runEvolution(int initialPopulation, int generations, int survivalThreshold) 
{
    srand(time(0));

    std::vector<Creature> population;
    for (int i = 0; i < initialPopulation; i++) 
    {
        population.push_back(Creature(rand() % 10 + 1));
    }

    for (int generation = 0; generation < generations; generation++) 
    {
        std::vector<Creature> survivors;
        for (Creature &creature : population) 
        {
            if (creature.speed > survivalThreshold) 
            {
                survivors.push_back(creature);
            }
        }

        std::vector<Creature> newGeneration;
        for (Creature &survivor : survivors) 
        {
            newGeneration.push_back(survivor.reproduce());
        }

        population = newGeneration;

        if (!population.empty()) 
        {
            double averageSpeed = 0;
            for (Creature &creature : population) 
            {
                averageSpeed += creature.speed;
            }
            averageSpeed /= population.size();
            std::cout << "Generation " << generation + 1 << ": Avg Speed = " << averageSpeed
                      << ", Population = " << population.size() << std::endl;
        } else 
        {
            std::cout << "All creatures have perished by Generation " << generation + 1 << "." << std::endl;
            // No need for break; since the loop will end naturally
        }
    }
}

int main() 
{
    int initialPopulation = 100;
    int generations = 10;
    int survivalThreshold = 5;

    runEvolution(initialPopulation, generations, survivalThreshold);
    return 0;
}

