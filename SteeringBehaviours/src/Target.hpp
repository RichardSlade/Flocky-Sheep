#ifndef TARGET_HPP
#define TARGET_HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct Target
{
public:
                                Target(){};
    virtual                     ~Target(){};

    // Virtual Getters
    virtual sf::Vector2f        position() const = 0;
    virtual sf::Vector2f        velocity() const = 0;
    virtual sf::Vector2f        heading() const = 0;
    virtual float               speed() const = 0;
};

#endif // TARGET_HPP
