#include <iostream>

#include "Controller.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "LevelBlock.hpp"

const sf::Time Controller::mFPS = sf::seconds(1.f / 60.f);

Controller::Controller()
: mParams()
, mWindowX(mParams.WindowX)
, mWindowY(mParams.WindowY)
, mWindow(sf::VideoMode(mWindowX, mWindowY), "AI Steering behaviours", sf::Style::Close)
, mCountDown(mFPS)
, mResetViewCenter(mWindow.getView().getCenter())
, mAppStateType(AppState::StateType::Menu)
, mChangeState(false)
{
    mWindow.setPosition(sf::Vector2i(0, 0));
    loadMedia();

//    mHighScoreTable = std::unique_ptr<HighScoreTable>(new HighScoreTable(*this
//                                                                         , mWindow));

//    mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this
//                                                                , mWindow
//                                                                , getHighScoreTable()
//                                                                , ""
//                                                                , mParams.WorldDimHard
//                                                                , mParams.NumSheepHard
//                                                                , mParams.TimeEasy));

    mCurrentAppState = std::unique_ptr<MenuState>(new MenuState(*this,
                                                                mWindow));
};

void Controller::loadMedia()
{
    for(unsigned int i = 0; i < Textures::NumTextures; i++)
        mTextures.push_back(sf::Texture());

    createBackgroundTexture();

    std::vector<std::string> fileNames;
//    fileNames.push_back("media/textures/grassDepth.png");
//    fileNames.push_back("media/textures/grassDepth.png");
    fileNames.push_back("media/textures/dog.png");
    fileNames.push_back("media/textures/sheep.png");
    fileNames.push_back("media/textures/grass.png");
//    fileNames.push_back("media/textures/obstacle1.png");
    fileNames.push_back("media/textures/wall.png");
    fileNames.push_back("media/textures/corner.png");
    fileNames.push_back("media/textures/exit.png");

    int index = 0;

    for(std::string s : fileNames)
    {
        sf::Texture t;

        if(!t.loadFromFile(s))
            throw std::runtime_error("Failed to load file: " + s);

        mTextures.at(index) = t;

        index ++;
    }

    mTextures.at(Textures::GameBackground) = createBackgroundTexture();
//
//    mTextures.at(Controller::Textures::MenuBackground).setRepeated(true);
//    mTextures.at(Controller::Textures::GameBackground).setRepeated(true);

    fileNames.clear();

    fileNames.push_back("media/fonts/Sansation.ttf");
    fileNames.push_back("media/fonts/AlphaSmoke.TTF");
    fileNames.push_back("media/fonts/KingthingsSheepishly.ttf");

     for(std::string s : fileNames)
    {
        sf::Font f;

        if(!f.loadFromFile(s))
            throw std::runtime_error("Failed to load file: " + s);

        mFonts.push_back(f);
    }
}

const sf::Texture& Controller::createBackgroundTexture()
{
    const float WorldX = mParams.WorldDimMax;
    const float WorldY = mParams.WorldDimMax;
    const int BlockSize = mParams.LevelBlockSize;

    const float TextureX = WorldX * 2.f;
    const float TextureY = WorldY * 2.f;

    mBackgroundTexture.create(TextureX
                              , TextureY);

    mBackgroundTexture.clear();

    for(float row = 0; row < TextureY * 4; row += BlockSize)
    {
        for(float col = 0; col < TextureX; col += BlockSize)
        {
            LevelBlock levelBlock(mTextures.at(Textures::Grass)
                                            , sf::Vector2f(col, row));

            mBackgroundTexture.draw(levelBlock.getBackground());
        }
    }

    return mBackgroundTexture.getTexture();
}

void Controller::changeAppState()
{
    resetView();

    switch(mAppStateType)
    {
        case AppState::StateType::Menu:
        {
            mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this
                                                                        , mWindow
//                                                                        , mHighScoreTable.get()
                                                                        , mUserName));
            mAppStateType = AppState::StateType::Game;
            break;

        }
        case AppState::StateType::Game:
        {
            mCurrentAppState = std::unique_ptr<MenuState>(new MenuState(*this
                                                                        , mWindow));
            mAppStateType = AppState::StateType::Menu;
            break;
        }
        default: break;
    }

    mChangeState = false;
}

void Controller::run()
{
    while(mWindow.isOpen())
    {
        mCountDown -= mClock.restart();

        if(mCountDown < sf::Time::Zero)
        {
            mCountDown = mFPS;
            mCurrentAppState->update(mFPS);
        }

        mCurrentAppState->handleInput();
        mCurrentAppState->display();

        if(mChangeState)
            changeAppState();
    }
}

void Controller::resetView()
{
    sf::View view = mWindow.getView();
    view.setCenter(mResetViewCenter);
    mWindow.setView(view);
}

