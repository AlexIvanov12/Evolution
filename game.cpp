#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp> // Updated to include all necessary SFML headers

enum GeneType 
{
    ADAPTABILITY,
    SPEED,
    REPRODUCTION,
    DIET // Herbivorous or carnivorous
};

class Environment 
{
public:
    enum Season { SPRING, SUMMER, FALL, WINTER };
    enum ClimateChange { NONE, ICE_AGE, HEATWAVE };
    Season currentSeason;
    ClimateChange currentClimate;
    int epochCounter;
    int climateChangeDuration;

    Environment() : currentSeason(SPRING), currentClimate(NONE), epochCounter(0), climateChangeDuration(0) {}

    void update() 
    {
        if (climateChangeDuration > 0) 
        {
            climateChangeDuration--;
            if (climateChangeDuration == 0) 
            {
                currentClimate = NONE; // Reset climate change after its duration
            }
        } else 
        {
            // Check for random climate change every 20 epochs
            if (epochCounter % 20 == 0 && rand() % 2 == 0) 
            { // 50% chance for climate change every 20 epochs
                currentClimate = (rand() % 2 == 0) ? ICE_AGE : HEATWAVE;
                climateChangeDuration = 5; // Lasts for 5 epochs
            } else 
            {
                // Cycle through the seasons
                currentSeason = static_cast<Season>((currentSeason + 1) % 4);
            }
        }
        epochCounter++;
    }
};

class Creature 
{
public:
    float x, y; // Position for rendering
    int speed;
    bool hasAdaptabilityGene;
    bool hasReproductionGene;
    bool isCarnivorous;
    bool isSick; // New attribute for sickness
    bool hasThickFur;
    bool hasHeatResistance;
    int age; // New attribute for age
    int lifespan; // Maximum age a creature can live
    // Removed duplicate gainRandomGene declaration

    Creature(float x, float y, int speed, bool adaptability = false, bool reproduction = false, bool carnivorous = false, bool thickFur = false, bool heatResistance = false)
    : x(x), y(y), speed(speed), hasAdaptabilityGene(adaptability), hasReproductionGene(reproduction), isCarnivorous(carnivorous),
      isSick(false), age(0), lifespan(60), hasThickFur(thickFur), hasHeatResistance(heatResistance) {}

    // Fixed the reproduce method declaration
    Creature reproduce(const Environment& environment) 
    {
        bool childThickFur = this->hasThickFur || (rand() % 100 < 5); // 5% chance of mutation
        bool childHeatResistance = this->hasHeatResistance || (rand() % 100 < 5); // 5% chance of mutation

        bool childSickness = isSick && (rand() % 10 < 2); // 20% chance to inherit sickness
        Creature child(x + rand() % 20 - 10, y + rand() % 20 - 10, speed + (rand() % 3 - 1), hasAdaptabilityGene, hasReproductionGene, isCarnivorous, childThickFur, childHeatResistance);
        child.isSick = childSickness;
        return child;
    }

    void checkSickness() 
    {
        if (age == 4 || age == 10 || age == 20) {
            isSick = (rand() % 2 == 0); // 50% chance to get sick at these ages
        }
    }

    void ageOneYear() 
    {
        if (!isSick) 
        {
            age++;
        } else 
        {
            age += 2; // Sickness accelerates aging
        }
    }

    bool isAlive() 
    {
        return age <= lifespan && !isSick;
    }
};

void runSimulation(sf::RenderWindow& window, int initialPopulation, int generations, int survivalThreshold) 
{
    srand(time(0));
    Environment environment;
    std::vector<Creature> population;

    for (int i = 0; i < initialPopulation; i++) 
    {
        population.push_back(Creature(rand() % window.getSize().x, rand() % window.getSize().y, rand() % 10 + 1, rand() % 2, false, rand() % 2));
    }

    for (int generation = 0; generation < generations && window.isOpen(); generation++) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (generation % 5 == 0) 
        {
            environment.update();
        }

        window.clear();

        std::vector<Creature> newGeneration;
        for (auto& creature : population) 
        {
            creature.ageOneYear();
            creature.checkSickness();
            if (creature.isAlive() && creature.survivesInCurrentCondition(environment)) 
            {
                newGeneration.push_back(creature.reproduce(environment));
            }
        }
        population = newGeneration;

        for (const Creature& creature : population) 
        {
            sf::CircleShape shape(5);
            shape.setPosition(creature.x, creature.y);
            shape.setFillColor(creature.isCarnivorous ? sf::Color::Red : sf::Color::Green);
            if (creature.isSick) 
            {
                shape.setOutlineColor(sf::Color::Yellow);
                shape.setOutlineThickness(2);
            }
            window.draw(shape);
        }

        window.display();
        sf::sleep(sf::milliseconds(100)); // Slow down the simulation for visibility
    }
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Evolution Simulation");

    int initialPopulation = 100;
    int generations = 500; // Increase or decrease based on your needs
    int survivalThreshold = 5;

    runSimulation(window, initialPopulation, generations, survivalThreshold);
    return 0;
}







