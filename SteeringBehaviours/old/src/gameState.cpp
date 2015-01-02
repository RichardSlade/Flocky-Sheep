#include "GameState.hpp"

GameState::GameState(Controller& cntrl
                     , sf::RenderWindow& window)
: mWorld(cntrl
         , window)
{

}

void GameState::update(sf::Time dt)
{
    mWorld.update(dt);
}

void GameState::handleInput()
{
    mWorld.handleInput();
}

void GameState::display()
{
    mWorld.display();
}
