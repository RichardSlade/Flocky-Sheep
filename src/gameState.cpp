//#include <string>

#include <SFML/Window/Event.hpp>

#include "GameState.hpp"
#include "Controller.hpp"
#include "Params.hpp"
//#include "HighScore.hpp"

GameState::GameState(Controller& cntrl
                     , sf::RenderWindow& window
//                     , HighScoreTable* highScores
                     , std::string username)
: mWorldDimMax(cntrl.getParams().WorldDimMax)
, mNumSheepMax(cntrl.getParams().NumSheepMax)
, mLevelTimeMin(cntrl.getParams().LevelTimeMin)
, mResetWorldDim(cntrl.getParams().WorldDimMin)
, mResetNumSheep(cntrl.getParams().NumSheepMin)
, mResetLevelTime(cntrl.getParams().LevelTimeMax)
, mController(cntrl)
, mWindow(window)
, mWorldDim(mResetWorldDim)
, mNumSheep(mResetNumSheep)
, mLevelTime(sf::seconds(mResetLevelTime * 60.f))
, mTotalSheepHerded(0)
, mUsername(username)
, mWorld(new World(*this
                   , cntrl
                   , window
                   , username
                   , mWorldDim
                   , mNumSheep
                   , mLevelTime))
//, mHighscoreTable(highScores)
, mPausedScreen(mController
                , *this
                , mWindow)
, mLevelCompleteScreen(mController
                     , *this
                     , mWindow)
, mGameCompleteScreen(mController
                   , *this
                   , mWindow)
, mCurrentScreen(GameState::Screen::Game)
, mNewScreen(mCurrentScreen)
, mPaused(false)
{

}

void GameState::restartWorld()
{
    mWorld = std::unique_ptr<World>(new World(*this
                                              , mController
                                              , mWindow
                                              , mUsername
                                              , mWorldDim
                                              , mNumSheep
                                              , mLevelTime));

    mNewScreen = GameState::Screen::Game;
}

void GameState::update(sf::Time dt)
{
    if(mNewScreen != mCurrentScreen)
        mCurrentScreen = mNewScreen;

    switch(mCurrentScreen)
    {
        case GameState::Screen::Game:
        {
            if(!mPausedScreen.isPaused())
                mWorld->update(dt);
            else
                mPausedScreen.update(dt);

            break;
        }
        case GameState::Screen::LevelComplete: mLevelCompleteScreen.update(dt); break;
//        case GameState::Screen::HighScore: mHighscoreTable->update
        case GameState::Screen::GameComplete: mGameCompleteScreen.update(dt); break;
        default: break;
    }
}

void GameState::handleInput()
{
    if(mCurrentScreen == GameState::Screen::Game
       && mWorld)
    {
        if(!mPausedScreen.isPaused())
            mWorld->handleInput();
        else
            mPausedScreen.handleInput();
    }
    else if(mCurrentScreen == GameState::Screen::LevelComplete)
        mLevelCompleteScreen.handleInput();
    else if(mCurrentScreen == GameState::Screen::GameComplete)
        mGameCompleteScreen.handleInput();
//    else if(mCurrentScreen == GameState::Screen::Scores)
//        mHighscoreTable->handleInput();
}

void GameState::display()
{
    mWindow.clear();

    if(mWorld)
        mWorld->display();

    if(mCurrentScreen == GameState::Screen::Game)
    {
        if(mPausedScreen.isPaused())
            mWindow.draw(mPausedScreen);
    }
    else if(mCurrentScreen == GameState::Screen::LevelComplete)
        mWindow.draw(mLevelCompleteScreen);
    else if(mCurrentScreen == GameState::Screen::GameComplete)
        mWindow.draw(mGameCompleteScreen);
//    else if(mCurrentScreen == GameState::Screen::Scores)
//        mWindow.draw(*mHighscoreTable);

    mWindow.display();
}

void GameState::pause()
{
    mPausedScreen.setup(mWindow.getView());
}

void GameState::levelComplete()
{
    mNewScreen = GameState::Screen::LevelComplete;
    mLevelCompleteScreen.setup(mWindow.getView());

//    HighScore newHighScore(mUserName
//                           , mWorld->getPlayerScore());
//
//    mHighScoreTable->addHighScore(newHighScore);

//    mController.resetView();
}

void GameState::gameComplete(int sheepFromLastLevel)
{
    mNewScreen = GameState::Screen::GameComplete;
//    mHighscoreTable->setup(mWindow.getView());

    mGameCompleteScreen.setup(mWindow.getView(), mTotalSheepHerded + sheepFromLastLevel);
}

void GameState::nextLevel()
{
    const float blockSize = mController.getParams().LevelBlockSize;

    mWorldDim += blockSize * 4;
    mNumSheep += 10;
    mLevelTime -= mWorld->getTimeTaken();
    mLevelTime += sf::seconds(mLevelTimeMin * 60.f);

    mTotalSheepHerded += mWorld->getSheepHerded();

//    if(mLevelTime < mLevelTimeMin)
//        mLevelTime = mLevelTimeMin;

    if(mWorldDim > mWorldDimMax)
        mWorldDim = mWorldDimMax;

    restartWorld();
}

void GameState::resetGame()
{
    mWorldDim = mResetWorldDim;
    mNumSheep = mResetNumSheep;
    mLevelTime = sf::seconds(mResetLevelTime * 60.f);

    mTotalSheepHerded = 0;

//    mHighscoreTable->addHighScore(HighScore(mUsername
//                                            , mTotalSheepHerded));

    restartWorld();
}

void GameState::quitGameState()
{
    mController.changeState();
}
