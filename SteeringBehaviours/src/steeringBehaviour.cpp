#include <cmath>
#include <algorithm>
#include <iostream>

#include "SteeringBehaviour.hpp"
#include "Sheep.hpp"
#include "Target.hpp"
#include "Utility.hpp"
#include "Scenery.hpp"
#include "Wall.hpp"

const float SteeringBehaviour::mWanderRadius = 1.2f;
const float SteeringBehaviour::mWanderDistance = 30.f;
const float SteeringBehaviour::mWanderJitter = 80.f;
const float SteeringBehaviour::mPI = 3.14159265358979;
const float SteeringBehaviour::mMinViewBoxLength = 60.f;
const float SteeringBehaviour::mObstacleAvoidanceMultiplier = 5.f;
const float SteeringBehaviour::mWallAvoidanceMultiplier = 10.f;
const float SteeringBehaviour::mSeekMultiplier = 1.f;
const float SteeringBehaviour::mWanderMultiplier = 1.f;
const float SteeringBehaviour::mFeelerLength = 25.f;

SteeringBehaviour::SteeringBehaviour(const Sheep* host)
: mHost(host)
, mTheta(mHost->getRotation() * (mPI / 180.f))
, mWanderTarget(std::sin(mTheta) * mWanderRadius, -std::cos(mTheta) * mWanderRadius)
, mObstacleCollision(false)
//, mWanderCircle(mWanderRadius)
//, mLocalTarget(4.f)
//, mWorldTarget(4.f)
, mViewBox(sf::Vector2f(mHost->getSpriteWidth(), -mMinViewBoxLength))
, mViewBoxEnd(mHost->radius(),-mMinViewBoxLength)
, mLines(sf::Lines)
{
    mViewBox.setPosition(-mHost->getSpriteWidth() / 2.f, 0.f);
    mViewBox.setFillColor(sf::Color::Green);

    for(bool& b : mBehaviourFlags)
        b = false;

    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;


//    sf::FloatRect bounds = mWanderCircle.getLocalBounds();
//    mWanderCircle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//    mWanderCircle.setPosition(mHost->position());
//    mWanderCircle.setFillColor(sf::Color::Yellow);
//
//    bounds = mLocalTarget.getLocalBounds();
//    mLocalTarget.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//    mLocalTarget.setFillColor(sf::Color::Green);
//
//    bounds = mWorldTarget.getLocalBounds();
//    mWorldTarget.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//    mWorldTarget.setFillColor(sf::Color::Cyan);

//    mWorldTarget.setPosition(0.f, 0.f);

//    mWanderTarget = mHost->getTransform() * mWanderTarget;
};

bool SteeringBehaviour::accumulateForce(sf::Vector2f& runningTotal, sf::Vector2f forceToAdd)
{
    float magSoFar = magVec(runningTotal);

    float magRemaining = mHost->maxForce() - magSoFar;

//    std::cout << "Running Total X: " << runningTotal.x << " Y: " << runningTotal.y << std::endl;
//    std::cout << "Force to Add X: " << forceToAdd.x << " Y: " << forceToAdd.y << std::endl;
//    std::cout << "Mag so far: " << magSoFar << std::endl;
//    std::cout << "Mag Remaining: " << magRemaining << std::endl;

    if(magRemaining <= 0.01)
    {
//        std::cout << "No Space" << std::endl;
        return false;
    }

    double magToAdd = magVec(forceToAdd);

    if(magToAdd < magRemaining)
    {
        runningTotal += forceToAdd;
    }
    else
    {
        runningTotal += normVec(forceToAdd) * magRemaining;
    }

    return true;
}

void SteeringBehaviour::createFeelers()
{
    mFeelers.clear();
    mFeelerCollisions.clear();
    mLines.clear();

    mFeelers.push_back(sf::Vector2f(std::sin(-45.f * (mPI / 180.f)) * mFeelerLength
                                      , -std::cos(-45.f *(mPI / 180.f)) * mFeelerLength));

    mFeelers.push_back(sf::Vector2f(0.f
                                      , -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(std::sin(45.f * (mPI / 180.f)) * mFeelerLength
                                      , -std::cos(45.f *(mPI / 180.f)) * mFeelerLength));

    for(sf::Vector2f& v : mFeelers)
    {
        mFeelerCollisions.push_back(false);

        mLines.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Red));
        mLines.append(sf::Vertex(v, sf::Color::Red));
    }
}

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

    sf::Vector2f steeringForce;

    if(dist > 0.f)
    {
        const float DecelerationTweaker = 0.3;

        float speed = dist / (static_cast<float>(deceleration) * DecelerationTweaker);

        speed = std::min(speed, mHost->maxSpeed());

        mHost->setSpeed(speed);

        sf::Vector2f desiredVelocity = toTarget * speed / dist;

        steeringForce = (desiredVelocity - mHost->velocity());
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::evade(const Sheep* pursuer)
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

    float jitter1 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;
    float jitter2 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;

    mWanderTarget += sf::Vector2f(jitter1
                                  , jitter2);

    mWanderTarget = normVec(mWanderTarget);
    mWanderTarget *= mWanderRadius;

    sf::Vector2f targetLocal = mWanderTarget + sf::Vector2f(0.f, -mWanderDistance);
    sf::Vector2f targetWorld = mHost->getWorldTransform() * targetLocal;

//    mLines.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Green));
//    mLines.append(sf::Vertex(targetLocal, sf::Color::Green));

    sf::Vector2f returns(targetWorld - mHost->getWorldPosition());

    return returns;
}

sf::Vector2f SteeringBehaviour::obstacleAvoidance()
{
    mViewBox.setFillColor(sf::Color::Green);

    std::vector<Scenery*> nearObstacles = mHost->getObstaclesInRange();

    Scenery* closestObstacle = nullptr;

    float distToClosest = -1.f;

    for(Scenery* ob : nearObstacles)
    {
        sf::Vector2f obPos = mHost->getInverseTransform() * ob->getPosition();

        if(obPos.y <= 0.f)
        {
            float expandedRadius = ob->radius() + mHost->radius();
//            float expandedRadius = ob->radius() + mMinViewBoxLength;

            if(std::fabs(obPos.x) < expandedRadius
               && std::fabs(obPos.y) < expandedRadius)
            {
//                ob->changeColour(sf::Color::Magenta);

                float sqrtPart = std::sqrt((expandedRadius * expandedRadius) - (obPos.x * obPos.x));
                float intersectionPoint = obPos.y - sqrtPart;

                if(intersectionPoint >= 0.f)
                {
                    intersectionPoint = obPos.y + sqrtPart;
                }

                if(intersectionPoint < distToClosest
                   || distToClosest == -1.f)
                {
                    distToClosest = intersectionPoint;
                    closestObstacle = ob;
                }
            }
        }
    }

    sf::Vector2f steeringForce;

    if(closestObstacle)
    {
//        mViewBox.setFillColor(sf::Color::Magenta);
        mObstacleCollision = true;

        sf::Vector2f closestPos = mHost->getInverseTransform() * closestObstacle->getPosition();

        float multiplier = 1.f + (mMinViewBoxLength - closestPos.y) / mMinViewBoxLength;

        steeringForce.x = (closestObstacle->radius() - closestPos.x) * multiplier;

//        const float brakingWeight = 0.02;

        steeringForce.y = (closestObstacle->radius() - closestPos.y);// * brakingWeight;
        steeringForce.y *= -1.f;
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::wallAvoidance()
{
    createFeelers();

    float distToThisIntersection = 0.f;
    float distToClosestIntersection = 9999.f;

//    int closestWall = -1;

    sf::Vector2f steeringForce, point, closestPoint;

    std::vector<Wall*> walls = mHost->getWalls();

//    int collisionCount = 0;

    for(size_t flr = 0; flr < Feelers::NumFlr; flr++)
    {
        int closestWall = -1;
        int index = 0;

        for(Wall* w : walls)
        {
            std::pair<sf::Vector2f, sf::Vector2f> wallPoints = w->getPoints();

            if(lineIntersection2D(mHost->getWorldPosition()
                                  , mHost->getWorldTransform() * mFeelers.at(flr)
                                  , wallPoints.first
                                  , wallPoints.second
                                  , distToThisIntersection
                                  , point))
            {
                mFeelerCollisions.at(flr) = true;

                if(distToThisIntersection < distToClosestIntersection)
                {
                    distToClosestIntersection = distToThisIntersection;
                    closestWall = index;
                    closestPoint = point;
                }
            }

            index ++;
        }

        if(closestWall >= 0)
        {
            sf::Vector2f overShoot = (mHost->getWorldTransform() * mFeelers.at(flr)) - closestPoint;
            steeringForce = walls.at(closestWall)->getNorm() * magVec(overShoot);
        }
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::calculate(sf::Time dt)
{
    sf::Vector2f steeringForce;

    mObstacleCollision = false;

    if(mBehaviourFlags[WallAvoidance])
    {
        sf::Vector2f force = wallAvoidance() * mWallAvoidanceMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags[ObstacleAvoidance])
    {
        sf::Vector2f force = obstacleAvoidance() * mObstacleAvoidanceMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;

    }

    if(mBehaviourFlags[Seek])
    {
        sf::Vector2f force = seek(mHost->targetPos()) * mSeekMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags[Wander])
    {
        sf::Vector2f force = wander(dt) * mWanderMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

//      steeringForce = arrive(mHost->targetPos(), Deceleration::Fast);
//     steeringForce = flee(mHost->targetPos());
//    steeringForce = pursuit(mHost->target());
//    steeringForce = wander(dt);

    return steeringForce;
}

void SteeringBehaviour::setNewBehaviour(SteeringBehaviour::Behaviour newType)
{
    for(bool& b : mBehaviourFlags)
        b = false;

    mBehaviourFlags.at(newType) = true;
    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
}
