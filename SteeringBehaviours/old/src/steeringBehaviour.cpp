#include <cmath>
#include <algorithm>
#include <iostream>

#include "SteeringBehaviour.hpp"
#include "MovingEntity.hpp"
//#include "Target.hpp"
#include "MovingTarget.hpp"
#include "Utility.hpp"
//#include "Scenery.hpp"
#include "Wall.hpp"
#include "LevelBlock.hpp"
#include "Sheep.hpp"
#include "Params.hpp"

const float SteeringBehaviour::mPI = 3.14159265358979;
//
//const float SteeringBehaviour::mWanderRadius = 2.4f;
//const float SteeringBehaviour::mWanderDistance = 10.f;
//const float SteeringBehaviour::mWanderJitter = 80.f;
////const float SteeringBehaviour::mMinViewBoxLength = 60.f;
//const float SteeringBehaviour::mInteractionRadius = 100.f;
//const float SteeringBehaviour::mFeelerLength = 50.f;
//const float SteeringBehaviour::mMinArriveDist = 15.f;
//
//const float SteeringBehaviour::mObstacleAvoidanceMultiplier = 10.f;
//const float SteeringBehaviour::mWallAvoidanceMultiplier = 10.f;
////const float SteeringBehaviour::mSeekMultiplier = 1.f;
//const float SteeringBehaviour::mArriveMultiplier = 1.f;
//const float SteeringBehaviour::mEvadeMultiplier = 2.f;
//const float SteeringBehaviour::mWanderMultiplier = 0.1f;
//const float SteeringBehaviour::mSeperationMultiplier = 4.f;
//const float SteeringBehaviour::mAlignmentMultiplier = 4.f;
//const float SteeringBehaviour::mCohesionMultiplier = 2.f;
//const float SteeringBehaviour::mFlockingMultiplier = 20.f;

SteeringBehaviour::SteeringBehaviour(const MovingEntity* host
                                     , Params& params)
: mWanderRadius(params.WanderRadius)
, mWanderDistance(params.WanderDistance)
, mWanderJitter(params.WanderJitter)
, mMinViewBoxLength(params.MinViewBoxLength)
, mInteractionRadius(params.InteractionRadius)
, mFeelerLength(params.FeelerLength)
, mMinArriveDist(params.MinArriveDist)
, mObstacleAvoidanceMultiplier(params.ObstacleAvoidanceMultiplier)
, mWallAvoidanceMultiplier(params.WallAvoidanceMultiplier)
, mArriveMultiplier(params.ArriveMultiplier)
, mEvadeMultiplier(params.EvadeMultiplier)
, mWanderMultiplier(params.WanderMultiplier)
, mSeperationMultiplier(params.SeperationMultiplier)
, mAlignmentMultiplier(params.AlignmentMultiplier)
, mCohesionMultiplier(params.CohesionMultiplier)
, mFlockingMultiplier(params.FlockingMultiplier)
, mHost(host)
, mTheta(mHost->getRotation() * (mPI / 180.f))
, mWanderTarget(std::sin(mTheta) * mWanderRadius, -std::cos(mTheta) * mWanderRadius)
//, mObstacleCollision(false)
//, mViewBox(sf::Vector2f(mHost->getSpriteWidth(), -mMinViewBoxLength))
//, mViewBoxEnd(mHost->getRadius(),-mMinViewBoxLength)
//, mLines(sf::Lines)
{
//    mViewBox.setPosition(-mHost->getSpriteWidth() / 2.f, 0.f);
//    mViewBox.setFillColor(sf::Color::Green);

    for(bool& b : mBehaviourFlags)
        b = false;

    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
};

bool SteeringBehaviour::accumulateForce(sf::Vector2f& runningTotal, sf::Vector2f forceToAdd)
{
    float magSoFar = magVec(runningTotal);

    float magRemaining = mHost->getMaxForce() - magSoFar;

    if(magRemaining <= 0.f) return false;

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
//    mLines.clear();

    mFeelers.push_back(sf::Vector2f(std::sin(-45.f * (mPI / 180.f)) * mFeelerLength
                                      , -std::cos(-45.f *(mPI / 180.f)) * mFeelerLength));

    mFeelers.push_back(sf::Vector2f(0.f
                                      , -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(std::sin(45.f * (mPI / 180.f)) * mFeelerLength
                                      , -std::cos(45.f *(mPI / 180.f)) * mFeelerLength));

//    for(sf::Vector2f& v : mFeelers)
//    {
//        mLines.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Red));
//        mLines.append(sf::Vertex(v, sf::Color::Red));
//    }
}

sf::Vector2f SteeringBehaviour::rest()
{
    return sf::Vector2f();
}

sf::Vector2f SteeringBehaviour::arrive(Deceleration deceleration)
{
    sf::Vector2f toTarget = mHost->getTarget()->targetPosition() - mHost->getWorldPosition();

    float dist = magVec(toTarget);

    sf::Vector2f steeringForce;

    if(dist > mMinArriveDist)
    {
        const float DecelerationTweaker = 0.3;

        float speed = dist / (static_cast<float>(deceleration) * DecelerationTweaker);

        speed = std::min(speed, mHost->getMaxSpeed());

//        mHost->setSpeed(speed);

        sf::Vector2f desiredVelocity = toTarget * speed / dist;

        steeringForce = (desiredVelocity - mHost->getVelocity());
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::seek(sf::Vector2f target)
{
    sf::Vector2f desiredVelocity = normVec(target - mHost->getWorldPosition()) * mHost->getMaxSpeed();
//    sf::Vector2f desiredVelocity = normVec(target - mHost->getWorldPosition());

    return (desiredVelocity - mHost->getVelocity());
}

sf::Vector2f SteeringBehaviour::evade()
{
    const MovingTarget* pursuer = mHost->getMovingTarget();

    sf::Vector2f toPursuer(pursuer->targetPosition() - mHost->getWorldPosition());

    float magPursuer = magVec(toPursuer);

    if((magPursuer * magPursuer) > Sheep::mPanicDistance * Sheep::mPanicDistance)
        return sf::Vector2f();

    float lookAheadTime = magPursuer / (mHost->getMaxSpeed() + pursuer->targetSpeed());

    return flee(pursuer->targetPosition() + pursuer->targetVelocity() * lookAheadTime);
}

sf::Vector2f SteeringBehaviour::flee(sf::Vector2f target)
{
//    const float PanicDistance = 100.f * 100.f;

//    if(std::pow(magVec(mHost->getWorldPosition() - target), 2) < PanicDistance)
//    {
        sf::Vector2f desiredVelocity = normVec(mHost->getWorldPosition() - target) * mHost->getMaxSpeed();

//        return (desiredVelocity - mHost->getVelocity());
//    }

    return (desiredVelocity - mHost->getVelocity());
}

sf::Vector2f SteeringBehaviour::pursuit()
{
    const MovingTarget* evader = mHost->getMovingTarget();

    sf::Vector2f toEvader(evader->targetPosition() - mHost->getWorldPosition());

    float relativeHeading = dotVec(mHost->getHeading(), evader->targetHeading());

    if((dotVec(toEvader, mHost->getHeading()) > 0.f)
        && (relativeHeading < -0.95f))
    {
        return seek(evader->targetPosition());
    }

    float lookAheadTime = magVec(toEvader) / (mHost->getMaxSpeed() * evader->targetSpeed());

    return seek(evader->targetPosition() + evader->targetVelocity() * lookAheadTime);
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
//    sf::Vector2f targetLocal = mWanderTarget;
    sf::Vector2f targetWorld = mHost->getWorldTransform() * targetLocal;

//    mLines.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Green));
//    mLines.append(sf::Vertex(targetLocal, sf::Color::Green));

    sf::Vector2f returns(targetWorld - mHost->getWorldPosition());



    return returns;
}

sf::Vector2f SteeringBehaviour::obstacleAvoidance()
{
//    mViewBox.setFillColor(sf::Color::Green);

    float boxLength = mMinViewBoxLength
                    + (mHost->getSpeed() / mHost->getMaxSpeed())
                    * mMinViewBoxLength;

    std::vector<LevelBlock*> nearObstacles = mHost->getBlockTypeInRange(LevelBlock::Type::ObstacleBlock, 200.f);

    LevelBlock* closestObstacle = nullptr;

    float distToClosest = MAXFLOAT;

    sf::Transform hostTrans = mHost->getInverseTransform();

    for(LevelBlock* blck : nearObstacles)
    {
        sf::Vector2f blckPos = hostTrans * blck->getWorldPosition();

        if(blckPos.y <= 0.f)
        {
            float expandedRadius = blck->getRadius() + (mHost->getRadius());

            if(std::fabs(blckPos.x) < expandedRadius
               && std::fabs(blckPos.y) < expandedRadius)
            {
                if((std::fabs(blckPos.x) - blck->getRadius()) <= 0.f)
                {
                    float sqrtPart = std::sqrt((expandedRadius * expandedRadius) - (blckPos.x * blckPos.x));
                    float intersectionPoint = blckPos.y - sqrtPart;

                    if(intersectionPoint <= 0.f)
                    {
                        intersectionPoint = blckPos.y + sqrtPart;
                    }

                    if(intersectionPoint < distToClosest)
                    {
                        distToClosest = intersectionPoint;
                        closestObstacle = blck;
                    }
                }
            }
        }
    }

    sf::Vector2f steeringForce;

    if(closestObstacle)
    {
        sf::Vector2f closestPos = hostTrans * closestObstacle->getWorldPosition();

        float multiplier = 1.f + (boxLength - closestPos.y) / boxLength;

        steeringForce.x = (closestObstacle->getRadius() - closestPos.x) * multiplier;

        const float brakingWeight = 0.02;

        steeringForce.y = (closestObstacle->getRadius() - closestPos.y) * brakingWeight;
//        steeringForce.y *= -1.f;
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::wallAvoidance()
{
    createFeelers();

    float distToThisIntersection = 0.f;
    float distToClosestIntersection = MAXFLOAT;

    sf::Vector2f steeringForce, point, closestPoint, closestNorm;

    std::vector<LevelBlock*> wallBlocks = mHost->getBlockTypeInRange(LevelBlock::Type::WallBlock, mFeelerLength);

    for(size_t flr = 0; flr < Feelers::NumFlr; flr++)
    {
        int closestWall = -1;
        int index = 0;

        for(LevelBlock* block : wallBlocks)
        {
            Wall* wall = dynamic_cast<Wall*>(block->getScenery());

            assert(wall);

            Wall::WallData wallData = wall->getSceneryData();

            if(lineIntersection2D(mHost->getWorldPosition()
                                  , mHost->getWorldTransform() * mFeelers.at(flr)
                                  , wallData.first.first
                                  , wallData.first.second
                                  , distToThisIntersection
                                  , point))
            {

                if(distToThisIntersection < distToClosestIntersection)
                {
                    distToClosestIntersection = distToThisIntersection;
                    closestWall = index;
                    closestPoint = point;
                    closestNorm = wallData.second;
                }
            }

            index ++;
        }

        if(closestWall >= 0)
        {
            sf::Vector2f overShoot = (mHost->getWorldTransform() * mFeelers.at(flr)) - closestPoint;
            steeringForce = closestNorm * magVec(overShoot);
        }
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::seperation()
{
    sf::Vector2f steeringForce;

    std::vector<MovingEntity*> neighbours = mHost->getNeighbours(mHost->getRadius());
//    std::vector<MovingEntity*> neighbours;// = mHost->getNeighbours(mHost->getRadius());

    for(MovingEntity* e : neighbours)
    {
        sf::Vector2f toNeighbour = mHost->getWorldPosition() - e->getWorldPosition();

        steeringForce += normVec(toNeighbour);
//        steeringForce += normVec(toNeighbour) / magVec(toNeighbour);
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::alignment()
{
    sf::Vector2f averageHeading;
    int neighbourCount = 0;

    std::vector<MovingEntity*> neighbours = mHost->getNeighbours(mInteractionRadius);
//    std::vector<MovingEntity*> neighbours;// = mHost->getNeighbours(mInteractionRadius / 4.f);

    sf::Transform hostTrans = mHost->getInverseTransform();
//    sf::Vector2f hostHeading = mHost->getWorldTransform() * mHost->getHeading();
//    sf::Vector2f hostPos = mHost->getWorldPosition();

    for(MovingEntity* e : neighbours)
    {
        if(e != mHost)
        {
            sf::Vector2f entityHostLocalPos = hostTrans * e->getWorldPosition();
//
//            sf::Vector2f toEntity = entityHostLocalPos - hostPos;
//            sf::Vector2f toEntity = e->getWorldPosition() - hostPos;
//
//            float angle = std::acos(dotVec(hostHeading, normVec(toEntity))) * (180.f / mPI);
//
//            if(std::fabs(angle) <= Sheep::mAngleOfVision)
            if(entityHostLocalPos.y < 0.f)
            {
                averageHeading += e->getWorldTransform() * e->getHeading();
                neighbourCount ++;
            }
        }
    }

    if(neighbourCount > 0)
    {
        averageHeading /= static_cast<float>(neighbourCount);
        averageHeading -= mHost->getWorldTransform() * mHost->getHeading();
    }

    return averageHeading;
}

sf::Vector2f SteeringBehaviour::cohesion()
{
    sf::Vector2f steeringForce, centerOfMass;
    int neighbourCount = 0;

//    std::vector<MovingEntity*> neighbours = mHost->getNeighbours(mHost->getRadius() + 20);
    std::vector<MovingEntity*> neighbours = mHost->getNeighbours(mInteractionRadius);
//    std::vector<MovingEntity*> neighbours;// = mHost->getNeighbours(mInteractionRadius);

//    centerOfMass += mHost->getWorldPosition();

//    sf::Transform hostTrans = mHost->getInverseTransform();
//    sf::Vector2f hostPos = mHost->getWorldPosition();
//    sf::Vector2f hostHeading = mHost->getHeading();

    for(MovingEntity* e : neighbours)
    {
        if(e != mHost
           && e)
        {

//            std::cout << "asdas" << std::endl;
//            sf::Vector2f transPos = hostTrans * e->getWorldPosition();

//            sf::Vector2f toEntity = e->getWorldPosition() - hostPos;

//            float angle = std::acos(dotVec(hostHeading, normVec(toEntity))) * (180.f / mPI);

//            if(std::fabs(angle) <= Sheep::mAngleOfVision)
//            {
                centerOfMass += e->getWorldPosition();
                neighbourCount ++;
//            }
        }
    }

    if(neighbourCount > 0)
    {
//        centerOfMass /= static_cast<float>(neighbourCount + 1);
        centerOfMass /= static_cast<float>(neighbourCount);
        steeringForce = seek(centerOfMass);
//
//        std::cout << "NeighboursSize: " << neighbourCount << std::endl;
//        std::cout << "Steering X: " << steeringForce.x << " Y: " << steeringForce.y << std::endl;
    }

    return normVec(steeringForce);
}

sf::Vector2f SteeringBehaviour::flocking()
{
    sf::Vector2f steeringForce;

    steeringForce += alignment() * mAlignmentMultiplier;
    steeringForce += cohesion() * mCohesionMultiplier;

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::calculate(sf::Time dt)
{
    sf::Vector2f steeringForce;

    if(mBehaviourFlags.at(WallAvoidance))
    {
        sf::Vector2f force = wallAvoidance() * mWallAvoidanceMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(ObstacleAvoidance))
    {
        sf::Vector2f force = obstacleAvoidance() * mObstacleAvoidanceMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Seperation))
    {
        sf::Vector2f force = seperation() * mSeperationMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Evade))
    {
        sf::Vector2f force = evade() * mEvadeMultiplier;
//        sf::Vector2f force = flee(mHost->getPursuer()->targetPosition()) * mEvadeMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Flock))
    {
        sf::Vector2f force = flocking() * mFlockingMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Arrive))
    {
        sf::Vector2f force = arrive(Deceleration::Fast) * mArriveMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Wander))
    {
        sf::Vector2f force = wander(dt) * mWanderMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Rest))
    {
        sf::Vector2f force = rest();

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    return steeringForce;
}

void SteeringBehaviour::setNewBehaviours(std::vector<SteeringBehaviour::Behaviour> newTypes)
{
    for(bool& b : mBehaviourFlags)
        b = false;

    for(SteeringBehaviour::Behaviour& bh : newTypes)
    {
        mBehaviourFlags.at(bh) = true;
    }

    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
}

