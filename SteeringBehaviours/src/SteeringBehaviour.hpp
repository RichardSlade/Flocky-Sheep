#ifndef STEERINGBEHAVIOUR_HPP
#define STEERINGBEHAVIOUR_HPP

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Enum.hpp"

class Vehicle;
class Target;

class SteeringBehaviour
{
public:
    enum Deceleration
    {
        Slow = 3,
        Normal = 2,
        Fast = 1,
    };

private:
    static const float              mWanderRadius;
    static const float              mWanderDistance;
    static const float              mWanderJitter;
    static const float              mPI;

    const Vehicle*                  mHost;
    SteeringBehaviours::Type        mSteerType;
    float                           mTheta;
    sf::Vector2f                    mWanderTarget;

    sf::CircleShape                 mWanderCircle;
    sf::CircleShape                 mLocalTarget;
    sf::CircleShape                 mWorldTarget;
    sf::VertexArray                 mLines;

    sf::Vector2f                    seek(sf::Vector2f);
    sf::Vector2f                    arrive(sf::Vector2f, Deceleration);
    sf::Vector2f                    evade(const Vehicle*);
    sf::Vector2f                    flee(sf::Vector2f);
    sf::Vector2f                    pursuit(const Target*);
    sf::Vector2f                    wander(sf::Time);

public:
                                    SteeringBehaviour(const Vehicle*
                                                      , SteeringBehaviours::Type
                                                      , sf::Vector2f);

    sf::Vector2f                    calculate(sf::Time);

    sf::CircleShape                 wanderCircle() const { return mWanderCircle; }
    sf::CircleShape                 targetLocal() const { return mLocalTarget; }
    sf::CircleShape                 targetWorld() const { return mWorldTarget; }
    sf::VertexArray                 lines() const { return mLines; }
};

#endif // STEERINGBEHAVIOUR_HPP
