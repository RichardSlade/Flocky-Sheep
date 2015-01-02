#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "AppState.hpp"
#include "World.hpp"

class MenuState : public AppState
{
private:
    Controller&         mController;
    sf::RenderWindow&   mWindow;

public:
                        MenuState(Controller&
                                  , sf::RenderWindow&);

    virtual             ~MenuState(){};

    virtual void        update(sf::Time);
    virtual void        handleInput();
    virtual void        display();
};


#endif // MENUSTATE_HPP

