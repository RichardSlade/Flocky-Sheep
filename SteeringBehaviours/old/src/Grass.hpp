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
    static const float          mGrassGrowth;

    GrassType                   mGrassType;
    float                       mGrassLeft;

    void                        updateFaceColour();

public:
                                Grass(LevelBlock*
                                      , sf::Texture&);

    virtual                     ~Grass(){};

    // Getters
    virtual float               getSceneryData()
                                { return mGrassLeft; }

    // Setters
    virtual void                update();

    virtual void                changeSceneryData(float value)
                                { mGrassLeft -= value; }
};

#endif // GRASS_HPP
