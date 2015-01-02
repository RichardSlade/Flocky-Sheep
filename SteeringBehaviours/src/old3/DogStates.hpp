#ifndef DOGSTATES_HPP
#define DOGSTATES_HPP

#include "Dog.hpp"
#include "State.hpp"

struct LookOutForWaypoint : public State<Dog>
{
public:
    virtual             ~LookOutForWaypoint(){};

    virtual void        enter(Dog*);

    virtual void        execute(Dog*);

    virtual void        exit(Dog*);
};

struct FollowWaypoints : public State<Dog>
{
public:
    virtual             ~FollowWaypoints(){};

    virtual void        enter(Dog*);

    virtual void        execute(Dog*);

    virtual void        exit(Dog*);
};

struct DogRelax : public State<Dog>
{
public:
    virtual             ~DogRelax(){};

    virtual void        enter(Dog*);

    virtual void        execute(Dog*);

    virtual void        exit(Dog*);
};

#endif // DOGSTATES_HPP
