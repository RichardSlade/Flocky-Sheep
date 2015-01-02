#ifndef DOG_HPP
#define DOG_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "MovingEntity.hpp"

class Dog : public MovingEntity
{
private:
//    static const float          mDogSpeed;
    const float                 mDogMaxRotation;

    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                Dog(const World*
                                    , sf::Texture&
                                    , sf::Font&
                                    , sf::Vector2f
                                    , EntityStats
                                    , Params&);

    virtual                     ~Dog(){};
};

#endif // DOG_HPP
