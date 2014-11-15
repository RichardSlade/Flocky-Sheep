#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <stdexcept>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Collidable.hpp"

class Obstacle : public Collidable, public SceneNode
{
public:
    typedef std::unique_ptr<Obstacle>   upObstacle;

private:
    sf::VertexArray             mVertices;
    sf::FloatRect               mBounds;

    void                        updateCurrent(sf::Time){};
    void                        drawCurrent(sf::RenderWindow& window){ window.draw(mVertices); }

public:

                                Obstacle(Collidable::Type colType
                                         , sf::PrimitiveType shapeType
                                         , std::vector<sf::Vector2f> verts
                                         , sf::Color colour)
                                : Collidable(colType)
                                , mVertices(shapeType)
                                {
                                    int numVerts = verts.size();

                                    if(numVerts < 3)
                                        throw std::runtime_error("Object: Not enough vertices");

                                    for(int i = 0; i < numVerts; i++)
                                    {
                                        mVertices.append(sf::Vertex(verts[i], colour));

                                        mBounds.left = verts[0].x;
                                        mBounds.top = verts[0].y;
                                        mBounds.width = verts[1].x - verts[0].x;
                                        mBounds.height = verts[2].y - verts[1].y;
                                    }
                                };

    // Getters
//    const sf::VertexArray&      vertices(){ return mVertices; }
};

#endif // OBSTACLE_HPP
