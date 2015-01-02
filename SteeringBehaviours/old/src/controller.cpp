#include "Controller.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"

const sf::Time Controller::mFPS = sf::seconds(1.f/60.f);
//const float Controller::mWindowX = 1024;
//const float Controller::mWindowY = 640;

Controller::Controller()
: mParams("data/params.txt")
, mWindowX(mParams.WindowX)
, mWindowY(mParams.WindowY)
, mWindow(sf::VideoMode(mWindowX, mWindowY), "AI Steering behaviours", sf::Style::Close)
, mCountDown(mFPS)
, mAppStateType(AppState::StateType::Game)
{
    mWindow.setPosition(sf::Vector2i(0, 0));
    loadTextures();

    if(!mFont.loadFromFile("media/Sansation.ttf"))
        throw std::runtime_error("Failed to load file: media/Sansation.ttf");

    mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this,
                                                                mWindow));

//    mWorld = std::unique_ptr<World>(new World(*this, mWindow));
};

void Controller::loadTextures()
{
    std::vector<std::string> fileNames;

    fileNames.push_back("media/grass.png");
    fileNames.push_back("media/dog.png");
    fileNames.push_back("media/sheep.png");
    fileNames.push_back("media/grassDepth.png");
    fileNames.push_back("media/wall.png");
    fileNames.push_back("media/corner.png");
    fileNames.push_back("media/exit.png");
//    fileNames.push_back("media/tree.png");
//    fileNames.push_back("media/pen.png");

    for(std::string s : fileNames)
    {
        sf::Texture t;

        if(!t.loadFromFile(s))
            throw std::runtime_error("Failed to load file: " + s);

        mTextures.push_back(t);
    }
}

void Controller::run()
{
    while(mWindow.isOpen())
    {
        mCountDown -= mClock.restart();

        if(mCountDown < sf::Time::Zero)
        {
            mCountDown = mFPS;
//            mWorld->update(FPS);
            mCurrentAppState->update(mFPS);
        }

            mCurrentAppState->handleInput();
            mCurrentAppState->display();
//        mWorld->handleInput();
//        mWorld->display();
    }
}

void Controller::changeAppState()
{
    switch(mAppStateType)
    {
        case AppState::StateType::Menu:
        {
            mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this
                                                                        , mWindow));
            mAppStateType = AppState::StateType::Game;

        }
        case AppState::StateType::Game:
        {
            mCurrentAppState = std::unique_ptr<MenuState>(new MenuState(*this
                                                                        , mWindow));
            mAppStateType = AppState::StateType::Menu;
        }
        default: break;
    }
}
