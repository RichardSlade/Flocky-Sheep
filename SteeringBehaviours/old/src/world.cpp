#include <iostream>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "World.hpp"
#include "Controller.hpp"
#include "SpriteNode.hpp"
#include "Utility.hpp"
#include "LevelBlock.hpp"
#include "Dog.hpp"
#include "Sheep.hpp"
#include "Waypoint.hpp"
#include "Wall.hpp"
#include "Scenery.hpp"
#include "SheepStates.hpp"
//#include "Params.hpp"

//const int World::mLevelBlockSize = 40;
//
//const int World::mWorldX = 800;
//const int World::mWorldY = 800;
//const int World::mNumSheep = 30;
//const int World::mNumWalls = 4;
//const int World::mNumObstacles = 50;
//const float World::mObstacleRadius = 25.f;
//const float World::mWaypointRadius = 10.f;
////const float World::mWallWidth = 20.f;
//const float World::mScrollSpeed = 0.5f;

World::World(Controller& controller
             , sf::RenderWindow& window)
: mLevelBlockSize(controller.mParams.LevelBlockSize)
, mWorldX(controller.mParams.WorldX)
, mWorldY(controller.mParams.WorldY)
, mNumSheep(controller.mParams.NumSheep)
, mWaypointRadius(controller.mParams.WaypointRadius)
, mScrollSpeed(controller.mParams.ScrollSpeed)
, mController(controller)
, mWindow(window)
, mWorldView(mWindow.getDefaultView())
, mWorldBounds(sf::Vector2i(0, 0), sf::Vector2i(mWorldX, mWorldY))
, mFocusPoint(getViewBounds().width / 2.f, getViewBounds().height / 2.f)
, mLevel(mLevelBlockSize
              , mWorldBounds)
{
    initialiseStatesAndStats();
    buildScene();
}

void World::initialiseStatesAndStats()
{
    // Initialise sheep states
    mSheepStates.push_back(std::unique_ptr<LookOutForDog>(new LookOutForDog));
    mSheepStates.push_back(std::unique_ptr<EvadeDog>(new EvadeDog));
    mSheepStates.push_back(std::unique_ptr<Sleep>(new Sleep));
    mSheepStates.push_back(std::unique_ptr<Relax>(new Relax));
    mSheepStates.push_back(std::unique_ptr<Forage>(new Forage));
    mSheepStates.push_back(std::unique_ptr<ApproachFood>(new ApproachFood));
    mSheepStates.push_back(std::unique_ptr<Eat>(new Eat));

    mEntityStats.push_back(EntityStats(1.f
                                       , 1.f
                                       , 2.f
                                       , 0.1f));

    mEntityStats.push_back(EntityStats(1.f
                                       , 2.f
                                       , 2.f
                                       , 0.2f));
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

    std::unique_ptr<Waypoint> waypointNode(new Waypoint(mWaypointRadius
                                                        , sf::Vector2f(200.f, 200.f)));
    mWaypoints.push_back(waypointNode.get());
    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(waypointNode));

    mLevel.generateLevel(mSceneLayers, mController);
    generateAgents();
}

void World::generateAgents()
{
    // Initialise dog and add to scene graph
    std::unique_ptr<Dog> dogNode(new Dog(const_cast<World*>(this)
                                         , mController.getTexture(Controller::Textures::Dog)
                                         , mController.getFont()
                                         , sf::Vector2f(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
                                         , mEntityStats.at(StatsType::DogStats)
                                         , mController.getParams()));

    dogNode->setTarget(mWaypoints.at(0));

    // Save pointer to dog for sheep initialisation
    Dog* dogPtr = dogNode.get();

    std::vector<SteeringBehaviour::Behaviour> behaviours;
    behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
    dogNode->setSteeringTypes(behaviours);
    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(dogNode));

    // Initialise sheep and add to scene graph
    for(int i = 0 ; i < mNumSheep; i++)
    {
        // Find square for sheep to start in
        sf::Vector2f pos(rangedClamped(mLevelBlockSize, mWorldBounds.width - mLevelBlockSize)
                         , rangedClamped(mLevelBlockSize, mWorldBounds.width - mLevelBlockSize));

        std::unique_ptr<Sheep> sheepNode(new Sheep(const_cast<World*>(this)
                                                    , mController.getTexture(Controller::Textures::Sheep)
                                                    , mController.getFont()
                                                    , pos
                                                    , mSheepStates.at(Sheep::States::LookOut).get()
                                                    , mSheepStates.at(Sheep::States::Relax).get()
                                                    , mEntityStats.at(StatsType::SheepStats)
                                                    , mController.getParams()
                                                    , rangedClamped(0.75f, 1.25f)));

        sheepNode->setMovingTarget(dogPtr);
        mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(sheepNode));
    }
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
    mSceneGraph.removeDeletedNodes();
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

            sf::Vector2f mousePosF(mousePos.x, mousePos.y);

            if(event.mouseButton.button == sf::Mouse::Left)
            {
                mousePosF.x = std::min(mousePosF.x, static_cast<float>(mWorldBounds.width - (mLevelBlockSize + mWaypointRadius)));
                mousePosF.x = std::max(mousePosF.x, static_cast<float>(mLevelBlockSize + mWaypointRadius));

                mousePosF.y = std::min(mousePosF.y, static_cast<float>(mWorldBounds.height - (mLevelBlockSize + mWaypointRadius)));
                mousePosF.y = std::max(mousePosF.y, static_cast<float>(mLevelBlockSize + mWaypointRadius));

                mWaypoints.at(0)->setPosition(mousePosF);
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

    sf::CircleShape circle(5.f);
    sf::FloatRect bounds = circle.getLocalBounds();
    circle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    circle.setFillColor(sf::Color::Yellow);

    mWindow.draw(circle);

    mWindow.display();
}

const sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

std::vector<LevelBlock*> World::getBlockTypeInRange(const MovingEntity* entity
                                                    , LevelBlock::Type blockType
                                                    , float radius = 0.f) const
{
    return mLevel.getBlockTypeInRange(entity
                                           , radius
                                           , blockType);
}

LevelBlock* World::insertEntityIntoLevel(MovingEntity* entity) const
{
    return mLevel.insertEntityIntoLevel(entity);
}

std::vector<MovingEntity*> World::getEntitiesInRange(const MovingEntity* entity
                                                     , float neighbourhoodRadius) const
{
    return mLevel.getEntitiesInRange(entity
                                     , neighbourhoodRadius);
}
