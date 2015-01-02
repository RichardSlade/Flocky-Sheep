#ifndef WALL_HPP
#define WALL_HPP

#include <stdexcept>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "SceneNode.hpp"

class Wall : public SceneNode
{
public:
    typedef std::unique_ptr<Wall> upWall;

private:
    sf::RectangleShape          mRect;
    sf::Vector2f                mPointA;
    sf::Vector2f                mPointB;
    sf::Vector2f                mNormal;

    virtual void                updateCurrent(sf::Time){};
    virtual void                drawCurrent(sf::RenderTarget& target
                                            , sf::RenderStates states) const
                                                                    { target.draw(mRect, states); }

public:

                                Wall(sf::Vector2f pos
                                     , sf::Vector2f rectSize
                                     , sf::Vector2f pointA
                                     , sf::Vector2f pointB
                                     , sf::Vector2f norm
                                     , const sf::Color colour)
                                : mRect(rectSize)
                                , mPointA(pointA)
                                , mPointB(pointB)
                                , mNormal(norm)
                                {
                                    setPosition(pos);
                                    mRect.setFillColor(colour);
                                };

    virtual                     ~Wall(){};

    // Getters
    sf::Vector2f                getNorm(){ return mNormal; }

    std::pair<sf::Vector2f
                , sf::Vector2f> getPoints(){ return std::pair<sf::Vector2f
                                                                , sf::Vector2f>(mPointA
                                                                                , mPointB); }

};

#endif // WALL_HPP
