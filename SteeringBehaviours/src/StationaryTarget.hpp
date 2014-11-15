#ifndef STATIONARYTARGET_HPP
#define STATIONARYTARGET_HPP

#include <memory>

#include <SFML/Graphics/CircleShape.hpp>

#include "Target.hpp"
#include "SceneNode.hpp"

class StationaryTarget : public SceneNode, public Target
{
private:
    sf::CircleShape     mCircle;

    virtual void        drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

public:
                        StationaryTarget(float radius, sf::Vector2f pos)
                        : mCircle(radius)
                        {
                            sf::FloatRect bounds = mCircle.getLocalBounds();
                            mCircle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                            setPosition(pos);
                        };

    // Getters
    sf::Vector2f        position() const { return getWorldPosition(); }
    sf::Vector2f        velocity() const { return sf::Vector2f(0.f, 0.f); }
    sf::Vector2f        heading() const { return sf::Vector2f(0.f, 0.f); }
    float               speed() const { return 0.f; }

    // Setters
};

#endif // STATIONARYTARGET_HPP
