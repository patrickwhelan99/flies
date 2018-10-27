#include <SFML/Graphics.hpp>
#include "fly.h"
#include <random>

#define PI 3.14159265358
#define mutationChance 1

struct sortByFitness
{
    inline bool operator() (const fly& f1, const fly& f2)
    {
        return (f1.fitness > f2.fitness);
    }
};

void selectChild(std::vector<fly> &flies, std::vector<fly> &newFlies, sf::CircleShape &goal, std::mt19937 &rng)
{

    double runningTotal = 0;

    for(fly &f : flies)
    {
        runningTotal += f.calcFitness(goal);
    }

    std::uniform_real_distribution<> dist(0, runningTotal);
    double rDouble = dist(rng);

    double finder = 0;
    for(fly &f : flies)
    {
        finder += f.calcFitness(goal);
        if(finder >= rDouble)
        {
            fly newFly;
            newFly.setMoves(f.getMoves());
            newFlies.push_back(newFly);
            break;
        }
    }
}

void nextGen(std::vector<fly> &flies, sf::CircleShape &goal, std::mt19937 &rng, int &moveNum)
{
    std::vector<fly> newFlies;

    double runningTotal = 0;

    for(fly &f : flies)
    {
        f.fitness = f.calcFitness(goal);
        runningTotal += f.fitness;
    }

    printf("Avg fitness:\t%f\n", runningTotal/static_cast<double>(flies.size()));

    std::sort(flies.begin(), flies.end(), sortByFitness());

    flies.erase(flies.begin()+100, flies.end()); /// Cull the bottom half of the population

    for(int i=0;i<150;i++) /// Gen the next 150 by probability, higher fitness = higher chance of reproducing
    {
        selectChild(flies, newFlies, goal, rng);
    }

    flies = newFlies;
    moveNum = 0;
}

void update(sf::RenderWindow &app, sf::Vector2f &windowSize, sf::RectangleShape &bg, sf::CircleShape &goal, std::vector<fly> &flies, std::mt19937 &rng, std::uniform_real_distribution<> &dist, int &moveNum, int &generation, sf::Text &generationText)
{
    app.clear();



    generationText.setString(std::to_string(generation));
    generationText.setPosition(sf::Vector2f(450-generationText.getString().getSize()*150, 100));



    app.draw(bg);
    app.draw(generationText);
    app.draw(goal);

    bool allDead = true;

    for(auto &f : flies)
    {
        if(!f.isDead())
        {
            allDead = false;
            bool mutation = false;

            std::uniform_int_distribution<int> intDist(0, 1000);
            int randNum = intDist(rng);

            if(randNum < mutationChance)
                mutation = true;


            if(mutation || f.getMoves().size() <= moveNum)
                f.moveFly(dist(rng), windowSize, goal); //Provide random number between 0 & 2*PI || Euclidian x movement = Cos(angle) || Euclidian y movement = Sin(angle)
            else
                f.moveFly(f.getMoves().at(moveNum), windowSize, goal);
        }

        app.draw(f.getFlyShape());
    }

    moveNum++;

    if(allDead)
    {
        nextGen(flies, goal, rng, moveNum);
        generation++;
    }


    app.display();
    return;
}



int main()
{

    sf::Vector2f windowSize(800, 800);
    sf::RenderWindow app(sf::VideoMode(windowSize.x, windowSize.y), "Bloody Flies!");


    sf::RectangleShape bg(windowSize);

    sf::CircleShape goal(5, 10);
    goal.setPosition(sf::Vector2f(windowSize.x/2 - 2.5, 20));
    goal.setFillColor(sf::Color::Red);


    std::vector<fly> flies;


    for(int i=0;i<200;i++)
    {
        fly newFly;
        flies.push_back(newFly);
    }


    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<> dist(0, 2*PI);

    int generation = 1;
    int moveNum = 0;


    // Setup Background text
    sf::Text generationText;
    generationText.setCharacterSize(512);


    generationText.setColor(sf::Color(245, 245, 245));

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/google-droid/DroidSans.ttf");
    generationText.setFont(font);


    while (app.isOpen())
    {

        sf::Event event;
        while (app.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                app.close();
        }


        update(app, windowSize, bg, goal, flies, rng, dist, moveNum, generation, generationText);
    }

    return EXIT_SUCCESS;
}
