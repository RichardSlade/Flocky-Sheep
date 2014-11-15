#include "StationaryTarget.hpp"

void StationaryTarget::drawCurrent(sf::RenderTarget& target
                                   , sf::RenderStates states) const
{
    target.draw(mCircle, states);
}
