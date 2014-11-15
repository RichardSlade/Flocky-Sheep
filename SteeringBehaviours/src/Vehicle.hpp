#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "MovingEntity.hpp"
#include "SteeringBehaviour.hpp"
#include "SceneNode.hpp"

class World;

class Vehicle : public MovingEntity//, public SceneNode,
{
private:
    const World*                mWorld;
    sf::Sprite                  mSprite;

    SteeringBehaviours::Type    mSteeringType;
    SteeringBehaviour           mSteering;

    Target*                     mTarget;
    Target*                     mPursuer;

    virtual void                updateCurrent(sf::Time);
    virtual void                drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

    void                        handleCollisions();

public:
                                Vehicle(const World*
                                        , sf::Texture&
                                        , sf::Vector2f
                                        , SteeringBehaviours::Type
                                        , Target* = nullptr
                                        , Target* = nullptr);

                                ~Vehicle(){};
    // Getters
    sf::Vector2f                targetPos() const{ return mTarget->position(); }
    Target*                     target() const { return mTarget; }
    Target*                     pursuer() const { return mPursuer; }


    // Setters
    void                        setTarget(Target* target){ mTarget = target; }

};

#endif // VEHICLE_HPP
