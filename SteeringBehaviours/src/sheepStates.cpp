#include <cassert>

#include "SheepStates.hpp"
#include "SteeringBehaviour.hpp"

void Forage::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        sheep->setSteeringType(SteeringBehaviour::Behaviour::Wander);
    }

}

void Forage::execute(Sheep* sheep)
{

}

void Forage::exit(Sheep* sheep)
{

}

void ApproachFood::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        sheep->setSteeringType(SteeringBehaviour::Behaviour::Wander);
    }

}

void ApproachFood::execute(Sheep* sheep)
{

}

void ApproachFood::exit(Sheep* sheep)
{

}

void Eat::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        sheep->setSteeringType(SteeringBehaviour::Behaviour::Wander);
    }

}

void Eat::execute(Sheep* sheep)
{

}

void Eat::exit(Sheep* sheep)
{

}
