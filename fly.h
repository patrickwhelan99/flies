#include <SFML/Graphics.hpp>

class fly
{
    public:
        fly();

        sf::Vector2f getPos();
        void setPos(sf::Vector2f newPos);

        sf::CircleShape getFlyShape();
        void setFlyShapePosition(const sf::Vector2f &vec);

        void moveFly(const int &angle, const sf::Vector2f &windowSize, sf::CircleShape &goal);
        void moveFly(sf::Vector2f &movement, const sf::Vector2f &windowSize, sf::CircleShape &goal);

        bool isDead();
        void setDead(bool &isDead);

        std::vector<sf::Vector2f> getMoves();
        void setMoves(const std::vector<sf::Vector2f> &movements);
        void addMove(const sf::Vector2f &movement);

        double calcFitness(sf::CircleShape &goal);

        double fitness = 0;

        void setFlyShapeColour(const sf::Color &colour);

    private:
        sf::Vector2f pos;
        sf::CircleShape flyShape;
        bool dead = false;
        std::vector<sf::Vector2f> moves;

};
