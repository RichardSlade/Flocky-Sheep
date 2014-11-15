#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "Enum.hpp"
#include "Vehicle.hpp"
#include "StationaryTarget.hpp"
#include "Obstacle.hpp"
#include "SceneNode.hpp"

class Controller;

class World : private sf::NonCopyable
{
public:
    typedef std::unique_ptr<World>  upWorld;

private:
    static const int                        NumVehicles;
    static const int                        NumWalls;
    static const float                      TargetRadius;

    sf::Font                                mFont;

    Controller&                             mController;
    sf::RenderWindow&                       mWindow;
    sf::View                                mWorldView;
    sf::IntRect                             mWorldBounds;
    sf::Vector2f                            mFocusPoint;

    SceneNode                               mSceneGraph;
    std::array<SceneNode*
                , SceneNode::Layers::Num>   mSceneLayers;

    std::vector<Obstacle*>                  mObstacles;
    std::vector<Vehicle*>                   mVehicles;
    std::vector<StationaryTarget*>          mTargets;

    void                                    buildScene();
    void                                    generateObstacles();

public:
                                            World(Controller&, sf::RenderWindow&);

    void                                    update(sf::Time);
    void                                    handleInput();
    void                                    display();

    sf::FloatRect                           getViewBounds();
};

#endif // WORLD_HPP
