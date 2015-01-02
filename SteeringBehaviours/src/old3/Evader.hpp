#ifndef EVADER_HPP
#define EVADER_HPP

struct Evader
{
public:
    virtual                 ~Evader(){};

    virtual sf::Vector2f    evaderPosition() const = 0;
    virtual sf::Vector2f    evaderVelocity() const = 0;
    virtual sf::Vector2f    evaderHeading() const = 0;
    virtual float           evaderSpeed() const = 0;
    virtual float           evaderRadius() const = 0;
};

#endif // EVADER_HPP
