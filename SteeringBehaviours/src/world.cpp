#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
//#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "World.hpp"
#include "Controller.hpp"
#include "SpriteNode.hpp"

const int World::NumVehicles = 1;
const int World::NumWalls = 4;
const float World::TargetRadius = 10.f;

World::World(Controller& controller, sf::RenderWindow& window)
: mController(controller)
, mWindow(window)
, mWorldView(mWindow.getDefaultView())
, mWorldBounds(sf::Vector2i(0, 0), sf::Vector2i(1280, 960))
, mFocusPoint(getViewBounds().width / 2.f, getViewBounds().height / 2.f)
, mObstacles()
, mVehicles()
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

	sf::Texture& texture = mController.getTexture(Textures::Background);
    texture.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture
                                                                , sf::Vector2f(0.f, 0.f)
                                                                , mWorldBounds));

    mSceneLayers.at(SceneNode::Layers::Background)->addChild(std::move(backgroundSprite));

    generateObstacles();

    std::unique_ptr<StationaryTarget> targetNode(new StationaryTarget(TargetRadius
                                                                    , sf::Vector2f(200.f, 200.f)));

    mTargets.push_back(targetNode.get());
    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(targetNode));

    std::unique_ptr<Vehicle> vehicleNode(new Vehicle(this
                                                    , mController.getTexture(Textures::Vehicle)
                                                    , sf::Vector2f(300.f, 300.f)
                                                    , SteeringBehaviours::Wander));
    vehicleNode->setTarget(mTargets.at(0));
    mVehicles.push_back(vehicleNode.get());

    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(vehicleNode));
}

void World::generateObstacles()
{
    std::vector<std::vector<sf::Vector2f>> walls;

    std::vector<sf::Vector2f> wallLeft;
    wallLeft.push_back(sf::Vector2f(0.f, 0.f));
    wallLeft.push_back(sf::Vector2f(20.f, 0.f));
    wallLeft.push_back(sf::Vector2f(20.f, mWorldBounds.height));
    wallLeft.push_back(sf::Vector2f(0.f, mWorldBounds.height));

    walls.push_back(wallLeft);

    std::vector<sf::Vector2f> wallTop;
    wallTop.push_back(sf::Vector2f(0.f, 0.f));
    wallTop.push_back(sf::Vector2f(0.f, 20.f));
    wallTop.push_back(sf::Vector2f(mWorldBounds.width, 20.f));
    wallTop.push_back(sf::Vector2f(mWorldBounds.width, 0.f));

    walls.push_back(wallTop);

    std::vector<sf::Vector2f> wallRight;
    wallRight.push_back(sf::Vector2f(mWorldBounds.width - 20.f, 0.f));
    wallRight.push_back(sf::Vector2f(mWorldBounds.width, 0.f));
    wallRight.push_back(sf::Vector2f(mWorldBounds.width, mWorldBounds.height));
    wallRight.push_back(sf::Vector2f(mWorldBounds.width  - 20.f, mWorldBounds.height));

    walls.push_back(wallRight);


    std::vector<sf::Vector2f> wallBottom;
    wallBottom.push_back(sf::Vector2f(mWorldBounds.width, mWorldBounds.height - 20.f));
    wallBottom.push_back(sf::Vector2f(mWorldBounds.width, mWorldBounds.height));
    wallBottom.push_back(sf::Vector2f(0.f, mWorldBounds.height));
    wallBottom.push_back(sf::Vector2f(0.f, mWorldBounds.height - 20.f));

    walls.push_back(wallBottom);


    for(int i = 0; i < NumWalls; i++)
    {
        mSceneLayers.at(SceneNode::Foreground)->addChild(Obstacle::upObstacle(new Obstacle(Collidable::Wall
                                                    , sf::PrimitiveType::Quads
                                                    , walls.at(i)
                                                    , sf::Color::Blue)));
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

            mTargets.at(0)->setPosition(mousePosFloat);
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Up:
                {
                    mFocusPoint.y -= 20.f;
                    break;
                }
            case sf::Keyboard::Down:
                {
                    mFocusPoint.y += 20.f;
                    break;
                }
            case sf::Keyboard::Right:
                {
                    mFocusPoint.x += 20.f;
                    break;
                }
            case sf::Keyboard::Left:
                {
                    mFocusPoint.x -= 20.f;
                    break;
                }
            default: break;
            }
        }
    }
}

void World::display()
{
    mWindow.clear();

    mWorldView.setCenter(mFocusPoint);
    mWindow.setView(mWorldView);

    mWindow.draw(mSceneGraph);

    sf::Vector2f pos(mVehicles.at(0)->position());

    sf::Text text("Hello", mFont);
    text.setString("X: " + std::to_string(pos.x) + "\nY: " + std::to_string(pos.y));

    sf::FloatRect viewBounds = getViewBounds();
    text.setPosition(viewBounds.left, viewBounds.top);
    mWindow.draw(text);

    mWindow.display();
}

sf::FloatRect World::getViewBounds()
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}
