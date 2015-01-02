#ifndef SCENERY_HPP
#define SCENERY_HPP

#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SceneNode.hpp"
//#include "Target.hpp"

class LevelBlock;

class Scenery
{
public:
    typedef std::unique_ptr<Scenery> SceneryPtr;

protected:
    static const sf::Color      mBrown;
    static const sf::Color      mYellow;
    static const sf::Color      mGreen;

    const LevelBlock*           mHostBlock;
    sf::Sprite                  mSprite;
//    sf::RectangleShape          mBlockFace;
    float                       mGrassLeft;

public:
                                Scenery(LevelBlock*
                                        , sf::Texture&);

    virtual                     ~Scenery(){};

    // Getters
    virtual void                getSceneryData() const {};

//    const sf::RectangleShape&   getBlockFace() const
//                                { return mBlockFace; }

    const sf::Sprite&           getSprite()
                                { return mSprite; }

    // Setters
    virtual void                update(){};
    virtual void                changeSceneryData(float){};

    void                        rotateSprite(float angle)
                                { mSprite.rotate(angle); }
};

#endif // SCENERY_HPP
