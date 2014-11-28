#ifndef SCENERY_HPP
#define SCENERY_HPP

#include <stdexcept>
#include <memory>

//#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "SceneNode.hpp"
//#include "LevelBlock.hpp"

class LevelBlock;

class Scenery// : public SceneNode
{
public:
    //typedef std::unique_ptr<Obstacle> upObstacle;

private:
    const LevelBlock*       mHostBlock;
    sf::Sprite              mSprite;
    float                   mRadius;

    //virtual void                updateCurrent(sf::Time){};
    //virtual void                drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
                                //{ target.draw(mCircle, states); }

public:

                                Scenery(LevelBlock* host
                                        , sf::Texture& texture
                                        , float rad);

                                Scenery(LevelBlock* host);


//    virtual                     ~Scenery(){};

    // Getters
    float                       radius() const { return mRadius; }
    const sf::Sprite&           getSprite() const { return mSprite; }
    sf::Vector2f                getPosition() const;
    sf::Vector2f                getWorldPosition() const;

    // Setters
    void                        setPosition(float x, float y){ mSprite.setPosition(x, y); }
    //void                        changeColour(sf::Color colour){ mCircle.setFillColor(colour); }
};

#endif // SCENERY_HPP
