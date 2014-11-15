#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <array>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "World.hpp"

class Controller : sf::NonCopyable
{
private:
    static const sf::Time           FPS;

    sf::RenderWindow                mWindow;
    sf::Clock                       mClock;
    sf::Time                        mCountDown;

    std::vector<sf::Texture>        mTextures;

    World::upWorld                  mWorld;

    void                            loadTextures();

public:
                                    Controller();

    void                            run();

    sf::Texture&                    getTexture(Textures::Type type){ return mTextures[type]; };
};

#endif // CONTROLLER_HPP
