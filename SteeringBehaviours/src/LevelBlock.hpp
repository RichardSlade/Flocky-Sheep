#ifndef LEVELBLOCK_HPP
#define LEVELBLOCK_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "SceneNode.hpp"
#include "Scenery.hpp"

class LevelBlock : public SceneNode
{
public:

    enum Type
    {
        Nothing,
        Grass,
        Tree,
        Pen,
        Num
    };

private:
    Type                mType;
    sf::Vector2f        mBlockPos;
    float               mSize;

    Scenery             mScenery;

//    float               mItemRadius;
//    bool                mItemCollected;


//    sf::Sprite          mSprite;

    virtual void        drawCurrent(sf::RenderTarget&
                                    , sf::RenderStates) const;

    void                positionItem();

public:
                        LevelBlock(Type
                                   , sf::Texture&
                                   , sf::Vector2f
                                   , float
                                   , float);

                        LevelBlock(Type
                                   , sf::Vector2f
                                   , float);


    // Getters
    Scenery*            getScenery() { return &mScenery; }
    float               getSize(){ return mSize; }
    LevelBlock::Type    getType(){ return mType; }
//    sf::Vector2f        getItemWorldPosition();

};

#endif // LEVELBLOCK_HPP
