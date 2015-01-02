#ifndef SHEEP_HPP
#define SHEEP_HPP

#include <memory>

//#include <SFML/Graphics/Text.hpp>

#include "MovingEntity.hpp"
#include "StateMachine.hpp"
//#include "State.hpp"
//#include "Utility.hpp"

class Sheep : public MovingEntity
{
public:
    enum States
    {
        LookOut,
        Evade,
        Sleep,
        Relax,
        Forage,
        ApproachFood,
        Eat,
        NumSheepStates
    };

//    static const float          mHungerInc;
//    static const float          mHungerDec;
//    static const float          mFatigueInc;
//    static const float          mFearInc;
    static const float          mSightRange;
//    static const float          mEatDistance;
//    static const float          mEatAmount;
//    static const float          mMaxHungerBeforeForage;
//    static const float          mMaxFatigueBeforeSleep;
//    static const float          mChanceToRest;
    static const float          mAngleOfVision;
    static const float          mPanicDistance;

private:

//    static const float          mSheepSpeed;
    static const float          mSheepMaxRotation;

//    LevelBlock*                 mCurrentBlock;
    sf::Vector2i                mTargetBlockIndex;
//    sf::Text                    mText;
    StateMachine<Sheep>         mStateMachine;

    float                       mHunger;
    float                       mFatigue;
    float                       mFear;

    sf::Time                    mSleepCountDown;

    virtual void                updateCurrent(sf::Time);
    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                Sheep(const World*
                                      , sf::Texture&
                                      , sf::Font&
                                      , sf::Vector2f
                                      , State<Sheep>*
                                      , State<Sheep>*
                                      , EntityStats
                                      , Params&
                                      , float = 1.f);

    virtual                    ~Sheep(){};

    // Getters
    float                       getHunger() const
                                { return mHunger; }

    float                       getFatigue() const
                                { return mFatigue; }

    float                       getFear() const
                                { return mFear; }

    sf::Time                    getSleepCountDown()
                                { return mSleepCountDown; }

    LevelBlock*                 getTargetBlock()
                                { return getLevelBlock(mTargetBlockIndex); }

    // Setters
    void                        setVelocity(sf::Vector2f vel)
                                { mVelocity = vel; }

    void                        setHunger(float newValue)
                                { mHunger = newValue; }

    void                        incHunger()
                                { mHunger += mHungerInc; }

    void                        decHunger(float value)
                                { mHunger -= value; }

    void                        incFatigue()
                                { mFatigue += mFatigueInc; }

    void                        zeroFatigue()
                                { mFatigue = 0.f; }

    void                        resetSleepCountDown()
                                { mSleepCountDown = sf::seconds(rangedClamped(5.f, 10.f)); }

    void                        zeroSleepCountDown()
                                { mSleepCountDown = sf::Time::Zero; }

    void                        setText(std::string msg)
                                {
                                    mText.setString(msg);
                                    mText.setColor(sf::Color(255, 255, 255, 255));
                                }

    void                        setTargetBlockIndex(sf::Vector2i index)
                                { mTargetBlockIndex = index; }

    void                        returnToPreviousState()
                                { mStateMachine.returnToPreviousState(); }

    void                        changeState(Sheep::States newState);
};

#endif // SHEEP_HPP
