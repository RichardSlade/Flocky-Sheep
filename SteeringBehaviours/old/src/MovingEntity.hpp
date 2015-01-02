#ifndef MOVINGENTITY_HPP
#define MOVINGENTITY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "SceneNode.hpp"
#include "SteeringBehaviour.hpp"
#include "Utility.hpp"
#include "LevelBlock.hpp"
#include "EntityStats.hpp"
#include "MovingTarget.hpp"
#include "Params.hpp"
//#include "Wall.hpp"

class World;
//class Wall;

class MovingEntity : public SceneNode, public MovingTarget
{
protected:
//    static const float          mNeighbourhoodRadius;

    const World*                mWorld;

    const float                 mMass;
    const float                 mMaxSpeed;
    const float                 mMaxForce;
    const float                 mMaxTurnRate;

    LevelBlock*                 mCurrentBlock;
//    sf::Vector2i                mCurrentBlockIndex;

    sf::Sprite                  mSprite;
    sf::Vector2f                mVelocity;
    sf::Vector2f                mHeading;

    float                       mRadius;
    float                       mSpeed;

    SteeringBehaviour           mSteering;
    Target*                     mTarget;
    MovingTarget*               mMovingTarget;
    sf::Text                    mText;


    virtual void                updateCurrent(sf::Time);

    void                        adjustPosition();
    void                        ensureZeroOverlap();

public:
                                MovingEntity(const World*
                                             , sf::Texture&
                                             , sf::Font&
                                             , sf::Vector2f
                                             , EntityStats
                                             , Params&
                                             , float = 1.f);

    virtual                     ~MovingEntity(){};

    // Virtual Functions
    virtual sf::Vector2f        targetPosition() const
                                { return getWorldPosition(); }

    virtual sf::Vector2f        targetVelocity() const
                                { return getVelocity(); }

    virtual sf::Vector2f        targetHeading() const
                                { return getHeading(); }

    virtual float               targetSpeed() const
                                { return getSpeed(); }

    virtual float               targetRadius() const
                                { return getRadius(); }

    // Getters
    std::vector<MovingEntity*>  getNeighbours(float = 50.f) const;

    std::vector<LevelBlock*>    getBlockTypeInRange(LevelBlock::Type, float) const;

    sf::Vector2f                getVelocity() const
                                { return mVelocity; }

    sf::Vector2f                getHeading() const
                                { return mHeading; }

    float                       getSpeed() const
                                { return magVec(mVelocity); }

    float                       getMass() const
                                { return mMass; }

    float                       getMaxSpeed() const
                                { return mMaxSpeed; }

    float                       getMaxForce() const
                                { return mMaxForce; }

    float                       getMaxTurnRate() const
                                { return mMaxTurnRate; }

    bool                        checkSteeringBehaviour(SteeringBehaviour::Behaviour type) const
                                { return mSteering.checkBehaviour(type); }

    float                       getRadius() const
                                { return mRadius; }

    float                       getSpriteWidth() const
                                { return mSprite.getLocalBounds().width; }

    const Target*               getTarget() const
                                { return mTarget; }

    const MovingTarget*         getMovingTarget() const
                                { return mMovingTarget; }

    LevelBlock*                 getLevelBlock(sf::Vector2i);

    LevelBlock*                 getLevelBlock()
                                { return mCurrentBlock; }

//    sf::Vector2i                getLevelBlockIndex()
//                                { return mCurrentBlockIndex; }

    // Setters
    void                        setTarget(Target* target)
                                { mTarget = target; }

    void                        setMovingTarget(MovingTarget* mvTarget)
                                { mMovingTarget = mvTarget; }

    void                        setSpeed(float speed)
                                { mSpeed = speed; }

    void                        setSteeringTypes(std::vector<SteeringBehaviour::Behaviour> newBehaviours)
                                { mSteering.setNewBehaviours(newBehaviours); }

    void                        changeColour(sf::Color colour)
                                { mSprite.setColor(colour); }

    void                        setText(std::string msg)
                                {
                                    mText.setString(msg);
                                    mText.setColor(sf::Color(255, 255, 255, 255));
                                }
};

#endif // MOVINGENTITY_HPP
