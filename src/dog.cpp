#include "Dog.hpp"
#include "World.hpp"

Dog::Dog(World* world
         , const sf::Texture& texture
         , const sf::Font& font
         , sf::Vector2f startPos
//         , State<Dog>* globalState
//         , State<Dog>* initState
         , EntityStats stats
         , const Params& params)
: MovingEntity(world
               , texture
               , font
               , startPos
               , stats
               , params
               , MovingEntity::EntityType::Dog)
//, mWaypointRadius(params.WaypointRadius)
//, mWaypoints(waypoints)
//, mPathIndex(-1)
//, mStateMachine(this, globalState, initState)
{
    setSteeringTypes(SteeringBehaviour::Behaviour::FollowPath);
    mText.setString("Woof!");

//    mSprite.setScale(1.25f, 1.25f);
}

void Dog::updateCurrent(sf::Time dt)
{
//    mStateMachine.update();

//    MovingEntity::updateCurrent(dt);

    sf::Color currentTextColor = mText.getColor();

//    if(currentTextColor.a > 4)
        currentTextColor.a -= 1;

    mText.setColor(currentTextColor);

    sf::Vector2f steering = mSteering.calculate(dt);
//    sf::Vector2f acceleration = steering / mMass;

//    mVelocity += acceleration * dt.asSeconds();
    mVelocity += steering;// * dt.asSeconds();
//    mVelocity = acceleration;// * dt.asSeconds();

    if(std::fabs(magVec(mVelocity)) > MINFLOAT)
    {
        int sign = signVec(mHeading, mVelocity);

        float angle = std::acos(dotVec(mHeading, normVec(mVelocity)));
//        float angle = std::acos(dotVec(mHeading, mVelocity));
        angle *= sign;

        clampRotation(angle
                      , -mMaxTurnRate
                      , mMaxTurnRate);

        if(angle > MINFLOAT || angle < -MINFLOAT)
            rotate(angle * (180.f / SteeringBehaviour::mPI));
    }

    float currentRotation = getRotation() * (SteeringBehaviour::mPI / 180.f);
    mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));

//    truncateVec(mVelocity, mMaxSpeed);
    move(mVelocity);

    adjustPosition();

    sf::FloatRect bounds = mText.getLocalBounds();
    mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sf::Vector2f textPos = getWorldPosition();
    textPos.y -= 20.f;
    mText.setPosition(textPos);
}

void Dog::drawCurrent(sf::RenderTarget& target
                                    , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);

    std::vector<sf::CircleShape> wypnts = mSteering.getPathToDraw();

    for(sf::CircleShape circle : wypnts)
        target.draw(circle);
}
