#ifndef HIGHSCORETABLE_HPP
#define HIGHSCORETABLE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "HighScore.hpp"
//#include "GameState.hpp"

class Controller;
class GameState;

class HighScoreTable : public sf::Drawable
{
private:
    static const int            mMaxNumScores;

    enum TextType
    {
        TitleText,
        HighScoreText,
        OptionText,
        NumTextType
    };

    GameState*                  mHostGameState;
    sf::RenderWindow&           mWindow;
    sf::View                    mView;
    sf::Vector2f                mViewCenter;

    int 						mCurrentMenuSelection;

    std::vector<sf::Text>       mHighScoreTableText;
    std::vector<HighScore>      mHighScores;

    void                        sortAndTruncateScores();
    void                        setupHighScoreText();

public:
                                HighScoreTable(Controller&
                                               , sf::RenderWindow&);

    virtual                     ~HighScoreTable();

    virtual void                draw(sf::RenderTarget&
                                     , sf::RenderStates) const;

    void                        handleInput();

    void                        setup(sf::View);
    void                        addHighScore(HighScore);
};

#endif // HIGHSCORETABLE_HPP
