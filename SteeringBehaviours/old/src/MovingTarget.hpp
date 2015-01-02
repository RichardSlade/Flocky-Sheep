#ifndef MOVINGTARGET_HPP
#define MOVINGTARGET_HPP

#include "Target.hpp"

struct MovingTarget : public Target
{
public:
    virtual                 ~MovingTarget(){};

    virtual sf::Vector2f    targetVelocity() const = 0;
    virtual sf::Vector2f    targetHeading() const = 0;
    virtual float           targetSpeed() const = 0;
};

#endif // MOVINGTARGET_HPP
