#include <iostream>
#include <cmath>
#include <vector>

#include "Sheep.hpp"
//#include "Dog.hpp"
#include "Utility.hpp"
#include "World.hpp"

const float Sheep::mHungerInc = 0.1f;
const float Sheep::mHungerDec = 0.1f;
const float Sheep::mFatigueInc = 0.01f;
const float Sheep::mFearInc = 0.1f;
const float Sheep::mSightRange = 250.f;
const float Sheep::mEatDistance = 25.f;
const float Sheep::mEatAmount = 1.f;
const float Sheep::mMaxHungerBeforeForage = 100.f;
const float Sheep::mMaxFatigueBeforeSleep = 100.f;
const float Sheep::mChanceToRest = 0.1f;
const float Sheep::mAngleOfVision = 180.f;
const float Sheep::mPanicDistance = Sheep::mSightRange;

const float Sheep::mSheepSpeed = 50.f;
const float Sheep::mSheepMaxRotation = 0.08f;

Sheep::Sheep(const World* world
                 , sf::Texture& texture
                 , sf::Font& font
                 , sf::Vector2f startPos
                 , State<Sheep>* globalState
                 , State<Sheep>* initState
                 , EntityStats stats
                 , Params& params
                 , float scale)
: MovingEntity(world
               , texture
               , font
               , startPos
               , stats
               , params
               , scale)
//, mCurrentBlock(nullptr)
//, mText("", font, 12)
, mStateMachine(this, globalState, initState)
, mHunger(rangedClamped(0.f, mMaxHungerBeforeForage))
, mFatigue(rangedClamped(0.f, mMaxFatigueBeforeSleep))
, mFear(0.f)
, mSleepCountDown(sf::Time::Zero)
{
    mText.setPosition(-10.f, -40.f);

//    mCurrentBlockIndex = mWorld->insertEntityIntoLevel(this);

    mCurrentBlock = mWorld->insertEntityIntoLevel(this);
}

void Sheep::updateCurrent(sf::Time dt)
{
    if(mSleepCountDown > sf::Time::Zero)
        mSleepCountDown -= dt;

    mStateMachine.update();

    MovingEntity::updateCurrent(dt);

    mCurrentBlock->deleteEntity(this);
    mCurrentBlock = mWorld->insertEntityIntoLevel(this);

    if(mCurrentBlock->getType() == LevelBlock::Type::ExitBlock)
    {
        mToRemove = true;
        mTarget = nullptr;
        mMovingTarget = nullptr;
        mCurrentBlock->deleteEntity(this);
    }
}

void Sheep::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);
//    target.draw(mSteering.wanderCircle(), states);
//    target.draw(mSteering.targetLocal(), states);
//    target.draw(mSteering.targetWorld(), states);
//    target.draw(mSteering.viewBox(), states);
//    target.draw(mSteering.lines(), states);
}

void Sheep::changeState(Sheep::States newState)
{
    mStateMachine.changeState(mWorld->getSheepState(newState));
}
