#include <algorithm>
#include <fstream>
#include <iostream>

#include <SFML/Window/Event.hpp>

#include "HighScoreTable.hpp"
#include "Controller.hpp"
#include "AppState.hpp"
#include "GameState.hpp"

const int HighScoreTable::mMaxNumScores = 10;

HighScoreTable::HighScoreTable(Controller& cntrl
                               , sf::RenderWindow& window)
: mHostGameState(nullptr)
, mWindow(window)
, mView(window.getView())
, mViewCenter(mView.getCenter())
{
    for(size_t i = 0; i < HighScoreTable::TextType::NumTextType; i++)
        mHighScoreTableText.push_back(sf::Text("", cntrl.getFont(Controller::Fonts::Sansation), 32));

    mHighScoreTableText.at(HighScoreTable::TextType::TitleText).setCharacterSize(128);
    mHighScoreTableText.at(HighScoreTable::TextType::TitleText).setFont(cntrl.getFont(Controller::Fonts::Smoke));
    mHighScoreTableText.at(HighScoreTable::TextType::TitleText).setString("High Scores");

    mHighScoreTableText.at(HighScoreTable::TextType::OptionText).setCharacterSize(64);
    mHighScoreTableText.at(HighScoreTable::TextType::OptionText).setString("Replay");

//    for(sf::Text& text : mHighScoreTableText)
//    {
//        sf::FloatRect bounds = text.getLocalBounds();
//        text.setOrigin(bounds.width / 2.f
//                       , 0.f);
//    }

    std::ifstream inNames("data/highscore/highScoreNames.dat");
    std::ifstream inScores("data/highscore/highScoreTimes.dat");

    if(!inNames.is_open())
    {
        std::cout << "Initialising scores" << std::endl;

        for(int i = 0; i < mMaxNumScores; i++)
            mHighScores.push_back(HighScore());
    }
    else
    {
        std::string name;
        float score;

        for(int i = 0; i < mMaxNumScores; i++)
        {
            inNames >> name;
            inScores >> score;

            mHighScores.push_back(HighScore(name
                                            , score));
        }
    }

    inNames.close();
    inScores.close();

    setupHighScoreText();
    setup(window.getView());
}

HighScoreTable::~HighScoreTable()
{
    std::ofstream outNames("data/highscore/highScoreNames.dat");
    std::ofstream outScores("data/highscore/highScoreTimes.dat");

    if(!outNames.is_open())
        throw std::runtime_error("Error: writing to file data/highscore/highScoresNames.dat");

    sortAndTruncateScores();

    for(HighScore& hs : mHighScores)
    {
        if(outNames.eof()
          || outScores.eof())
            break;

        outNames << hs.name << "\n";
        outScores << hs.score << "\n";
    }

    outNames.close();
    outScores.close();
}

void HighScoreTable::sortAndTruncateScores()
{
    std::sort(mHighScores.begin(), mHighScores.end());

    std::vector<HighScore>::iterator it = mHighScores.begin();
    it += mMaxNumScores - 1;

    mHighScores.erase(it, mHighScores.end());
}

void HighScoreTable::setupHighScoreText()
{
    std::string newStr = "Name\t\t\tSheep\n";

    for(HighScore& hs : mHighScores)
    {
        newStr += hs.name + "\t\t\t" + std::to_string(hs.score) + "\n";
    }

    mHighScoreTableText.at(HighScoreTable::TextType::HighScoreText).setString(newStr);
}

void HighScoreTable::draw(sf::RenderTarget& target
                          , sf::RenderStates states) const
{
    for(const sf::Text& txt : mHighScoreTableText)
        target.draw(txt);
}

void HighScoreTable::handleInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			mWindow.close();
		else if(event.type == sf::Event::KeyReleased)
		{
            if(mHostGameState)
                mHostGameState->resetGame();
		}
	}
}

void HighScoreTable::setup(sf::View view)
{
    for(sf::Text& text : mHighScoreTableText)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, 0.f);
    }

    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    size /= 2.f;

    mHighScoreTableText.at(HighScoreTable::TextType::TitleText).setPosition(center.x
                                                                            , center.y - 128);

    mHighScoreTableText.at(HighScoreTable::TextType::HighScoreText).setPosition(center);

    mHighScoreTableText.at(HighScoreTable::TextType::OptionText).setPosition(center.x
                                                                             , center.y + (size.x - 20));
}

void HighScoreTable::addHighScore(HighScore newHighScore)
{
    std::vector<HighScore>::iterator it;

    std::string newUsername = newHighScore.name;
    float newVal = newHighScore.score;

    bool exists = false;
//    float existsVal;

    for(it = mHighScores.begin(); it != mHighScores.end(); it++)
    {
        std::string name = it->name;

        if(name.compare(newUsername))
        {
            exists = true;

            if(it->score > newVal)
                it = mHighScores.end();

            break;
        }
    }

    if(!exists)
        mHighScores.push_back(newHighScore);
    else
    {
        if(it != mHighScores.end())
        {
            mHighScores.erase(it);
            mHighScores.push_back(newHighScore);
        }
    }

    sortAndTruncateScores();
    setupHighScoreText();
}

