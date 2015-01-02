#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

#include <memory>

#include <SFML/Graphics/CircleShape.hpp>

#include "Target.hpp"

class Waypoint : public Target
{
public:
    typedef std::unique_ptr<Waypoint> WypntPtr;

private:
    sf::CircleShape             mCircle;

public:
                                Waypoint(float radius
                                         , sf::Vector2f pos)
                                : mCircle(radius)

                                {
                                    mCircle.setFillColor(sf::Color::Red);

                                    sf::FloatRect bounds = mCircle.getLocalBounds();
                                    mCircle.setOrigin(bounds.width / 2.f
                                                      , bounds.height / 2.f);

                                    mCircle.setPosition(pos);
                                }

    virtual                     ~Waypoint(){};

    // Getters
    virtual sf::Vector2f        targetPosition() const
                                { return mCircle.getPosition(); }

    virtual float               targetRadius() const
                                { return mCircle.getRadius(); }

    const sf::CircleShape&      getCircle()
                                { return mCircle; }
};

#endif // WAYPOINT_HPP
