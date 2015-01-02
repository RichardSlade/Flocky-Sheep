#include "Wall.hpp"

Wall::Wall(LevelBlock* host
           , sf::Texture& texture
           , sf::Vector2f pointA
           , sf::Vector2f pointB
           , sf::Vector2f norm)
: Scenery(host
          , texture)
, mPointA(pointA)
, mPointB(pointB)
, mNormal(norm)
{

}

Wall::WallData Wall::retrieveWallData()
{
    PointPair points(mPointA, mPointB);

    return WallData(points, mNormal);
}
