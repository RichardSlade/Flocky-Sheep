#include <iostream>
#include <list>
#include <string>

#include "Level.hpp"
#include "MovingEntity.hpp"
#include "Utility.hpp"
#include "SceneNode.hpp"
#include "World.hpp"
#include "Controller.hpp"
#include "Grass.hpp"
#include "Exit.hpp"
#include "Wall.hpp"
#include "Corner.hpp"

Level::Level(int blockSize
             , sf::IntRect worldBounds)
: mWorldBounds(worldBounds)
, mBlockSize(blockSize)
, mLevelX(mWorldBounds.width / mBlockSize)
, mLevelY(mWorldBounds.height / mBlockSize)
, mMidX(mLevelX / 2)
{
    // Initialise level array to correct size
    for(int y = 0; y < mLevelY; y++)
    {
        mLevelArray.push_back(ArrayBlock());

        for(int x = 0; x < mLevelX; x++)
        {
            mLevelArray.at(y).push_back(nullptr);
        }
    }

    defineWallData();
}

void Level::defineWallData()
{
    sf::Vector2f pointA, pointB, norm;
    Wall::PointPair points;

    // Left wall
    pointA = sf::Vector2f(mBlockSize, 0.f);
    pointB = sf::Vector2f(mBlockSize, mWorldBounds.height);
    points = Wall::PointPair(pointA, pointB);
    norm = sf::Vector2f(1.f, 0.f);

    mBoundaryWallData.at(Wall::Type::LeftWall) = Wall::WallData(points, norm);

    // Right wall
    pointA = sf::Vector2f(mWorldBounds.width - mBlockSize, 0.f);
    pointB = sf::Vector2f(mWorldBounds.width - mBlockSize, mWorldBounds.height);
    points = Wall::PointPair(pointA, pointB);
    norm = sf::Vector2f(-1.f, 0.f);

    mBoundaryWallData.at(Wall::Type::RightWall) = Wall::WallData(points, norm);

    // Top wall left
    pointA = sf::Vector2f(0.f, mBlockSize);
//    pointB = sf::Vector2f(mWorldBounds.width, mBlockSize);
    pointB = sf::Vector2f(mBlockSize * (mMidX - 1), mBlockSize);
    points = Wall::PointPair(pointA, pointB);
    norm = sf::Vector2f(0.f, 1.f);

    mBoundaryWallData.at(Wall::Type::TopLeftWall) = Wall::WallData(points, norm);

     // Top wall right
    pointA = sf::Vector2f(mBlockSize * (mMidX + 2), mBlockSize);
//    pointB = sf::Vector2f(mWorldBounds.width, mBlockSize);
    pointB = sf::Vector2f(mWorldBounds.width, mBlockSize);
    points = Wall::PointPair(pointA, pointB);
    norm = sf::Vector2f(0.f, 1.f);

    mBoundaryWallData.at(Wall::Type::TopRightWall) = Wall::WallData(points, norm);

    // Bottom wall
    pointA = sf::Vector2f(0.f, mWorldBounds.height - mBlockSize);
    pointB = sf::Vector2f(mWorldBounds.width, mWorldBounds.height - mBlockSize);
    points = Wall::PointPair(pointA, pointB);
    norm = sf::Vector2f(0.f, -1.f);

    mBoundaryWallData.at(Wall::Type::BottomWall) = Wall::WallData(points, norm);
}

sf::Vector2i Level::worldCordsToIndex(sf::Vector2f pos) const
{
    int x = static_cast<int>(pos.x);
    int y = static_cast<int>(pos.y);

    sf::Vector2i index(x, y);
    index /= mBlockSize;

    if(index.x > mLevelX - 1
   || index.y > mLevelY - 1
   || index.x < 0
   || index.y < 0)
    throw std::runtime_error("ERROR: Index out of bounds in worldCordsToIndex() \nX: "
                             + std::to_string(index.x)
                             + " Y: "
                             + std::to_string(index.y)
                             + "\n");

    return index;
}

std::vector<LevelBlock*> Level::getInRangeBlocks(const MovingEntity* entity
                                                 , float radius) const
{
    std::vector<LevelBlock*> inRangeBlocks;

    sf::Vector2i index = worldCordsToIndex(entity->getWorldPosition());

    int x, y, leftStart, bottomStart, right, top;

    int sizeY = mLevelY - 1;
    int sizeX = mLevelX - 1;
    int range = 1;

    leftStart = std::max(0, index.x - range);
    right = std::min(sizeX, index.x + range);
    bottomStart = std::max(0, index.y - range);
    top = std::min(sizeY, index.y + range);

    for(y = bottomStart; y <= top; y++)
    {
        for(x = leftStart; x <= right; x++)
        {
            inRangeBlocks.push_back(mLevelArray.at(y).at(x));
        }
    }

    return inRangeBlocks;
}

std::vector<LevelBlock*> Level::getBlockTypeInRange(const MovingEntity* entity
                                                    , float radius
                                                    , LevelBlock::Type blockType) const
{
    std::vector<LevelBlock*> inRangeBlocks = getInRangeBlocks(entity
                                                              , radius);

    std::vector<LevelBlock*> returnBlocks;

    for(LevelBlock* lvlBlck : inRangeBlocks)
    {
        if(lvlBlck->getType() == blockType)
            returnBlocks.push_back(lvlBlck);
    }
//    }
//    else
//    {
//        for(LevelBlock* lvlBlck : inRangeBlocks)
//        {
//            Scenery* scenery = lvlBlck->getScenery();
//            sf::Vector2f toVec = scenery->getWorldPosition() - sheep->getWorldPosition();
//
//    //        radius = 500.f;
//
//            //the bounding radius of the other is taken into account by adding it
//            //to the range
//            double range = radius + scenery->radius();
//
//            //if entity within range, tag for further consideration. (working in
//            //distance-squared space to avoid sqrts)
//            if (magVec(toVec) < range*range)
//            {
//                inRangeScenery.push_back(scenery);
//            }
//        }
//    }

    return returnBlocks;
}

std::vector<MovingEntity*> Level::getEntitiesInRange(const MovingEntity* entity
                                                     , float neighbourhood) const
{
//    std::cout << "sada" << std::endl;

    assert(entity);

    std::vector<MovingEntity*> neighbourEntities;
    std::vector<LevelBlock*> inRangeBlocks = getInRangeBlocks(entity
                                                              , neighbourhood);

    const sf::Vector2f entityPos = entity->getWorldPosition();

//        std::cout << "sppppppppdda" << std::endl;


    for(LevelBlock* lvlBlck : inRangeBlocks)
    {
        std::list<MovingEntity*> entitiesInBlock = lvlBlck->getEntities();

//        std::cout << "asasttttt" << std::endl;


        for(MovingEntity* e : entitiesInBlock)
        {
            assert(e);
//            if(e)
//            {
//                    std::cout << "yyyyyyyyyyyyda" << std::endl;


                sf::Vector2f toNeighbour = e->getWorldPosition() - entityPos;
    //
                if(magVec(toNeighbour) <= neighbourhood)
                    neighbourEntities.push_back(e);

//                        std::cout << "shhhhhhhhhhhhhh" << std::endl;

//            }
        }
    }

//    std::cout << "saasdsadadda" << std::endl;

    return neighbourEntities;
}

void Level::generateLevel(std::array<SceneNode*, SceneNode::Layers::Num> sceneLayers
                          , Controller& controller)
{
    int levelX = 0;
    int levelY = 0;
    sf::Vector2f blockPos;

    float radius = std::sqrt((mBlockSize * mBlockSize)
                             + (mBlockSize * mBlockSize));
    int row, col;

//    int midIndexX;
//
//    if(mLevelX % 2 == 0)
//        midIndexX = (mLevelX / 2) - 1;
//    else
//        midIndexX = mLevelX / 2;

    for(row = 0; row < mLevelY; row++)
    {
        for(col = 0; col < mLevelX; col++)
        {
            blockPos = sf::Vector2f(levelX, levelY);
            LevelBlock::BlockPtr levelBlock = LevelBlock::BlockPtr(new LevelBlock(blockPos
                                                                                  , sf::Vector2i(col, row)
                                                                                  , mBlockSize
                                                                                , radius));

            // If exit
            if(row == 0
               && (col == mMidX
                   || col == mMidX - 1
                   || col == mMidX + 1))
            {
                levelBlock->setType(LevelBlock::Type::ExitBlock);
                levelBlock->addScenery(std::move(Exit::ExitPtr(new Exit(levelBlock.get()
                                                                        , controller.getTexture(Controller::Textures::Exit)))));
            }
            // If corner block
            else if((row == 0 && col == 0)
               || (row == mLevelY - 1 && col == mLevelX - 1)
               || (row == 0 && col == mLevelX - 1)
               || (row == mLevelY - 1 && col == 0))
            {
                levelBlock->setType(LevelBlock::Type::CornerBlock);

                Corner::CornerPtr corner = Corner::CornerPtr(new Corner(levelBlock.get()
                                                                              , controller.getTexture(Controller::Textures::Corner)));

                if(row == mLevelY - 1 && col == mLevelX - 1)
                    corner->rotateSprite(180.f);
                else if(row == 0 && col == mLevelX - 1)
                    corner->rotateSprite(90.f);
                else if(row == mLevelY - 1 && col == 0)
                    corner->rotateSprite(-90.f);

                levelBlock->addScenery(std::move(corner));


            }
            // Else if wall block
            else if(row == 0
               || row == mLevelY - 1
               || col == 0
               || col == mLevelX - 1)
            {
                levelBlock->setType(LevelBlock::Type::WallBlock);
                Wall::WallData wallData;

                // Get correct wall data for block
                // If top wall
                if(row == 0)
                {
                    if(col < mMidX - 1)
                        wallData = mBoundaryWallData.at(Wall::Type::TopLeftWall);
                    else if(col > mMidX + 1)
                        wallData = mBoundaryWallData.at(Wall::Type::TopRightWall);
                }
                // Else if bottom wall
                else if(row == mLevelY - 1)
                    wallData = mBoundaryWallData.at(Wall::Type::BottomWall);
                // Else if left wall
                else if(col == 0)
                    wallData = mBoundaryWallData.at(Wall::Type::LeftWall);
                // Else right wall
                else
                    wallData = mBoundaryWallData.at(Wall::Type::RightWall);


                Wall::WallPtr wall = Wall::WallPtr(new Wall(levelBlock.get()
                                                            , controller.getTexture(Controller::Textures::Wall)
                                                            , wallData.first.first
                                                            , wallData.first.second
                                                            , wallData.second));

                if(row == 0)
                    wall->rotateSprite(-90.f);
                // Else if bottom wall
                else if(row == mLevelY - 1)
                    wall->rotateSprite(90.f);
                // Else if left wall
                else if(col == mLevelX - 1)
                    wall->rotateSprite(180.f);

                levelBlock->addScenery(std::move(wall));

            }
            // Else if grass block
            else// if((row > 1 && row < mLevelY - 2)
                  //  && (col > 1 && col < mLevelX - 2))
            {
                levelBlock->setType(LevelBlock::Type::GrassBlock);
                levelBlock->addScenery(std::move(Grass::GrassPtr(new Grass(levelBlock.get()
                                                                           , controller.getTexture(Controller::Textures::Grass)))));

            }
                        // Else if grass block
//            else if((row > 1 && row < mLevelY - 2)
//                    && (col > 1 && col < mLevelX - 2))
//            {
//                levelBlock->setType(LevelBlock::Type::GrassBlock);
//                levelBlock->addScenery(std::move(Grass::GrassPtr(new Grass(levelBlock.get()))));
//            }
            // Else empty block
//            else
//            {
//                levelBlock->setType(LevelBlock::Type::EmptyBlock);
//                levelBlock->addScenery(std::move(Empty::EmptyPtr(new Empty(levelBlock.get()))));
//            }

            mLevelArray.at(row).at(col) = levelBlock.get();
            sceneLayers.at(SceneNode::Layers::Background)->addChild(std::move(levelBlock));

            levelX += mBlockSize;
        }

        levelX = 0;
        levelY += mBlockSize;
    }
}

LevelBlock* Level::insertEntityIntoLevel(MovingEntity* entity) const
{
    sf::Vector2i index = worldCordsToIndex(entity->getWorldPosition());
    return mLevelArray.at(index.y).at(index.x)->insertEntity(entity);
}


