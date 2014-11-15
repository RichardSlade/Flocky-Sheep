#include <cmath>
#include <ctime>
#include<cstdlib>

#include "Utility.hpp"
#include "C2DMatrix.h"

float magVec(const sf::Vector2f vec)
{
    return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

sf::Vector2f normVec(const sf::Vector2f vec)
{
    return vec / magVec(vec);
}

void truncateVec(sf::Vector2f& vec, float maxSpeed)
{
    float mag = magVec(vec);

    while(mag > maxSpeed)
    {
        vec /= 16.f;

        mag = magVec(vec);
    }
}

int signVec(sf::Vector2f v1, sf::Vector2f v2)
{
  if (v1.y*v2.x > v1.x*v2.y)
  {
    return -1;
  }
  else
  {
    return 1;
  }
}

float dotVec(const sf::Vector2f vec1, const sf::Vector2f vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

float randomClamped()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float rangedClamped(float lo, float hi)
{
    return lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
}

void clampRotation(float& angle, float min, float max)
{
    if(angle < min)
        angle = min;
    else if(angle > max)
        angle = max;
}
