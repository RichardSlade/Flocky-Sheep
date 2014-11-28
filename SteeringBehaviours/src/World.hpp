#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "SceneNode.hpp"

class Controller;
class LevelBlock;
class Sheep;
class Scenery;
class Wall;
class StationaryTarget;

class World : private sf::NonCopyable
{
public:
    typedef std::unique_ptr<World>  upWorld;

private:
    static const int                        mLevelDivision;
    static const int                        mNumSheep;
    static const int                        mNumWalls;
    static const int                        mNumObstacles;
    static const float                      mObstacleRadius;
    static const float                      mTargetRadius;
    static const float                      mWallWidth;
    static const float                      mScrollSpeed;

    sf::Font                                mFont;

    Controller&                             mController;
    sf::RenderWindow&                       mWindow;
    sf::View                                mWorldView;
    sf::IntRect                             mWorldBounds;
    sf::Vector2f                            mFocusPoint;

    SceneNode                               mSceneGraph;
    std::array<SceneNode*
                , SceneNode::Layers::Num>   mSceneLayers;

    std::array<std::array<LevelBlock*
                         , 8>
                         , 8>               mLevel;

    std::vector<Wall*>                      mWalls;
    std::vector<Sheep*>                     mSheep;
    std::vector<StationaryTarget*>          mTargets;

    void                                    buildScene();
    void                                    generateLevel();
    void                                    generateWalls();
    void                                    generateAgents();
    void                                    generateObstacles();
    void                                    createObstacle(sf::Vector2f);
    void                                    handleRealTimeInput();

public:
                                            World(Controller&, sf::RenderWindow&);

    void                                    update(sf::Time);
    void                                    handleInput();
    void                                    display();

    std::vector<Scenery*>                   obstaclesInRange(Sheep*, float) const;
    std::vector<Wall*>                      getWorldWalls() const { return mWalls; }

    sf::FloatRect                           getViewBounds();
};

#endif // WORLD_HPP
