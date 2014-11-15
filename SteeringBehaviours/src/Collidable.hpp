#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

struct Collidable
{
public:
    enum Type
    {
        Wall,
        Target,
        Num
    };

    Type        type;
    bool        hasCollided;

                Collidable(Type t)
                : type(t)
                , hasCollided(false)
                {};
};

#endif // COLLIDABLE_HPP
