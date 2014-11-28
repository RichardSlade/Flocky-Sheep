#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

struct Collidable
{
protected:
    float       mRadius;

public:
    enum Type
    {
        Wall,
        Obstacle,
        Entity,
        Num
    };

    Type        mType;

                Collidable(Type t, float r = 0.f)
                : mRadius(r)
                , mType(t)
                {};

    // Getters
    float       radius() const { return mRadius; }
};

#endif // COLLIDABLE_HPP
