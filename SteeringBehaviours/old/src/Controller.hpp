#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <array>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

//#include "World.hpp"
//#include "Enum.hpp"
#include "AppState.hpp"
#include "Params.hpp"

class Controller : sf::NonCopyable
{
public:
    static const sf::Time           mFPS;
    Params                          mParams;

    enum Textures
    {
        Background,
        Dog,
        Sheep,
        Grass,
        Wall,
        Corner,
        Exit,
//        Tree,
//        Pen,
        Num
    };

private:
    const float                     mWindowX;
    const float                     mWindowY;

    sf::RenderWindow                mWindow;
    sf::Clock                       mClock;
    sf::Time                        mCountDown;

    std::vector<sf::Texture>        mTextures;
    sf::Font                        mFont;



//    std::unique_ptr<World>          mWorld;

    AppState::StateType             mAppStateType;
    std::unique_ptr<AppState>       mCurrentAppState;

    void                            loadTextures();

public:
                                    Controller();

    void                            run();

    // Getters
    sf::Texture&                    getTexture(Textures type)
                                    { return mTextures.at(type); }

    sf::Font&                       getFont()
                                    { return mFont; }

   Params&                          getParams()
                                    { return mParams; }

    // Setters
    void                            changeAppState();
};

#endif // CONTROLLER_HPP
