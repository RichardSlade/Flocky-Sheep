#include "Dog.hpp"

//const float Dog::mDogSpeed = 30.f;
//const float Dog::mDogMaxRotation = 0.4f;

Dog::Dog(const World* world
         , sf::Texture& texture
         , sf::Font& font
         , sf::Vector2f startPos
         , EntityStats stats
         , Params& params)
: MovingEntity(world
               , texture
               , font
               , startPos
               , stats
               , params)
, mDogMaxRotation(params.DogMaxRotation)
{
    mText.setString("Woof!");
}

void Dog::drawCurrent(sf::RenderTarget& target
                                    , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);
}
