#include "Controller.hpp"

const sf::Time Controller::FPS = sf::seconds(1.f/60.f);

Controller::Controller()
: mWindow(sf::VideoMode(640, 480), "AI Steering behaviours", sf::Style::Close)
, mCountDown(FPS)
//, mWorld(*this, mWindow)
{
    loadTextures();

//    mWorld = new World(*this, mWindow);
    mWorld = World::upWorld(new World(*this, mWindow));
};

void Controller::loadTextures()
{
    std::vector<std::string> fileNames;

    fileNames.push_back("media/grass.png");
    fileNames.push_back("media/hunter.png");


    for(std::string s : fileNames)
    {
        sf::Texture t;
        t.loadFromFile(s);

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
            mCountDown = FPS;
            mWorld->update(FPS);
        }

        mWorld->handleInput();
        mWorld->display();
    }
}
