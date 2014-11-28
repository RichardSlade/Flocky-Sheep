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

class Sheep;
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
        Seek,
        Arrive,
        Flee,
        Pursuit,
        Wander,
        ObstacleAvoidance,
        WallAvoidance,
        Num
    };

    enum Feelers
    {
        Left,
        Front,
        Right,
        NumFlr
    };

    static const float              mPI;
    static const float              mWanderRadius;
    static const float              mWanderDistance;
    static const float              mWanderJitter;
    static const float              mMinViewBoxLength;
    static const float              mObstacleAvoidanceMultiplier;
    static const float              mWallAvoidanceMultiplier;
    static const float              mSeekMultiplier;
    static const float              mWanderMultiplier;
    static const float              mFeelerLength;

private:
    const Sheep*                    mHost;
    float                           mTheta;

    std::array<bool,
                Behaviour::Num>     mBehaviourFlags;
    sf::Vector2f                    mWanderTarget;
    std::vector<sf::Vector2f>       mFeelers;
    std::vector<bool>               mFeelerCollisions;
    bool                            mObstacleCollision;

//    sf::CircleShape                 mWanderCircle;
//    sf::CircleShape                 mLocalTarget;
//    sf::CircleShape                 mWorldTarget;
    sf::RectangleShape              mViewBox;
    sf::Vector2f                    mViewBoxEnd;

    sf::VertexArray                 mLines;

    bool                            accumulateForce(sf::Vector2f&, sf::Vector2f);
    void                            createFeelers();

    sf::Vector2f                    seek(sf::Vector2f);
    sf::Vector2f                    arrive(sf::Vector2f, Deceleration);
    sf::Vector2f                    evade(const Sheep*);
    sf::Vector2f                    flee(sf::Vector2f);
    sf::Vector2f                    pursuit(const Target*);
    sf::Vector2f                    wander(sf::Time);
    sf::Vector2f                    obstacleAvoidance();
    sf::Vector2f                    wallAvoidance();

public:
                                    SteeringBehaviour(const Sheep*);

    sf::Vector2f                    calculate(sf::Time);
    void                            setNewBehaviour(SteeringBehaviour::Behaviour);

//    sf::CircleShape                 wanderCircle() const { return mWanderCircle; }
//    sf::CircleShape                 targetLocal() const { return mLocalTarget; }
//    sf::CircleShape                 targetWorld() const { return mWorldTarget; }
    std::vector<bool>               getFeelerCollisions(){ return mFeelerCollisions; }
    bool                            getObstacleCollision(){ return mObstacleCollision; }
    sf::RectangleShape              viewBox() const { return mViewBox; }
    sf::VertexArray                 lines() const { return mLines; }
    bool                            checkBehaviour(SteeringBehaviour::Behaviour type)
                                                    { return mBehaviourFlags.at(type); }
};

#endif // STEERINGBEHAVIOUR_HPP
