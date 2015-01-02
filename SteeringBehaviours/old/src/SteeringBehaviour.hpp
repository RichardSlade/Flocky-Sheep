#ifndef STEERINGBEHAVIOUR_HPP
#define STEERINGBEHAVIOUR_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Enum.hpp"

//class Sheep;
class Params;
class MovingEntity;
class Target;

class SteeringBehaviour
{
public:
    typedef std::unique_ptr<SteeringBehaviour> upSteeringBehaviour;

    enum Deceleration
    {
        Slow = 3,
        Normal = 2,
        Fast = 1
    };

    enum Behaviour
    {
        Rest,
        Arrive,
        Evade,
        Pursuit,
        Wander,
        ObstacleAvoidance,
        WallAvoidance,
        Seperation,
        Flock,
        NumBehaviour
    };

    enum Feelers
    {
        Left,
        Front,
        Right,
        NumFlr
    };

     static const float               mPI;

private:
     const float                      mWanderRadius;
     const float                      mWanderDistance;
     const float                      mWanderJitter;
     const float                      mMinViewBoxLength;
     const float                      mInteractionRadius;
     const float                      mFeelerLength;
     const float                      mMinArriveDist;

     const float                      mObstacleAvoidanceMultiplier;
     const float                      mWallAvoidanceMultiplier;
//     const float                      mSeekMultiplier;
     const float                      mArriveMultiplier;
     const float                      mEvadeMultiplier;
     const float                      mWanderMultiplier;
     const float                      mSeperationMultiplier;
     const float                      mAlignmentMultiplier;
     const float                      mCohesionMultiplier;
     const float                      mFlockingMultiplier;

    const MovingEntity*                     mHost;
    float                                   mTheta;

    std::array<bool,
                Behaviour::NumBehaviour>    mBehaviourFlags;

    sf::Vector2f                            mWanderTarget;
    std::vector<sf::Vector2f>               mFeelers;
//    std::vector<bool>                       mFeelerCollisions;
//    bool                                    mObstacleCollision;

//    sf::RectangleShape                      mViewBox;
    sf::Vector2f                            mViewBoxEnd;

//    sf::VertexArray                         mLines;

    bool                                    accumulateForce(sf::Vector2f&, sf::Vector2f);
    void                                    createFeelers();

    sf::Vector2f                            rest();

    sf::Vector2f                            arrive(Deceleration);
    sf::Vector2f                            seek(sf::Vector2f);

    sf::Vector2f                            evade();
    sf::Vector2f                            flee(sf::Vector2f);

    sf::Vector2f                            pursuit();

    sf::Vector2f                            wander(sf::Time);

    sf::Vector2f                            obstacleAvoidance();
    sf::Vector2f                            wallAvoidance();

    sf::Vector2f                            seperation();
    sf::Vector2f                            alignment();
    sf::Vector2f                            cohesion();
    sf::Vector2f                            flocking();

public:
                                            SteeringBehaviour(const MovingEntity*
                                                              , Params&);

    sf::Vector2f                            calculate(sf::Time);
    void                                    setNewBehaviours(std::vector<SteeringBehaviour::Behaviour>);

//    sf::CircleShape                 wanderCircle() const { return mWanderCircle; }
//    sf::CircleShape                 targetLocal() const { return mLocalTarget; }
//    sf::CircleShape                 targetWorld() const { return mWorldTarget; }
//    std::vector<bool>               getFeelerCollisions(){ return mFeelerCollisions; }
//    bool                            getObstacleCollision(){ return mObstacleCollision; }
//    sf::RectangleShape              viewBox() const { return mViewBox; }
//    sf::VertexArray                 lines() const { return mLines; }

    bool                                    checkBehaviour(SteeringBehaviour::Behaviour type) const
                                            { return mBehaviourFlags.at(type); }
};

#endif // STEERINGBEHAVIOUR_HPP
