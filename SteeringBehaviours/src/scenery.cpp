#include "Scenery.hpp"
#include "LevelBlock.hpp"

Scenery::Scenery(LevelBlock* host
        , sf::Texture& texture
        , float scale)
: mHostBlock(host)
, mSprite(texture)
{
    mSprite.scale(scale, scale);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    mRadius = std::max(bounds.width, bounds.height);
};

Scenery::Scenery(LevelBlock* host)
: mHostBlock(host)
, mRadius(0.f)
{

};

sf::Vector2f Scenery::getPosition() const
{
    return mHostBlock->getPosition() + mSprite.getPosition();
}

sf::Vector2f Scenery::getWorldPosition() const
{
    return mHostBlock->getWorldPosition() + mSprite.getPosition();
}
