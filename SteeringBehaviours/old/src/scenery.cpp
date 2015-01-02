#include "Scenery.hpp"
#include "LevelBlock.hpp"

const sf::Color Scenery::mBrown = sf::Color(150, 70, 0, 255);
const sf::Color Scenery::mYellow = sf::Color(200, 200, 0, 255);
const sf::Color Scenery::mGreen = sf::Color(0, 150, 0, 255);

Scenery::Scenery(LevelBlock* host
                 , sf::Texture& texture)
: mHostBlock(host)
//, mBlockFace(sf::Vector2f(mHostBlock->getSize()
//                          , mHostBlock->getSize()))
, mSprite(texture)
{
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

//    float blockSize = mHostBlock->getSize();
//    mBlockFace.setOrigin(-blockSize / 2.f, -blockSize / 2.f);
//    mBlockFace.setOrigin(mHostBlock->getOrigin());

//    sf::FloatRect bounds = mBlockFace.getLocalBounds();
//    mBlockFace.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//    mBlockFace.setPosition(mHostBlock->getPosition());
};
