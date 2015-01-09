#include <SFML/Window/Event.hpp>

#include "MenuState.hpp"
#include "Controller.hpp"
#include "World.hpp"

const int MenuState::mMaxTextNum = 4;
const sf::Color MenuState::mSelectedTextColour = sf::Color::White;
const sf::Color MenuState::mOtherTextColour = sf::Color(255, 255, 255, 125);
//const sf::Time MenuState::mUnderscoreResetTime = sf::seconds(0.25f);

MenuState::MenuState(Controller& cntrl
                     , sf::RenderWindow& window)
: mView(window.getView())
, mViewCenter(mView.getCenter())
, mMenuTextPos(mViewCenter.x
               , mViewCenter.y + 64)
, mController(cntrl)
, mWindow(window)
, mBackground(cntrl.getTexture(Controller::Textures::GameBackground)
                    , mWindow.getViewport(mView))
, mTitleText("Flocky\n\t Sheep"
             , mController.getFont(Controller::Fonts::Smoke)
             , 128)
//, mUnderscore("_"
//              , mController.getFont(Controller::Fonts::Smoke)
//              , 64)
//, mUnderscoreCountdown(mUnderscoreResetTime)
//, mUnderscoreOn(false)
, mCurrentMenu(MenuState::MenuType::Main)
, mNewMenu(mCurrentMenu)
, mPreviousMenu(mCurrentMenu)
, mCurrentMenuSelection(0)
//, mEnterName(false)
{
    mTitleText.setColor(mSelectedTextColour);

    float fontSize = mTitleText.getCharacterSize();

    sf::FloatRect bounds = mTitleText.getLocalBounds();
    mTitleText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mTitleText.setPosition(mViewCenter.x, mViewCenter.y - fontSize);

    for(size_t i = 0; i < mMaxTextNum; i++)
    {
        mMenuText.push_back(sf::Text(""
                                     , mController.getFont(Controller::Fonts::Sansation)
                                     , 64));
        mMenuText.at(i).setColor(mOtherTextColour);
    }

    changeMenu(mCurrentMenu);
}

void MenuState::handleMenuInput(int selectionChange)
{
    int maxMenu = getMaxMenuSelection(mCurrentMenu) - 1;

    mCurrentMenuSelection += selectionChange;

    if(mCurrentMenuSelection < 0)
        mCurrentMenuSelection = maxMenu;
    else if(mCurrentMenuSelection > maxMenu)
        mCurrentMenuSelection = 0;
}

void MenuState::changeMenu(MenuType newMenuType)
{
    for(sf::Text& txt : mMenuText)
        txt.setString("");

//    mMenuText.at(0).setFont(mController.getFont(Controller::Fonts::Sansation));

    switch(newMenuType)
    {
        case MenuState::MenuType::Main:
        {
            mMenuText.at(0).setString("Play");
//            mMenuText.at(1).setString("Help");
            mMenuText.at(1).setString("Quit");
            break;
        }
//        case MenuState::MenuType::EnterName:
//        {
//            mUserName = "Enter Name";
//            mMenuText.at(0).setFont(mController.getFont(Controller::Fonts::Sansation));
//
//            mMenuText.at(0).setString(mUserName);
//            mMenuText.at(1).setString("Back");
//            break;
//        }
//        case MenuState::MenuType::Difficulty:
//        {
//            mMenuText.at(0).setString("Easy");
//            mMenuText.at(1).setString("Medium");
//            mMenuText.at(2).setString("Hard");
//            mMenuText.at(3).setString("Back");
//            break;
//        }
        case MenuState::MenuType::Help:
        {
            mMenuText.at(0).setString("Back");
            break;
        }
        case MenuState::MenuType::Quit:
        {
            mWindow.close();
            break;
        }
        case MenuState::MenuType::Start:
        {
            mController.changeState();
            break;
        }
        default: break;
    }

    float inc = mMenuText.at(0).getCharacterSize();
    sf::Vector2f textPos = mMenuTextPos;

    for(sf::Text& txt : mMenuText)
    {
        sf::FloatRect bounds = txt.getLocalBounds();
        txt.setOrigin(bounds.width / 2.f, 0.f);
        txt.setPosition(textPos);

        textPos.y += inc;
    }

    mPreviousMenu = mCurrentMenu;
    mCurrentMenu = newMenuType;
    mCurrentMenuSelection = 0;
}

int MenuState::getMaxMenuSelection(MenuType menuType)
{
    switch(menuType)
    {
        case MenuState::MenuType::Main: return 2;
//        case MenuState::MenuType::EnterName: return 2;
//        case MenuState::MenuType::Difficulty: return 4;
        case MenuState::MenuType::Help: return 1;
        default: return 0;
    }
}

MenuState::MenuType MenuState::getSelectionMenuType(int menuSelection)
{
    switch(mCurrentMenu)
    {
        case MenuState::MenuType::Main:
        {
            switch(menuSelection)
            {
//                case 0: return MenuState::MenuType::EnterName;
                case 0: return MenuState::MenuType::Start;
//                case 1: return MenuState::MenuType::Help;
                case 1: return MenuState::MenuType::Quit;
                default: return MenuState::MenuType::Main;
            }
        }
//        case MenuState::MenuType::EnterName:
//        {
//            switch(menuSelection)
//            {
//                case 0: return MenuState::MenuType::Start;
//                case 1: return MenuState::MenuType::Main;
//                default: return mCurrentMenu;
//            }
//        }
        default: return MenuState::MenuType::Main;
    }
}

void MenuState::update(sf::Time dt)
{
    if(mCurrentMenu != mNewMenu)
        changeMenu(mNewMenu);

    for(sf::Text& txt : mMenuText)
        txt.setColor(mOtherTextColour);

    mMenuText.at(mCurrentMenuSelection).setColor(mSelectedTextColour);

//    if(mCurrentMenu == MenuType::EnterName
//       && mEnterName)
//    {
//        mUnderscoreCountdown -= dt;
//
//        if(mUnderscoreCountdown < sf::Time::Zero)
//        {
//            mUnderscoreCountdown = mUnderscoreResetTime;
//
//            if(mUnderscore.getString() == '_')
//                mUnderscore.setString("");
//            else
//                mUnderscore.setString("_");
//        }
//
//        sf::Vector2f txtPos = mMenuText.at(0).getPosition();
//
//        txtPos.x += (mMenuText.at(0).getGlobalBounds().width / 2.f);
//
//        mUnderscore.setPosition(txtPos);
//
//        mMenuText.at(0).setString(mUserName);
//    }
}

void MenuState::handleInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            mWindow.close();
            continue;
        }

        switch(mCurrentMenu)
        {
//            case MenuState::MenuType::EnterName:
//            {
//                if(!mEnterName)
//                {
//                    if(event.type == sf::Event::KeyReleased)
//                    {
//                        if(event.key.code == sf::Keyboard::Escape)
//                        {
////                            mPreviousMenu = MenuType::Difficulty;
//                            mNewMenu = mPreviousMenu;
////                            changeMenu(mNewMenu);
//                        }
//                        else if(event.key.code == sf::Keyboard::Up)
//                            handleMenuInput(-1);
//                        else if(event.key.code == sf::Keyboard::Down)
//                            handleMenuInput(1);
//                        else if(event.key.code == sf::Keyboard::Return)
//                        {
////                            mNewMenu = getSelectionMenuType(mCurrentMenuSelection);
//
//                            switch(mCurrentMenuSelection)
//                            {
//                                case 0:
//                                {
//                                    mEnterName = true;
//                                    mUserName = "";
//                                    mMenuText.at(0).setString(mUserName);
//
//                                    sf::Vector2f txtPos = mMenuText.at(0).getPosition();
//                                    txtPos.x += mMenuText.at(0).getGlobalBounds().width / 2.f;
//
//                                    mUnderscore.setPosition(txtPos);
//                                    break;
//                                }
//                                case 1: mNewMenu = getSelectionMenuType(mCurrentMenuSelection); break;
//                                default: break;
//                            }
//                        }
//                    }
//                }
//                else
//                {
//                    if(event.key.code == sf::Keyboard::Return)
//                    {
//                            mController.setUserName(mUserName);
//    //                        mNewMenu = MenuState::MenuType::Difficulty;
//                            mEnterName = false;
//                            mController.changeState();
//                    }
//                    else if(event.type == sf::Event::KeyReleased)
//                    {
//                        if(event.key.code == sf::Keyboard::Escape)
//                        {
//                            mEnterName = false;
//                            mUserName = "Enter Name";
//                            mMenuText.at(0).setString(mUserName);
//
//                            sf::FloatRect bounds = mMenuText.at(0).getLocalBounds();
//                            mMenuText.at(0).setOrigin(bounds.width / 2.f, 0.f);
//                            mMenuText.at(0).setPosition(mMenuTextPos);
//
//                            mUnderscore.setString("");
//                        }
//                    }
//                    else if(event.type == sf::Event::TextEntered)
//                    {
//                        if(event.text.unicode == 8) // If backspace pressed
//                            mUserName = mUserName.substr(0, mUserName.length() - 1);
//                        else if(mUserName.size() < 15)
//                            mUserName += static_cast<char>(event.text.unicode);
//
//                        mMenuText.at(0).setString(mUserName);
//
//                        sf::FloatRect bounds = mMenuText.at(0).getLocalBounds();
//                        mMenuText.at(0).setOrigin(bounds.width / 2.f, 0.f);
//                        mMenuText.at(0).setPosition(mMenuTextPos);
//                    }
//                }
//
//                break;
//            }
            default:
            {
                if(event.type == sf::Event::KeyReleased)
                {
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        if(mCurrentMenu == MenuType::Main)
                        {
                            mPreviousMenu = MenuType::Main;
                        }
                        else
                            mNewMenu = mPreviousMenu;
                    }
                    if(event.key.code == sf::Keyboard::Up)
                        handleMenuInput(-1);
                    else if(event.key.code == sf::Keyboard::Down)
                        handleMenuInput(1);
                    else if(event.key.code == sf::Keyboard::Return)
                        mNewMenu = getSelectionMenuType(mCurrentMenuSelection);
                }

                break;
            }
        }
    }
}

void MenuState::display()
{
    mWindow.clear(sf::Color(25, 25, 25));

    mWindow.draw(mBackground);
    mWindow.draw(mTitleText);

    for(sf::Text& txt : mMenuText)
        mWindow.draw(txt);

//    if(mCurrentMenu == MenuType::EnterName
//       && mEnterName)
//        mWindow.draw(mUnderscore);

    mWindow.display();
}

