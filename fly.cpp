#include "fly.h"
#include "math.h"

#include <functional>

#define speed 25

fly::fly()
{
    this->setPos(sf::Vector2f(400, 300));
    this->flyShape.setPointCount(10);
    this->flyShape.setRadius(3);
    this->flyShape.setPosition(this->getPos());
    this->flyShape.setFillColor(sf::Color::Black);
}


sf::Vector2f fly::getPos()
{
    return this->pos;
}

void fly::setPos(sf::Vector2f newPos)
{
    this->pos = newPos;
}

sf::CircleShape fly::getFlyShape()
{
    return this->flyShape;
}

void fly::setFlyShapePosition(const sf::Vector2f &vec)
{
    this->flyShape.setPosition(vec);
}

void fly::moveFly(const int &angle, const sf::Vector2f &windowSize, sf::CircleShape &goal)
{

    this->setPos(this->getPos() + sf::Vector2f(std::cos(angle)*speed, std::sin(angle)*speed));
    this->setFlyShapePosition(this->getPos());

    this->addMove(sf::Vector2f(std::cos(angle)*speed, std::sin(angle)*speed));

    if(this->flyShape.getGlobalBounds().intersects(goal.getGlobalBounds()) || this->getPos().x > windowSize.x-10 || this->getPos().y > windowSize.y-10 || this->getPos().x < 10 || this->getPos().y < 10)
        this->dead = true;

}


void fly::moveFly(sf::Vector2f &movement, const sf::Vector2f &windowSize, sf::CircleShape &goal)
{

    this->setPos(this->getPos() + movement);
    this->setFlyShapePosition(this->getPos());


    if(this->flyShape.getGlobalBounds().intersects(goal.getGlobalBounds()) || this->getPos().x > windowSize.x-10 || this->getPos().y > windowSize.y-10 || this->getPos().x < 10 || this->getPos().y < 10)
        this->dead = true;

}


bool fly::isDead()
{
    return this->dead;
}

void fly::setDead(bool &isDead)
{
    this->dead = isDead;
}

double fly::calcFitness(sf::CircleShape &goal)
{
    double bonus = 0;
    if(this->flyShape.getGlobalBounds().intersects(goal.getGlobalBounds()))
        bonus = 3;


    return 1/(std::sqrt(std::pow(goal.getPosition().x-this->getPos().x, 2) + (std::pow(goal.getPosition().y-this->getPos().y, 2)))) + bonus;
}


std::vector<sf::Vector2f> fly::getMoves()
{
    return this->moves;
}

void fly::setMoves(const std::vector<sf::Vector2f> &movements)
{
    this->moves = movements;
}

void fly::addMove(const sf::Vector2f &movement)
{
    this->moves.push_back(movement);
}

void fly::setFlyShapeColour(const sf::Color &colour)
{
    this->flyShape.setFillColor(colour);
}

