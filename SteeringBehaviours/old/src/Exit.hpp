#ifndef EXIT_HPP
#define EXIT_HPP

#include <memory>

#include "Scenery.hpp"

class Exit : public Scenery
{
public:
    typedef std::unique_ptr<Exit> ExitPtr;

                                Exit(LevelBlock* host
                                     , sf::Texture& texture)
                                :Scenery(host
                                         , texture)
                                {
//                                    mBlockFace.setFillColor(mBrown);
                                };

    virtual                     ~Exit(){};

    // Getters
    virtual void                getSceneryData(){};

    // Setters
    virtual void                update(){};
    virtual void                changeSceneryData(){};
};

#endif // EXIT_HPP

