#ifndef CORNER_HPP
#define CORNER_HPP

#include <memory>

#include "Scenery.hpp"

class Corner : public Scenery
{
public:
    typedef std::unique_ptr<Corner> CornerPtr;

                                Corner(LevelBlock* host
                                       , const sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {};

    virtual                     ~Corner(){};

    // Getters
    virtual void                getSceneryData(){};
};

#endif // CORNER_HPP
