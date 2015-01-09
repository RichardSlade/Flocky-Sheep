#ifndef TARGET_HPP
#define TARGET_HPP

#include <SFML/System/Vector2.hpp>

struct Target
{
public:
    virtual                     ~Target(){};

    // Virtual Getters
    virtual sf::Vector2f        targetPosition() const = 0;
    virtual float               targetRadius() const = 0;
};

#endif // TARGET_HPP
