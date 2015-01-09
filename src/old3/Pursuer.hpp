#ifndef PURSUER_HPP
#define PURSUER_HPP

struct Pursuer
{
public:
    virtual                     ~Pursuer(){};

    virtual sf::Vector2f        pursuerPosition() const = 0;
    virtual sf::Vector2f        pursuerVelocity() const = 0;
    virtual sf::Vector2f        pursuerHeading() const = 0;
    virtual float               pursuerSpeed() const = 0;
    virtual float               pursuerRadius() const = 0;
};

#endif // PURSUER_HPP
