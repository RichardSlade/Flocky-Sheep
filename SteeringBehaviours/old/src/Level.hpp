#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "LevelBlock.hpp"
#include "Wall.hpp"

class World;
class Controller;
class MovingEntity;

class Level
{
private:
    typedef std::vector<LevelBlock*> ArrayBlock;
    typedef std::vector<std::vector<LevelBlock*>> LevelArray;

    LevelArray                          mLevelArray;
    sf::IntRect                         mWorldBounds;
    int                                 mBlockSize;
    int                                 mLevelX;
    int                                 mLevelY;
    int                                 mMidX;

    std::array<Wall::WallData
                , Wall::NumWalls>       mBoundaryWallData;

    void                                defineWallData();
    sf::Vector2i                        worldCordsToIndex(sf::Vector2f) const;

    std::vector<LevelBlock*>            getInRangeBlocks(const MovingEntity*
                                                         , float) const;
public:
                                        Level(int
                                              , sf::IntRect);

    // Getters
    LevelBlock*                         getBlock(sf::Vector2i index) const
                                        { return mLevelArray.at(index.y).at(index.x); }

    std::vector<LevelBlock*>            getBlockTypeInRange(const MovingEntity*
                                                            , float
                                                            , LevelBlock::Type) const;

    std::vector<MovingEntity*>          getEntitiesInRange(const MovingEntity*
                                                           , float) const;

    // Setters
    void                                generateLevel(std::array<SceneNode*, SceneNode::Layers::Num>
                                                      , Controller&);

    LevelBlock*                         insertEntityIntoLevel(MovingEntity*) const;
};

#endif // LEVEL_HPP
