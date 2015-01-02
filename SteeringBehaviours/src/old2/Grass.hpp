#ifndef GRASS_HPP
#define GRASS_HPP

#include <memory>

#include <SFML/Graphics/Color.hpp>

#include "Scenery.hpp"

class Grass : public Scenery
{
public:
    typedef std::unique_ptr<Grass> GrassPtr;

    enum GrassType
    {
        Healthy,
        Dying,
        Dead,
        NumGrassTypes
    };

private:
    static const float          mMaxGrass;

    GrassType                   mGrassType;
    float                       mGrassLeft;

    void                        updateFaceColour();

public:
                                Grass(LevelBlock*
                                      , const sf::Texture&);

    virtual                     ~Grass(){};
};

#endif // GRASS_HPP
