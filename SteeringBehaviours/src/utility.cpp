#include <cmath>
#include <ctime>
#include<cstdlib>

#include "Utility.hpp"

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

bool lineIntersection2D(sf::Vector2f A
                        , sf::Vector2f B
                        , sf::Vector2f C
                        , sf::Vector2f D
                        , float& dist
                        , sf::Vector2f& point)
{

    float rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
    float rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

    float sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
    float sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

    if ( (rBot == 0) || (sBot == 0))
    {
        //lines are parallel
        return false;
    }

    float r = rTop/rBot;
    float s = sTop/sBot;

    if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
    {
        sf::Vector2f tempVec = (B - A) * r;

        dist = magVec(tempVec);
//        dist = Vec2Distance(A,B) * r;
        point = A + r * (B - A);
        return true;
    }

    else
    {
        dist = 0;
        return false;
    }
}
