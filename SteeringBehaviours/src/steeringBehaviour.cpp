#include <cmath>
#include <algorithm>
#include <iostream>

#include "SteeringBehaviour.hpp"
#include "Vehicle.hpp"
#include "Target.hpp"
#include "Utility.hpp"

const float SteeringBehaviour::mWanderRadius = 40.f;
const float SteeringBehaviour::mWanderDistance = 10.f;
const float SteeringBehaviour::mWanderJitter = 40.f;
const float SteeringBehaviour::mPI = 3.14159265358979;

SteeringBehaviour::SteeringBehaviour(const Vehicle* host
                                     , SteeringBehaviours::Type steerType
                                     , sf::Vector2f wanderTarg)
: mHost(host)
, mSteerType(steerType)
, mTheta(mHost->getRotation() * (mPI / 180.f))
//, mTheta(0.628)
//, mWanderTarget(sf::Vector2f(mHost->position().x + (mWanderRadius * std::cos(mTheta))
//                             , mHost->position().y + (mWanderRadius * std::sin(mTheta))))
//, mWanderTarget(sf::Vector2f(mWanderRadius * std::cos(mTheta)
//                             , mWanderRadius * std::sin(mTheta)))
, mWanderTarget(std::sin(mTheta) * mWanderRadius, -std::cos(mTheta) * mWanderRadius)
, mWanderCircle(mWanderRadius)
, mLocalTarget(4.f)
, mWorldTarget(4.f)
, mLines(sf::Lines)
{
    sf::FloatRect bounds = mWanderCircle.getLocalBounds();
    mWanderCircle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mWanderCircle.setPosition(mHost->position());

    bounds = mLocalTarget.getLocalBounds();
    mLocalTarget.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mLocalTarget.setFillColor(sf::Color::Green);

    bounds = mWorldTarget.getLocalBounds();
    mWorldTarget.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mWorldTarget.setFillColor(sf::Color::Cyan);

//    mWorldTarget.setPosition(0.f, 0.f);

//    mWanderTarget = mHost->getTransform() * mWanderTarget;
};

sf::Vector2f SteeringBehaviour::seek(sf::Vector2f target)
{
    sf::Vector2f desiredVelocity = normVec(target - mHost->position()) * mHost->maxSpeed();

    return (desiredVelocity - mHost->velocity());
}

sf::Vector2f SteeringBehaviour::arrive(sf::Vector2f target,
                                       Deceleration deceleration)
{
    sf::Vector2f toTarget = target - mHost->position();

    float dist = magVec(toTarget);

    if(dist > 0.f)
    {
        const float DecelerationTweaker = 0.3;

        float speed = dist / (static_cast<float>(deceleration) * DecelerationTweaker);

        speed = std::min(speed, mHost->maxSpeed());

        mHost->setSpeed(speed);

        sf::Vector2f desiredVelocity = toTarget * speed / dist;

        return (desiredVelocity - mHost->velocity());
    }

    return sf::Vector2f(0.f, 0.f);
}

sf::Vector2f SteeringBehaviour::evade(const Vehicle* pursuer)
{
    sf::Vector2f toPursuer(pursuer->position() - mHost->position());

    float lookAheadTime = magVec(toPursuer) / (mHost->maxSpeed() + pursuer->speed());

    return flee(pursuer->position() + pursuer->velocity() * lookAheadTime);
}

sf::Vector2f SteeringBehaviour::flee(sf::Vector2f target)
{
    const float PanicDistance = 100.f * 100.f;

    if(std::pow(magVec(mHost->position() - target), 2) < PanicDistance)
    {
        sf::Vector2f desiredVelocity = normVec(mHost->position() - target) * mHost->maxSpeed();

        return (desiredVelocity - mHost->velocity());
    }

    return mHost->velocity() * -1.f;
}

sf::Vector2f SteeringBehaviour::pursuit(const Target* evader)
{
    sf::Vector2f toEvader(evader->position() - mHost->position());

    float relativeHeading = dotVec(mHost->heading(), evader->heading());

    if((dotVec(toEvader, mHost->heading()) > 0.f)
        && (relativeHeading < 10.f))
    {
        return seek(evader->position());
    }

    float lookAheadTime = magVec(toEvader) / (mHost->maxSpeed() * evader->speed());

    return seek(evader->position() + evader->velocity() * lookAheadTime);
}

sf::Vector2f SteeringBehaviour::wander(sf::Time dt)
{
    float jitterTimeSlice = mWanderJitter * dt.asSeconds();

    std::cout << "Wander Targ: " << mWanderTarget.x << " " << mWanderTarget.y << std::endl;

//    float jitter1 = randomClamped() * jitterTimeSlice;
//    float jitter2 = randomClamped() * jitterTimeSlice;
    float jitter1 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;
    float jitter2 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;

    float currentRotation = mHost->getRotation() * (mPI / 180.f);

//    mWanderTarget = sf::Vector2f(std::sin(currentRotation) * mWanderRadius
//                                 , -std::cos(currentRotation) * mWanderRadius);

//    mWanderTarget /= std::sqrt(2.f);

    mWanderTarget += sf::Vector2f(jitter1
                                  , jitter2);

    std::cout << "Jitter: " << jitter1 << " " << jitter2 << std::endl;
    std::cout << "Wander Targ +Jitter: " << mWanderTarget.x << " " << mWanderTarget.y << std::endl;

    mWanderTarget = normVec(mWanderTarget);
    mWanderTarget *= mWanderRadius;

//    mWanderTarget.x *= std::sin(currentRotation);
//    mWanderTarget.y *= -std::cos(currentRotation);

    std::cout << "Wander Targ Translate to Circle: " << mWanderTarget.x << " " << mWanderTarget.y << std::endl;
    sf::Vector2f hostPos(mHost->position());

//    sf::Vector2f targetLocal = sf::Vector2f(std::sin(currentRotation) * mWanderTarget.x
//                                                            , std::cos(currentRotation) * mWanderTarget.y);
//    sf::Vector2f targetLocal = mWanderTarget *= mWanderDistance;
//    sf::Vector2f targetLocal = mWanderTarget *= mWanderDistance;
    sf::Vector2f targetLocal = mWanderTarget + sf::Vector2f(0.f, 0.f);
//    sf::Vector2f targetLocal = mHost->heading();// + sf::Vector2f(mWanderDistance, 0.f);

    sf::Vector2f targetWorld = mHost->getWorldTransform() * targetLocal;
//    sf::Vector2f targetWorld = mHost->getWorldTransform() * mWanderTarget;

//    std::cout << "Wander Targ With Dist: " << mWanderTarget.x << " " << mWanderTarget.y << std::endl;
    std::cout << "HostPos: " << hostPos.x << " " << hostPos.y << std::endl;
    std::cout << "Local: " << targetLocal.x << " " << targetLocal.y << std::endl;
    std::cout << "World: " << targetWorld.x << " " << targetWorld.y << std::endl;

    mLines.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Green));
    mLines.append(sf::Vertex(targetLocal, sf::Color::Green));

//    mLocalTarget.setPosition(targetLocal);
    mLocalTarget.setPosition(mWanderTarget);
    mWorldTarget.setPosition(targetLocal);

    sf::Vector2f returns(targetWorld - mHost->getWorldPosition());

    std::cout << "Returns: " << returns.x << " " << returns.y << std::endl << std::endl;

    return returns;
}

sf::Vector2f SteeringBehaviour::calculate(sf::Time dt)
{
    mLines.clear();

    switch(mSteerType)
    {
        case SteeringBehaviours::Seek: return seek(mHost->targetPos());
        case SteeringBehaviours::Arrive: return arrive(mHost->targetPos(), Deceleration::Fast);
        case SteeringBehaviours::Flee: return flee(mHost->targetPos());
        case SteeringBehaviours::Pursuit: return pursuit(mHost->target());
        case SteeringBehaviours::Wander: return wander(dt);
        default: return sf::Vector2f(0.f, 0.f);
    }
}
