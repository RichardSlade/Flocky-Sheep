#ifndef APPSTATE_HPP
#define APPSTATE_HPP

#include <SFML/System/Time.hpp>

struct AppState
{
public:
    enum StateType
    {
        Menu,
        Game,
        NumStateTypes
    };

    virtual             ~AppState(){};

    virtual void        update(sf::Time) = 0;
    virtual void        handleInput() = 0;
    virtual void        display() = 0;
};

#endif // APPSTATE_HPP
