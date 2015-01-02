#include <cassert>
#include <iostream>

#include "SheepStates.hpp"
//#include "Dog.hpp"
#include "SteeringBehaviour.hpp"
//#include "LevelBlock.hpp"
#include "Utility.hpp"
#include "Grass.hpp"
#include "MovingTarget.hpp"

void LookOutForDog::enter(Sheep* sheep)
{
    assert(sheep);
}

void LookOutForDog::execute(Sheep* sheep)
{
    const MovingTarget* dog = sheep->getMovingTarget();

    if(dog)
    {
        sf::Vector2f dogPos = dog->targetPosition();
        sf::Vector2f vecToDog = sheep->getWorldPosition() - dogPos;

        float mag = magVec(vecToDog);

        if(mag <= Sheep::mPanicDistance
           && !sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
            sheep->changeState(Sheep::States::Evade);
    }
}

void LookOutForDog::exit(Sheep* sheep)
{

}

void EvadeDog::enter(Sheep* sheep)
{
    assert(sheep);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);

    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

    sheep->setSteeringTypes(behaviours);
    sheep->setText("!!!!");
}

void EvadeDog::execute(Sheep* sheep)
{
    if(sheep->getMovingTarget())
    {
        sf::Vector2f dogPos = sheep->getMovingTarget()->targetPosition();
        sf::Vector2f vecToDog = sheep->getWorldPosition() - dogPos;

        float mag = magVec(vecToDog);

        if(mag > Sheep::mPanicDistance)
        {
            sheep->returnToPreviousState();
        }
    }
}

void EvadeDog::exit(Sheep* sheep)
{

}

void Sleep::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Rest))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Rest);

        sheep->setSteeringTypes(behaviours);
    }

    sheep->setVelocity(sf::Vector2f());
    sheep->resetSleepCountDown();
    sheep->setText("ZzzZz");
}

void Sleep::execute(Sheep* sheep)
{
    sheep->incHunger();

//    if(sheep->getHunger() > Sheep::mMinHungerBeforeForage)
//    {
//        sheep->changeState(Sheep::States::Forage);
//    }
    if(sheep->getSleepCountDown() < sf::Time::Zero)
    {
        sheep->changeState(Sheep::States::Relax);
    }
}

void Sleep::exit(Sheep* sheep)
{
    sheep->zeroSleepCountDown();
    sheep->zeroFatigue();
}

void Relax::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);
        behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        sheep->setSteeringTypes(behaviours);
    }

    sheep->setText("Baaah");
}

void Relax::execute(Sheep* sheep)
{
//    sheep->incHunger();
//    sheep->incFatigue();

//    if(sheep->getHunger() > Sheep::mMaxHungerBeforeForage)
//    {
//        sheep->changeState(Sheep::States::Forage);
//    }
//    else if(sheep->getFatigue() > Sheep::mMaxFatigueBeforeSleep)
//    {
//        sheep->changeState(Sheep::States::Sleep);
//    }
}

void Relax::exit(Sheep* sheep)
{

}

void Forage::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        sheep->setSteeringTypes(behaviours);
    }

    sheep->setText("Maaah!");
}

void Forage::execute(Sheep* sheep)
{
    sheep->incFatigue();

    std::vector<LevelBlock*> inRangeGrassBlocks = sheep->getBlockTypeInRange(LevelBlock::GrassBlock, Sheep::mSightRange);

//    float distToClosestGrass = MAXFLOAT;
    float mostGrassSoFar = 0.f;
    LevelBlock* healthiestGrass = nullptr;

    for(LevelBlock* block : inRangeGrassBlocks)
    {
        Grass* grass = dynamic_cast<Grass*>(block->getScenery());

        assert(grass
               && "ERROR: Attempting to downcast Scenery to Grass");

        sf::Vector2f grassPosTrans = sheep->getInverseTransform() * block->getPosition();

        if(grassPosTrans.y <= 0.f)
        {
            sf::Vector2f grassPos = block->getWorldPosition();
            sf::Vector2f vecToGrass = grassPos - sheep->getWorldPosition();

            float mag = magVec(vecToGrass);

            if(grass->getSceneryData() > mostGrassSoFar //mag < distToClosestGrass
               && mag < Sheep::mSightRange)
            {
//                closestGrass = grass;
                healthiestGrass = block;
                mostGrassSoFar = grass->getSceneryData();
            }
        }
    }

    if(healthiestGrass)
    {
        sheep->setTarget(healthiestGrass);
        sheep->setTargetBlockIndex(healthiestGrass->getIndex());
        sheep->changeState(Sheep::States::ApproachFood);
    }
}

void Forage::exit(Sheep* sheep)
{

}

void ApproachFood::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);

        sheep->setSteeringTypes(behaviours);
    }

//    if(!sheep->getTarget())
//        sheep->changeState(Sheep::States::Forage);

    sheep->setText("Aaaa!");
}

void ApproachFood::execute(Sheep* sheep)
{
    const Target* target = sheep->getTarget();

    if(target)
    {
        sf::Vector2f toTarget(target->targetPosition() - sheep->getWorldPosition());

        float mag = magVec(toTarget);

        if(mag < 2.f)
            sheep->changeState(Sheep::States::Eat);
    }
}

void ApproachFood::exit(Sheep* sheep)
{
    sheep->setTarget(nullptr);
}

void Eat::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Rest))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Rest);

        sheep->setSteeringTypes(behaviours);
    }

    sheep->setVelocity(sf::Vector2f());
    sheep->setText("*Munch");
}

void Eat::execute(Sheep* sheep)
{
    sheep->setVelocity(sf::Vector2f());

//    sf::Vector2i blockIndex = sheep->getLevelBlockIndex();
//    LevelBlock* lvlBlock = sheep->getLevelBlock();

    LevelBlock* lvlBlock = sheep->getTargetBlock();

//    if(lvlBlck->getType() == LevelBlock::Type::Grass)

    sheep->decHunger(Sheep::mHungerDec);
    lvlBlock->getScenery()->changeSceneryData(Sheep::mEatAmount);

    if(lvlBlock->getType() != LevelBlock::Type::GrassBlock)
    {
        sheep->changeState(Sheep::States::Forage);
    }
    if(sheep->getHunger() <= 0.f)
    {
        sheep->changeState(Sheep::States::Relax);
    }
}

void Eat::exit(Sheep* sheep)
{

}
