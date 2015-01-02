#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <memory>

#include "Scenery.hpp"

class Obstacle : public Scenery
{
public:
    typedef std::unique_ptr<Obstacle> ObstaclePtr;

                                Obstacle(LevelBlock* host)
                                :Scenery(host)
                                {
                                    mBlockFace.setFillColor(mBrown);
                                };

    virtual                     ~Obstacle(){};

    // Getters
    virtual void                getSceneryData(){};

    // Setters
    virtual void                update(){};

    virtual void                changeSceneryData(){};
};

#endif // OBSTACLE_HPP

