#include "LevelBlock.hpp"
#include "Utility.hpp"

LevelBlock::LevelBlock(LevelBlock::Type type
                       , sf::Texture& texture
                       , sf::Vector2f pos
                       , float size
                       , float scale)
: mType(type)
, mBlockPos(pos)
, mSize(size)
, mScenery(this, texture, scale)
{
    setPosition(pos);
    positionItem();
}

LevelBlock::LevelBlock(LevelBlock::Type type
                       , sf::Vector2f pos
                       , float size)
: mType(type)
, mBlockPos(pos)
, mSize(size)
, mScenery(this)
{
    setPosition(pos);
}

void LevelBlock::drawCurrent(sf::RenderTarget& target
                             , sf::RenderStates states) const
{
    target.draw(mScenery.getSprite(), states);
}

void LevelBlock::positionItem()
{
    mScenery.setPosition(rangedClamped(0.f, mSize)
                     , rangedClamped(0.f, mSize));
}
