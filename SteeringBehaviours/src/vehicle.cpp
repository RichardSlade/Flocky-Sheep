#include <iostream>
#include <cmath>

#include "Vehicle.hpp"
#include "Utility.hpp"
//#include "SteeringBehaviour.hpp"
#include "World.hpp"

Vehicle::Vehicle(const World* world
                 , sf::Texture& texture
                 , sf::Vector2f startPos
                 , SteeringBehaviours::Type steer
                 , Target* targ
                 , Target* pursue)
: MovingEntity()
, mWorld(world)
, mSprite(texture)
, mSteering(this, steer, sf::Vector2f(1.f, 1.f))
, mTarget(targ)
, mPursuer(pursue)
{
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    setPosition(startPos);

//    float theta = randomClamped() * 2.f;
//    rotate(theta * (180 / 3.14159));
//    mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));
}

void Vehicle::updateCurrent(sf::Time dt)
{
    sf::Vector2f steeringForce = mSteering.calculate(dt);

    sf::Vector2f acceleration = steeringForce / mMass;

    sf::Vector2f oldVelocity(mVelocity);

    float currentRotation = getRotation() * (3.14159 / 180.f);

//    sf::Vector2f targetVector(mTarget->position() - getWorldPosition());
    sf::Vector2f targetVector(acceleration);
    mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));
//    int sign = signVec(mHeading, steeringForce);
    int sign = signVec(mHeading, targetVector);
//    float angle = std::acos(dotVec(mHeading, normVec(acceleration)));
    float angle = std::acos(dotVec(mHeading, normVec(targetVector)));
    angle *= sign;

    clampRotation(angle, -0.2, 0.2);

    if(angle > 0.000001 || angle < -0.000001)
        rotate(angle * (180 / 3.14159));

    mVelocity += acceleration * dt.asSeconds();

    truncateVec(mVelocity, mMaxSpeed);

    move(mVelocity);

//    if(magVec(mVelocity) > 0.0000000001)
//        mHeading = normVec(mVelocity);
}

void Vehicle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
//    target.draw(mSteering.wanderCircle(), states);
    target.draw(mSteering.targetLocal(), states);
//    target.draw(mSteering.targetWorld(), states);
    target.draw(mSteering.lines(), states);
}

void Vehicle::handleCollisions()
{

}
