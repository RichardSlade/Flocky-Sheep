#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "SceneNode.hpp"

//class SceneNode;

class SpriteNode : public SceneNode
{
private:
    sf::Sprite                  mSprite;

    virtual void                drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

public:
                                SpriteNode(sf::Texture& t
                                           , sf::Vector2f pos
                                           , sf::IntRect rect
                                           , bool resetOrigin = false)
                                : mSprite(t, rect)
                                {
                                    if(resetOrigin)
                                    {
                                        sf::FloatRect bounds = mSprite.getLocalBounds();
                                        mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                                    }

                                    mSprite.setPosition(pos);
                                };

};

#endif // SPRITENODE_HPP

