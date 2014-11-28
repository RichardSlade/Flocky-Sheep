#include <iostream>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
//#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "World.hpp"
#include "Controller.hpp"
#include "SpriteNode.hpp"
#include "Utility.hpp"
#include "LevelBlock.hpp"
#include "Sheep.hpp"
#include "StationaryTarget.hpp"
#include "Wall.hpp"
#include "Scenery.hpp"
//#include "Enum.hpp"

const int World::mLevelDivision = 8;
const int World::mNumSheep = 25;
const int World::mNumWalls = 4;
const int World::mNumObstacles = 50;
const float World::mObstacleRadius = 25.f;
const float World::mTargetRadius = 10.f;
const float World::mWallWidth = 20.f;
const float World::mScrollSpeed = 10.f;

World::World(Controller& controller, sf::RenderWindow& window)
: mController(controller)
, mWindow(window)
, mWorldView(mWindow.getDefaultView())
, mWorldBounds(sf::Vector2i(0, 0), sf::Vector2i(1280, 1280))
, mFocusPoint(getViewBounds().width / 2.f, getViewBounds().height / 2.f)
//, mObstacles()
, mSheep()
, mTargets()
{
    mFont.loadFromFile("media/Sansation.ttf");

    buildScene();
}

void World::buildScene()
{
    // Initialize the different scene graph layers
	for(std::size_t i = 0; i < SceneNode::Layers::Num; i++)
	{
		SceneNode::upScNode layer(new SceneNode());
		mSceneLayers.at(i) = layer.get();

		mSceneGraph.addChild(std::move(layer));
	}

	sf::Texture& texture = mController.getTexture(Controller::Textures::Background);
    texture.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture
                                                                , sf::Vector2f(0.f, 0.f)
                                                                , mWorldBounds));

    mSceneLayers.at(SceneNode::Layers::Background)->addChild(std::move(backgroundSprite));

    generateLevel();
}

void World::generateLevel()
{
    int levelX = 0;
    int levelY = 0;
    int dividerInc = mWorldBounds.width / mLevelDivision;

    for(int row = 0; row < mLevelDivision; row++)
    {
        for(int col = 0; col < mLevelDivision; col++)
        {
            LevelBlock::Type blockType = LevelBlock::Type::Nothing;
//            sf::Texture& blckTexture = mController.getTexture(Controller::Textures::Grass);

            sf::Vector2f blckPos(levelX, levelY);
            float radius = rangedClamped(0.5f, 1.5f);

            if(col % 2 != 0
               && row != 0
               && col != 0
               && row != mLevelDivision - 1
               && col != mLevelDivision - 1)
            {
                unsigned int type = rand() % 2 + 1;
//                std::cout << "Type: " << type << std::endl;

                switch(type)
                {
                    case 1: blockType = LevelBlock::Type::Grass; break;
                    case 2: blockType = LevelBlock::Type::Tree; break;
                    case 3: blockType = LevelBlock::Type::Pen; break;
                    default: break;
                }


                if(blockType == LevelBlock::Type::Grass)
                {

                    std::unique_ptr<LevelBlock> levelBlock(new LevelBlock(blockType
                                                                       , mController.getTexture(Controller::Textures::Grass)
                                                                       , blckPos
                                                                       , dividerInc
                                                                       , radius));

                    mLevel.at(row).at(col) = levelBlock.get();
                    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(levelBlock));
                }
                else if(blockType == LevelBlock::Type::Tree)
                {
                    std::unique_ptr<LevelBlock> levelBlock(new LevelBlock(blockType
                                                                       , mController.getTexture(Controller::Textures::Tree)
                                                                       , blckPos
                                                                       , dividerInc
                                                                       , radius));

                    mLevel.at(row).at(col) = levelBlock.get();
                    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(levelBlock));

                }
                else if(blockType == LevelBlock::Type::Pen)
                {
                    std::unique_ptr<LevelBlock> levelBlock(new LevelBlock(blockType
                                                                       , mController.getTexture(Controller::Textures::Pen)
                                                                       , blckPos
                                                                       , dividerInc
                                                                       , radius));

                    mLevel.at(row).at(col) = levelBlock.get();
                    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(levelBlock));

                }
            }
            else
            {
                std::unique_ptr<LevelBlock> levelBlock(new LevelBlock(blockType
                                                                       , blckPos
                                                                       , dividerInc));

                mLevel.at(row).at(col) = levelBlock.get();
                mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(levelBlock));
            }

            levelX += dividerInc;
        }

        levelX = 0;
        levelY += dividerInc;
    }

    std::unique_ptr<StationaryTarget> targetNode(new StationaryTarget(mTargetRadius
                                                                     , sf::Vector2f(200.f, 200.f)));
    mTargets.push_back(targetNode.get());
    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(targetNode));

    generateWalls();
    generateAgents();
}

void World::generateWalls()
{
    enum VecElement{Pos, Size, PointA, PointB, Norm};

    std::vector<std::vector<sf::Vector2f>> walls;

    std::vector<sf::Vector2f> wallLeft;
    wallLeft.push_back(sf::Vector2f(0.f, 0.f));
    wallLeft.push_back(sf::Vector2f(mWallWidth, mWorldBounds.height));
    wallLeft.push_back(sf::Vector2f(mWallWidth, 0.f));
    wallLeft.push_back(sf::Vector2f(mWallWidth, mWorldBounds.height));
    wallLeft.push_back(sf::Vector2f(1.f, 0.f));

    walls.push_back(wallLeft);

    std::vector<sf::Vector2f> wallTop;
    wallTop.push_back(sf::Vector2f(0.f, 0.f));
    wallTop.push_back(sf::Vector2f(mWorldBounds.width, mWallWidth));
    wallTop.push_back(sf::Vector2f(0.f, mWallWidth));
    wallTop.push_back(sf::Vector2f(mWorldBounds.width, mWallWidth));
    wallTop.push_back(sf::Vector2f(0.f, 1.f));

    walls.push_back(wallTop);

    std::vector<sf::Vector2f> wallRight;
    wallRight.push_back(sf::Vector2f(mWorldBounds.width - mWallWidth, 0.f));
    wallRight.push_back(sf::Vector2f(mWallWidth, mWorldBounds.height));
    wallRight.push_back(sf::Vector2f(mWorldBounds.width - mWallWidth, 0.f));
    wallRight.push_back(sf::Vector2f(mWorldBounds.width - mWallWidth, mWorldBounds.height));
    wallRight.push_back(sf::Vector2f(-1.f, 0.f));

    walls.push_back(wallRight);


    std::vector<sf::Vector2f> wallBottom;
    wallBottom.push_back(sf::Vector2f(0.f, mWorldBounds.height - mWallWidth));
    wallBottom.push_back(sf::Vector2f(mWorldBounds.width, mWallWidth));
    wallBottom.push_back(sf::Vector2f(0.f, mWorldBounds.height - mWallWidth));
    wallBottom.push_back(sf::Vector2f(mWorldBounds.width, mWorldBounds.height - mWallWidth));
    wallBottom.push_back(sf::Vector2f(0.f, -1.f));

    walls.push_back(wallBottom);


    for(int wl = 0; wl < mNumWalls; wl++)
    {
        Wall::upWall wall(new Wall(walls.at(wl).at(VecElement::Pos)
                                   , walls.at(wl).at(VecElement::Size)
                                   , walls.at(wl).at(VecElement::PointA)
                                   , walls.at(wl).at(VecElement::PointB)
                                   , walls.at(wl).at(VecElement::Norm)
                                   , sf::Color::Blue));

        mWalls.push_back(wall.get());
        mSceneLayers.at(SceneNode::Foreground)->addChild(std::move(wall));
    }
}

void World::generateAgents()
{
    const int divider = mWorldBounds.width / mLevelDivision;

    for(int i = 0 ; i < mNumSheep; i++)
    {
        sf::Vector2f pos(rangedClamped(divider, mWorldBounds.width - divider)
                         , rangedClamped(divider, mWorldBounds.width - divider));

        int x = pos.x / divider;
        int y = pos.y / divider;

        LevelBlock* block = mLevel.at(y).at(x);

        if(block->getType() != LevelBlock::Type::Nothing)
        {
            sf::Vector2f sceneryPos = block->getScenery()->getWorldPosition();
            float sceneryRadius = block->getScenery()->radius();

            sf::Vector2f toScenery = sceneryPos - pos;

            if(magVec(toScenery) < sceneryRadius)
                pos += -toScenery;
        }

        std::unique_ptr<Sheep> sheepNode(new Sheep(this
                                                        , mController.getTexture(Controller::Textures::Sheep)
                                                        , pos
                                                        , rangedClamped(0.75f, 1.25f)));
        sheepNode->setTarget(mTargets.at(0));
        sheepNode->setSteeringType(SteeringBehaviour::Behaviour::Wander);
        mSheep.push_back(sheepNode.get());

        mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(sheepNode));
    }
}

void World::generateObstacles()
{
//    for(int i = 0; i < mNumObstacles; i++)
//    {
//        sf::Vector2f pos(rangedClamped(mWorldBounds.left + mWallWidth, mWorldBounds.width - mWallWidth)
//                         , rangedClamped(mWorldBounds.top + mWallWidth, mWorldBounds.height - mWallWidth));
//
//        Obstacle::upObstacle obs(new Obstacle(pos
//                                              , mObstacleRadius));
//
//        mObstacles.push_back(obs.get());
//        mSceneLayers.at(SceneNode::Foreground)->addChild(std::move(obs));
//
//    }

//    while(true)
//    {
//        sf::FloatRect
//    }
}

void World::createObstacle(sf::Vector2f pos)
{
//    Obstacle::upObstacle obs(new Obstacle(pos
//                                          , mObstacleRadius));
//
//    mObstacles.push_back(obs.get());
//    mSceneLayers.at(SceneNode::Foreground)->addChild(std::move(obs));
}

void World::handleRealTimeInput()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        mFocusPoint.x -= mScrollSpeed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        mFocusPoint.x += mScrollSpeed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        mFocusPoint.y -= mScrollSpeed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        mFocusPoint.y += mScrollSpeed;
    }
}

void World::update(sf::Time dt)
{
    mSceneGraph.update(dt);
}

void World::handleInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos;
            sf::FloatRect vBounds = getViewBounds();

            mousePos.x = sf::Mouse::getPosition(mWindow).x + vBounds.left;
            mousePos.y = sf::Mouse::getPosition(mWindow).y + vBounds.top;

            sf::Vector2f mousePosFloat(mousePos.x, mousePos.y);

            if(event.mouseButton.button == sf::Mouse::Left)
            {
                mTargets.at(0)->setPosition(mousePosFloat);
            }
            else if(event.mouseButton.button == sf::Mouse::Right)
            {
                createObstacle(mousePosFloat);
            }
        }
        else if(event.type == sf::Event::KeyPressed)
        {

        }
    }

    handleRealTimeInput();
}

void World::display()
{
    mWindow.clear();

    mWorldView.setCenter(mFocusPoint);
    mWindow.setView(mWorldView);

    mWindow.draw(mSceneGraph);

//    for(Obstacle* ob : mObstacles)
//        ob->changeColour(sf::Color::Green);

    mWindow.display();
}

std::vector<Scenery*> World::obstaclesInRange(Sheep* sheep, float radius) const
{
    const float BlockBorder = 25.f;

    std::vector<LevelBlock*> inRangeBlocks;
    std::vector<Scenery*> inRangeScenery;

    int divider = mWorldBounds.width / mLevelDivision;

    sf::Vector2i levelIndex;
    int x = std::fabs(sheep->getWorldPosition().x / divider);
    int y = std::fabs(sheep->getWorldPosition().y / divider);

    if(mLevel.at(y).at(x)->getType() == LevelBlock::Type::Tree)
        inRangeBlocks.push_back(mLevel.at(y).at(x));

    float blockSize = mLevel.at(y).at(x)->getSize();

    sf::Transform blockTransform = mLevel.at(levelIndex.y).at(levelIndex.x)->getInverseTransform();
    sf::Vector2f vehBlockPos = blockTransform * sheep->getWorldPosition();

    if(vehBlockPos.x < BlockBorder
       && vehBlockPos.x > 0)
    {
        if(mLevel.at(levelIndex.y).at(levelIndex.x - 1)->getType() == LevelBlock::Type::Tree)
            inRangeBlocks.push_back(mLevel.at(levelIndex.y).at(levelIndex.x - 1));
    }

    if(vehBlockPos.x > blockSize - BlockBorder
       && vehBlockPos.x < mLevelDivision - 1)
    {
        if(mLevel.at(levelIndex.y).at(levelIndex.x + 1)->getType() == LevelBlock::Type::Tree)
            inRangeBlocks.push_back(mLevel.at(levelIndex.y).at(levelIndex.x + 1));
    }

    if(vehBlockPos.y < BlockBorder
       && vehBlockPos.y > 0)
    {
        if(mLevel.at(levelIndex.y - 1).at(levelIndex.x)->getType() == LevelBlock::Type::Tree)
            inRangeBlocks.push_back(mLevel.at(levelIndex.y - 1).at(levelIndex.x));
    }

    if(vehBlockPos.y > blockSize - BlockBorder
       && vehBlockPos.y < mLevel.size() - 1)
    {
        if(mLevel.at(levelIndex.y + 1).at(levelIndex.x)->getType() == LevelBlock::Type::Tree)
            inRangeBlocks.push_back(mLevel.at(levelIndex.y + 1).at(levelIndex.x));
    }

    for(LevelBlock* lvlBlck : inRangeBlocks)
    {
        Scenery* scenery = lvlBlck->getScenery();
        sf::Vector2f toVec = scenery->getWorldPosition() - sheep->getWorldPosition();

        //the bounding radius of the other is taken into account by adding it
        //to the range
        double range = radius + scenery->radius();

        //if entity within range, tag for further consideration. (working in
        //distance-squared space to avoid sqrts)
        if (magVec(toVec) < range*range)
        {
            inRangeScenery.push_back(scenery);
        }
    }

    return inRangeScenery;
}

sf::FloatRect World::getViewBounds()
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}
