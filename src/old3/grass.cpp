//#include "Grass.hpp"
#include "BlockType.hpp"
#include "Utility.hpp"

const float Grass::mMaxGrass = 1000.f;

Grass::Grass(LevelBlock* host
             , const sf::Texture& texture)
: Scenery(host
          , texture)
, mGrassLeft(rangedClamped(mMaxGrass / 1.5f, mMaxGrass))
{
    if(mGrassLeft <= 0.f)
    {
        mSprite.setColor(mBrown);
        mGrassType = GrassType::Dead;
        mGrassLeft = 0.f;
    }
    else if(mGrassLeft < mMaxGrass / 2.f)
    {
        mGrassType = GrassType::Dying;
    }
    else
        mGrassType = GrassType::Healthy;

    updateFaceColour();
}

void Grass::updateFaceColour()
{
    sf::Color bottomColour, topColour, newColour;
    float maxGrass, minGrass;

    switch(mGrassType)
    {
        case GrassType::Healthy:
        {
            bottomColour = mYellow;
            topColour = mGreen;
            maxGrass = mMaxGrass;
            minGrass = mMaxGrass / 2.f;
            break;
        }
        case GrassType::Dying:
        {
            bottomColour = mBrown;
            topColour = mYellow;
            maxGrass = mMaxGrass / 2.f;
            minGrass = 0.f;
            break;
        }
        default: break;
    }

    float topPercentage = (mGrassLeft - minGrass) / maxGrass;
    float botPercentage = 1.f - topPercentage;

    newColour.r = (topColour.r * topPercentage) + (bottomColour.r * botPercentage);
    newColour.g = (topColour.g * topPercentage) + (bottomColour.g * botPercentage);
    newColour.b = 0.f;

    mSprite.setColor(newColour);
}

