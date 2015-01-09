#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <array>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "AppState.hpp"
#include "Params.hpp"
//#include "HighScoreTable.hpp"

class Controller : sf::NonCopyable
{
public:
    static const sf::Time           mFPS;

    enum Textures
    {
        Dog,
        Sheep,
        Grass,
        Wall,
        Corner,
        Exit,
        GameBackground,
        NumTextures
    };

    enum Fonts
    {
        Sansation,
        Smoke,
        Sheepy,
        NumFonts
    };

//    enum DifficultyType
//    {
//        Easy,
//        Medium,
//        Hard,
//        NumDifficulties
//    };

private:
    Params                          mParams;

    const float                     mWindowX;
    const float                     mWindowY;

    sf::RenderWindow                mWindow;
    sf::Clock                       mClock;
    sf::Time                        mCountDown;
    sf::Vector2f                    mResetViewCenter;

    sf::RenderTexture               mBackgroundTexture;
    std::vector<sf::Texture>        mTextures;
    std::vector<sf::Font>           mFonts;

    AppState::StateType             mAppStateType;
    std::unique_ptr<AppState>       mCurrentAppState;

//    DifficultyType                  mDiffType;

    std::string                     mUserName;
    bool                            mChangeState;

//    std::unique_ptr<HighScoreTable> mHighScoreTable;

    void                            loadMedia();
    const sf::Texture&              createBackgroundTexture();
//    void                            loadHighScores();
    void                            changeAppState();

public:
                                    Controller();

    void                            run();

    // Getters
    const sf::Texture&              getTexture(Textures type) const
                                    { return mTextures.at(type); }

    const sf::Font&                 getFont(Controller::Fonts type) const
                                    { return mFonts.at(type); }

    const Params&                   getParams() const
                                    { return mParams; }

//    HighScoreTable*                 getHighScoreTable()
//                                    { return mHighScoreTable.get(); }

    // Setters
    void                            resetView();

    void                            changeState()
                                    { mChangeState = true; }

    void                            setUserName(std::string name)
                                    { mUserName = name; }
};

#endif // CONTROLLER_HPP
