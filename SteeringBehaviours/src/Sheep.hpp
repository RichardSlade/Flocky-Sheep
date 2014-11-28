#ifndef SHEEP_HPP
#define SHEEP_HPP

#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "MovingEntity.hpp"
#include "SteeringBehaviour.hpp"

class World;
class Scenery;
class Wall;

class Sheep : public MovingEntity
{
private:
    const World*                mWorld;
    sf::Sprite                  mSprite;

    float                       mRadius;

//    SteeringBehaviours::Type    mSteeringType;
    SteeringBehaviour           mSteering;

    Target*                     mTarget;
    Target*                     mPursuer;

    virtual void                updateCurrent(sf::Time);
    virtual void                drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

    void                        handleCollisions();

public:
                                Sheep(const World*
                                        , sf::Texture&
                                        , sf::Vector2f
                                        , float
                                        , Target* = nullptr
                                        , Target* = nullptr);

     virtual                    ~Sheep(){};

    // Getters
    sf::Vector2f                targetPos() const{ return mTarget->position(); }
    Target*                     target() const { return mTarget; }
    Target*                     pursuer() const { return mPursuer; }
    float                       radius() const {return mRadius;}
    std::vector<Scenery*>       getObstaclesInRange() const;
    std::vector<Wall*>          getWalls() const;
    float                       getSpriteWidth() const { return mSprite.getLocalBounds().width; }
    bool                        checkSteeringBehaviour(SteeringBehaviour::Behaviour)
                                                        { return mSteering.checkBehaviour(type); }

    // Setters
//    void                        setCornerCollision(bool status){ mCornerCollision = status; }
    void                        setTarget(Target* target){ mTarget = target; }
    void                        setSteeringType(SteeringBehaviour::Behaviour newBehaviour)
                                                { mSteering.setNewBehaviour(newBehaviour); }

};

#endif // SHEEP_HPP
