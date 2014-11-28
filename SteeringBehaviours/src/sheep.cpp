#include <iostream>
#include <cmath>
#include <vector>

#include "Sheep.hpp"
#include "Utility.hpp"
#include "World.hpp"
#include "Scenery.hpp"

Sheep::Sheep(const World* world
                 , sf::Texture& texture
                 , sf::Vector2f startPos
                 , float scale
                 , Target* targ
                 , Target* pursue)
: MovingEntity()
, mWorld(world)
, mSprite(texture)
, mSteering(this)
, mTarget(targ)
, mPursuer(pursue)
{
    mSprite.scale(scale, scale);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mRadius = std::max(bounds.width, bounds.height);

    setPosition(startPos);

    float theta = randomClamped() * (2.f * SteeringBehaviour::mPI);
    rotate(theta * (180 / SteeringBehaviour::mPI));
    mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));
}

void Sheep::updateCurrent(sf::Time dt)
{
    sf::Vector2f steeringForce = mSteering.calculate(dt);

//    sf::Vector2f acceleration = steeringForce / mMass;
    sf::Vector2f acceleration = steeringForce * 25.f;// / mMass;

    std::vector<bool> collisions = mSteering.getFeelerCollisions();

    int colCount = 0;

    for(bool b : collisions)
        if(b) colCount ++;

    if(colCount > 1
       || (colCount > 1 && mSteering.getObstacleCollision()))
    {
        rotate(180.f);
    }
    else
    {
        float currentRotation = getRotation() * (SteeringBehaviour::mPI / 180.f);

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
            rotate(angle * (180.f / SteeringBehaviour::mPI));
    }

    mVelocity = acceleration * dt.asSeconds();

    truncateVec(mVelocity, mMaxSpeed);

    move(mVelocity);
}

void Sheep::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
//    target.draw(mSteering.wanderCircle(), states);
//    target.draw(mSteering.targetLocal(), states);
//    target.draw(mSteering.targetWorld(), states);
//    target.draw(mSteering.viewBox(), states);
//    target.draw(mSteering.lines(), states);
}

std::vector<Scenery*> Sheep::getObstaclesInRange() const
{
    return mWorld->obstaclesInRange(const_cast<Sheep*>(this), mRadius);
}

std::vector<Wall*> Sheep::getWalls() const
{
    return mWorld->getWorldWalls();
}
