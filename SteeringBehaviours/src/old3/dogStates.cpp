#include <cassert>
#include <iostream>
#include <string>

#include "DogStates.hpp"

void LookOutForWaypoint::enter(Dog* dog)
{
    assert(dog);
}

void LookOutForWaypoint::execute(Dog* dog)
{
//    if(dog->getCurrentWaypointIndex() == -1
//       && dog->getWaypoints().size() > 0)
//    {
//        dog->setCurrentWaypointIndex(0);
//        dog->setTarget(dog->getWaypoints().at(0));
//        dog->changeState(Dog::States::Waypoints);
//    }
}

void LookOutForWaypoint::exit(Dog* dog)
{

}

void FollowWaypoints::enter(Dog* dog)
{
    assert(dog);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!dog->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);

    dog->setSteeringTypes(behaviours);
}

void FollowWaypoints::execute(Dog* dog)
{
    int wypntIndex = dog->getCurrentWaypointIndex();

    std::vector<Waypoint*>& wypnts = dog->getWaypoints();

//    if(wypntIndex > wypnts.size() - 1)
//        throw std::runtime_error("Error:\nindex out of bounds: index: "
//                                 + std::to_string(wypntIndex)
//                                 + "size: "
//                                 + std::to_string(wypnts.size()));

    if(!wypnts.at(wypntIndex))
    {
        std::cout << "No target" << std::endl;
        dog->setCurrentWaypointIndex(-1);
        dog->setTarget(nullptr);
        dog->changeState(Dog::States::DogRelax);
    }
    else
    {
        sf::Vector2f vecToTarg = dog->getTarget()->targetPosition() - dog->getWorldPosition();

        float targMag = magVec(vecToTarg);
//        float velMag = magVec(dog->getVelocity());

        if(targMag < 10.f)
           //&& velMag < 0.1f)
        {
//            wypnts.at(wypntIndex)->setToRemove(true);

            wypnts.at(wypntIndex)->setReached(true);
            std::vector<Waypoint*>::iterator it = wypnts.begin();

            it += wypntIndex;
            wypnts.erase(it);

            int vecSize = wypnts.size();

            wypntIndex ++;

            if(vecSize - 1 < wypntIndex)
            {
                wypntIndex = -1;
                dog->setTarget(nullptr);
                dog->changeState(Dog::States::DogRelax);
            }
            else
                dog->setTarget(wypnts.at(wypntIndex));

            dog->setCurrentWaypointIndex(wypntIndex);

//            if(dog->getWaypoints().size() - 2 > dog->getCurrentWaypointIndex())
//            {
//                dog->nextWaypoint();
//                dog->setTarget(dog->getWaypoints().at(dog->getCurrentWaypointIndex()));
//            }
//            else
//            {
//                dog->setCurrentWaypointIndex(-1);
//                dog->setTarget(nullptr);
//                dog->changeState(Dog::States::DogRelax);
//            }
        }
    }
}

void FollowWaypoints::exit(Dog* dog)
{

}

void DogRelax::enter(Dog* dog)
{
    assert(dog);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!dog->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Rest))
        behaviours.push_back(SteeringBehaviour::Behaviour::Rest);

    dog->setSteeringTypes(behaviours);
}

void DogRelax::execute(Dog* dog)
{

}

void DogRelax::exit(Dog* dog)
{

}

