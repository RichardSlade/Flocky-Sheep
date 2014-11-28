#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/System/Vector2.hpp>

float magVec(const sf::Vector2f vec);

sf::Vector2f normVec(const sf::Vector2f vec);

void truncateVec(sf::Vector2f& vec
                 , float maxSpeed);

int signVec(sf::Vector2f
            , sf::Vector2f);

float dotVec(const sf::Vector2f vec1
             , const sf::Vector2f vec2);

float randomClamped();

float rangedClamped(float
                    , float);

void clampRotation(float&
                   , float
                   , float);

bool lineIntersection2D(sf::Vector2f
                        , sf::Vector2f
                        , sf::Vector2f
                        , sf::Vector2f
                        , float&
                        , sf::Vector2f&);

#endif // UTILITY_HPP
