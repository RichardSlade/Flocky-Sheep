#ifndef SHEEPSTATES_HPP
#define SHEEPSTATES_HPP

#include "State.hpp"
#include "Sheep.hpp"

struct LookOutForDog : public State<Sheep>
{
public:
    virtual             ~LookOutForDog(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

struct EvadeDog : public State<Sheep>
{
public:
    virtual             ~EvadeDog(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

struct Sleep : public State<Sheep>
{
public:
    virtual             ~Sleep(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

struct Relax : public State<Sheep>
{
public:
    virtual             ~Relax(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

struct Forage : public State<Sheep>
{
public:
    virtual             ~Forage(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

class ApproachFood : public State<Sheep>
{
public:
    virtual             ~ApproachFood(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

class Eat : public State<Sheep>
{
public:
    virtual             ~Eat(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

#endif // SHEEPSTATES_HPP
