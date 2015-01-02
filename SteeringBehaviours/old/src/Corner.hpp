#ifndef CORNER_HPP
#define CORNER_HPP

#include <memory>

#include "Scenery.hpp"

class Corner : public Scenery
{
public:
    typedef std::unique_ptr<Corner> CornerPtr;

                                Corner(LevelBlock* host
                                       , sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {
//                                    mBlockFace.setFillColor(sf::Color::Red);
                                };

    virtual                     ~Corner(){};

    // Getters
    virtual void                getSceneryData(){};

    // Setters
    virtual void                update(){};
    virtual void                changeSceneryData(){};
};

#endif // CORNER_HPP
