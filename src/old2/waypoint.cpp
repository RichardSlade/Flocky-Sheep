#include "Waypoint.hpp"

Waypoint::Waypoint(float radius
                   , sf::Vector2f pos)
: mCircle(radius)
, mReached(false)
{
    sf::FloatRect bounds = mCircle.getLocalBounds();
    mCircle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    setPosition(pos);
};

void Waypoint::drawCurrent(sf::RenderTarget& target
                                   , sf::RenderStates states) const
{
    target.draw(mCircle, states);
}

void Waypoint::updateCurrent(sf::Time dt)
{
    if(mReached)
        mToRemove = true;
}
