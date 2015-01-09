#ifndef EXIT_HPP
#define EXIT_HPP

#include <memory>

#include "Scenery.hpp"

class Exit : public Scenery
{
public:
    typedef std::unique_ptr<Exit> ExitPtr;

                                Exit(LevelBlock* host
                                     , const sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {};

    virtual                     ~Exit(){};

    // Getters
    virtual void                getSceneryData(){};
};

#endif // EXIT_HPP

