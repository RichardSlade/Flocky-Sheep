#ifndef BLOCKTYPE_HPP
#define BLOCKTYPE_HPP

#include <memory>

#include <SFML/Graphics/Color.hpp>

#include "Scenery.hpp"

class Corner : public Scenery
{
public:
    typedef std::unique_ptr<Corner> CornerPtr;

                                Corner(LevelBlock* host
                                       , const sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {};

//    virtual                     ~Corner(){};
//
//    // Getters
//    virtual void                getSceneryData(){};
};

class Exit : public Scenery
{
public:
    typedef std::unique_ptr<Exit> ExitPtr;

                                Exit(LevelBlock* host
                                     , const sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {};

//    virtual                     ~Exit(){};
//
//    // Getters
//    virtual void                getSceneryData(){};
};

class Grass : public Scenery
{
public:
    typedef std::unique_ptr<Grass> GrassPtr;
//
//    enum GrassType
//    {
//        Healthy,
//        Dying,
//        Dead,
//        NumGrassTypes
//    };

private:
//    static const float          mMaxGrass;

//    GrassType                   mGrassType;
//    float                       mGrassLeft;

//    void                        updateFaceColour();

public:
                                Grass(LevelBlock* host
                                      , const sf::Texture& texture)
                                : Scenery(host
                                          , texture)
                                {

                                };

//    virtual                     ~Grass(){};
};

class Wall : public Scenery
{
public:
    typedef std::unique_ptr<Wall> WallPtr;

    typedef std::pair<sf::Vector2f, sf::Vector2f> PointPair;
    typedef std::pair<PointPair, sf::Vector2f> WallData;

    enum WallType
    {
        LeftWall,
        RightWall,
        TopWall,
        TopLeftWall,
        TopRightWall,
        BottomWall,
        NumWalls
    };

private:
    sf::Vector2f                    mPointA;
    sf::Vector2f                    mPointB;
    sf::Vector2f                    mNormal;

    WallData                        retrieveWallData();

public:

                                    Wall(LevelBlock*
                                         , const sf::Texture&
                                         , sf::Vector2f
                                         , sf::Vector2f
                                         , sf::Vector2f);

    virtual                         ~Wall(){};

    // Getters
    virtual WallData                getSceneryData()
                                    { return retrieveWallData(); };
};

class Obstacle : public Scenery
{
public:
    typedef std::unique_ptr<Obstacle> ObstaclePtr;

                                Obstacle(LevelBlock* host
                                         , const sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {
                                };

    virtual                     ~Obstacle(){};

    // Getters
    virtual void                getSceneryData(){};
};


#endif // BLOCKTYPE_HPP
