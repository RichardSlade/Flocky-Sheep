#ifndef SHEEPSTATES_HPP
#define SHEEPSTATES_HPP

struct Forage : public State<Sheep>
{
public:
//                        Forage(){};
    virtual             ~Forage(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

class ApproachFood : public State<Sheep>
{
public:
//                        Forage(){};
    virtual             ~ApproachFood(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

class Eat : public State<Sheep>
{
public:
//                        Forage(){};
    virtual             ~Eat(){};

    virtual void        enter(Sheep* sheep);

    virtual void        execute(Sheep* sheep);

    virtual void        exit(Sheep* sheep);
};

#endif // SHEEPSTATES_HPP
