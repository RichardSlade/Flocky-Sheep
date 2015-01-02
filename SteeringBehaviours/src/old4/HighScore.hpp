#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include <string>

struct HighScore
{
    HighScore()
    : name("---------")
    , score(0.f)
    {}

    HighScore(std::string n
              , float scr)
    : name(n)
    , score(scr)
    {};

    std::string name;
    float score;

    friend bool operator<(const HighScore& lhs
                          , const HighScore& rhs)
    {
        return (lhs.score > rhs.score);
    }
};

#endif // HIGHSCORE_HPP
