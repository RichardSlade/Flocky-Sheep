#ifndef MOVINGENTITY_HPP
#define MOVINGENTITY_HPP

#include <SFML/System/Vector2.hpp>

#include "Target.hpp"
#include "SceneNode.hpp"

class MovingEntity : public SceneNode, public Target
{
protected:
    sf::Vector2f            mVelocity;
    sf::Vector2f            mHeading;

    mutable float           mSpeed;
    float                   mMass;
    float                   mMaxSpeed;
    float                   mMaxForce;
    float                   mMaxTurnRate;

public:
                            MovingEntity(double mass = 1.f, double speed = 2.f, double force = 2.f, double turn = 2.f)
                            : mVelocity(0.f, 0.f)
                            , mHeading(0.f, 0.f)
                            , mMass(mass)
                            , mMaxSpeed(speed)
                            , mMaxForce(force)
                            , mMaxTurnRate(turn)
                            {};

    virtual                 ~MovingEntity(){};

    // Virtual Functions
    virtual sf::Vector2f    position() const { return getWorldPosition(); }
//    virtual sf::Vector2f    position() const { return getPosition(); }
    virtual sf::Vector2f    velocity() const { return mVelocity; }
    virtual sf::Vector2f    heading() const { return mHeading; }
    virtual float           speed() const { return mSpeed; }

    // Getters
    float                   mass() const { return mMass; }
    float                   maxSpeed() const { return mMaxSpeed; }
    float                   maxForce() const { return mMaxForce; }
    float                   maxTurnRate() const { return mMaxTurnRate; }

    // Setters
    void                    setSpeed(float speed) const { mSpeed = speed; }
};

#endif // MOVINGENTITY_HPP
