#ifndef LEVELBLOCK_HPP
#define LEVELBLOCK_HPP

#include <list>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "SceneNode.hpp"
#include "Scenery.hpp"
#include "Target.hpp"

class MovingEntity;

class LevelBlock : public SceneNode, public Target
{
public:
    typedef std::unique_ptr<LevelBlock> BlockPtr;

    enum Type
    {
        ExitBlock,
        GrassBlock,
        ObstacleBlock,
        WallBlock,
        CornerBlock,
        NumBlockTypes
    };

private:
    Type                        mType;
    Scenery::SceneryPtr         mScenery;
    std::list<MovingEntity*>    mEntitiesInBlock;

    sf::CircleShape             mOriginCircle;

    sf::Vector2f                mBlockCorner;
    sf::Vector2i                mBlockIndex;
    float                       mSize;
    float                       mRadius;

    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

    virtual void                updateCurrent(sf::Time);

public:
                                LevelBlock(sf::Vector2f
                                           , sf::Vector2i
                                           , float
                                           , float);

    virtual                     ~LevelBlock(){};

    // Getters
    virtual sf::Vector2f        targetPosition() const
                                { return getWorldPosition() + mOriginCircle.getPosition(); }

    virtual float               targetRadius() const
                                { return getRadius(); }

    Scenery*                    getScenery()
                                { return mScenery.get(); }

    std::list<MovingEntity*>    getEntities()
                                { return mEntitiesInBlock; }

    float                       getRadius() const
                                { return mRadius; }

    float                       getSize() const
                                { return mSize; }

    sf::Vector2i                getIndex()
                                { return mBlockIndex; }

    LevelBlock::Type            getType() const
                                { return mType; }

    // Setters
    LevelBlock*                 insertEntity(MovingEntity*);
    void                        deleteEntity(MovingEntity*);

    void                        addScenery(Scenery::SceneryPtr scenery)
                                { mScenery = std::move(scenery); }

    void                        setType(LevelBlock::Type newType)
                                { mType = newType; }
};

#endif // LEVELBLOCK_HPP
