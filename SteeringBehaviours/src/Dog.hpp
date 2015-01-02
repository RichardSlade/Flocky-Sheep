#ifndef DOG_HPP
#define DOG_HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "MovingEntity.hpp"
//#include "Waypoint.hpp"
#include "Path.hpp"
//#include "StateMachine.hpp"

class Dog : public MovingEntity
{
public:
    enum States
    {
        LookOut,
        Waypoints,
        DogRelax,
        NumDogStates
    };

private:
//    const float                     mWaypointRadius;
//    std::vector<Waypoint*>&     mWaypoints;
//    std::vector<Waypoint::WypntPtr> mWaypoints;
//    Path                            mPath;

//    StateMachine<Dog>               mStateMachine;

    virtual void                    updateCurrent(sf::Time);
    virtual void                    drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                    Dog(World*
                                        , const sf::Texture&
                                        , const sf::Font&
                                        , sf::Vector2f
//                                        , State<Dog>*
//                                        , State<Dog>*
                                        , EntityStats
                                        , const Params&);

    virtual                         ~Dog(){};

    void                            addToPath(sf::Vector2f pos)
                                    { mSteering.addToPath(pos); }

    void                            startNewPath(sf::Vector2f pos)
                                    { mSteering.startNewPath(pos); }

    // Getters
//    std::vector<Waypoint::WypntPtr> getWaypoints()
//                                    { return mWaypoints; }

//    int                             getCurrentWaypointIndex()
//                                    { return mCurrentWaypointIndex; }

    // Setters
//    void                            changeState(Dog::States newState);

//    void                            setCurrentWaypointIndex(int index)
//                                    { mCurrentWaypointIndex = index; }
//
//    void                            nextWaypoint()
//                                    { mCurrentWaypointIndex ++; }
};

#endif // DOG_HPP
