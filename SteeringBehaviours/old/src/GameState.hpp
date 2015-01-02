#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "AppState.hpp"
#include "Controller.hpp"
#include "World.hpp"

class GameState : public AppState
{
private:
    World               mWorld;

public:
                        GameState(Controller&
                                  , sf::RenderWindow&);

    virtual             ~GameState(){};

    virtual void        update(sf::Time);
    virtual void        handleInput();
    virtual void        display();
};


#endif // GAMESTATE_HPP
