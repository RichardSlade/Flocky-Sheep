#include <iostream>
#include <cassert>

#include "LevelBlock.hpp"
#include "Utility.hpp"

LevelBlock::LevelBlock(sf::Vector2f pos
                       , sf::Vector2i index
                       , float size
                       , float rad)
: mType(LevelBlock::Type::GrassBlock)
, mScenery(nullptr)
, mOriginCircle(5.f)
, mBlockCorner(pos)
, mBlockIndex(index)
, mSize(size)
, mRadius(rad)
{
    sf::FloatRect bounds = mOriginCircle.getLocalBounds();
    mOriginCircle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

//    setOrigin(mSize / 2.f, mSize / 2.f);
    mOriginCircle.setPosition(mSize / 2.f, mSize / 2.f);
//    mOriginCircle.setPosition(pos.x + size / 2.f, pos.y + size / 2.f);
//    setPosition(pos.x + size / 2.f, pos.y + size / 2.f);
    setPosition(pos);
}

void LevelBlock::drawCurrent(sf::RenderTarget& target
                             , sf::RenderStates states) const
{
//    if(mScenery)
//        target.draw(mScenery->getBlockFace(), states);
        target.draw(mScenery->getSprite(), states);

//    target.draw(mOriginCircle, states);
}

void LevelBlock::updateCurrent(sf::Time dt)
{
    if(mScenery)
        mScenery->update();
}

LevelBlock* LevelBlock::insertEntity(MovingEntity* entity)
{
    mEntitiesInBlock.push_back(entity);
    return this;
}

void LevelBlock::deleteEntity(MovingEntity* entity)
{
    auto it = find (mEntitiesInBlock.begin(), mEntitiesInBlock.end(), entity);

    assert(it != mEntitiesInBlock.end());

    mEntitiesInBlock.erase(it);
}
