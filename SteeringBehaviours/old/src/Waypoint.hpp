#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

#include <memory>

#include <SFML/Graphics/CircleShape.hpp>

#include "Target.hpp"
#include "SceneNode.hpp"

class Waypoint : public SceneNode, public Target
{
private:
    sf::CircleShape             mCircle;

    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                Waypoint(float radius
                                               , sf::Vector2f pos);

    virtual                     ~Waypoint(){};

    // Getters
    virtual sf::Vector2f        targetPosition() const { return getWorldPosition(); }
    virtual sf::Vector2f        targetVelocity() const { return sf::Vector2f(0.f, 0.f); }
    virtual sf::Vector2f        targetHeading() const { return sf::Vector2f(0.f, 0.f); }
    virtual float               targetSpeed() const { return 0.f; }
    virtual float               targetRadius() const { return 0.f; }

    // Setters
};

#endif // WAYPOINT_HPP
