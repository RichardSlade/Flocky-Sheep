#ifndef BORDER_HPP
#define BORDER_HPP

#include <memory>

#include "Scenery.hpp"

class Border : public Scenery
{
public:
    typedef std::unique_ptr<Border> BorderPtr;

                                Border(LevelBlock* host)
                                :Scenery(host)
                                {
                                    mBlockFace.setFillColor(mBrown);
                                };

    virtual                     ~Border(){};

    // Getters
    virtual void                getSceneryData(){};

    // Setters
    virtual void                update(){};

    virtual void                changeSceneryData(){};
};

#endif // BORDER_HPP
