#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "Level.hpp"
#include "SceneNode.hpp"
#include "Sheep.hpp"
#include "State.hpp"
#include "EntityStats.hpp"
//#include "Params.hpp"

class Controller;
class LevelBlock;
class Wall;
class Waypoint;
class MovingEntity;

class World : private sf::NonCopyable
{
public:
    static const float                          mWallWidth;

    enum StatsType
    {
        SheepStats,
        DogStats,
        StatsTypeNum
    };

    const int                            mLevelBlockSize;

private:
    const int                                   mWorldX;
    const int                                   mWorldY;
    const int                                   mNumSheep;
//    const int                                   mNumWalls;
//    const int                                   mNumObstacles;
//    const float                                 mObstacleRadius;
    const float                                 mWaypointRadius;
    const float                                 mScrollSpeed;

    Controller&                                 mController;
    sf::RenderWindow&                           mWindow;
    sf::View                                    mWorldView;
    sf::IntRect                                 mWorldBounds;
    sf::Vector2f                                mFocusPoint;

    SceneNode                                   mSceneGraph;
    std::array<SceneNode*
                , SceneNode::Layers::Num>       mSceneLayers;

    Level                                       mLevel;

    std::vector<EntityStats>                    mEntityStats;
    std::vector<std::unique_ptr<State<Sheep>>>  mSheepStates;
    std::vector<Waypoint*>                      mWaypoints;

    void                                        initialiseStatesAndStats();
    void                                        buildScene();
    void                                        generateAgents();
    void                                        handleRealTimeInput();

public:
                                                World(Controller&
                                                      , sf::RenderWindow&);

    void                                        update(sf::Time);
    void                                        handleInput();
    void                                        display();

    // Getters
    const sf::FloatRect                         getViewBounds() const;

    State<Sheep>*                               getSheepState(Sheep::States newStateType) const
                                                { return mSheepStates.at(newStateType).get(); }

    const sf::IntRect                           getWorldBounds() const
                                                { return mWorldBounds; }

    LevelBlock*                                 getLevelBlock(sf::Vector2i index) const
                                                { return mLevel.getBlock(index); }

    std::vector<LevelBlock*>                    getBlockTypeInRange(const MovingEntity*
                                                                    , LevelBlock::Type, float) const;

    // Setters
    LevelBlock*                                 insertEntityIntoLevel(MovingEntity*) const;

    std::vector<MovingEntity*>                  getEntitiesInRange(const MovingEntity*
                                                                   , float) const;
};

#endif // WORLD_HPP
