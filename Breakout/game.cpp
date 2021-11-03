#include "game.h"
#include "playstate.h"

#include <SFML/Graphics.hpp>

Game::Game() :
    mWindow(sf::VideoMode(Window::width, Window::height), "Breakout"),
    mState(GameState::Welcome), mStates(statesNum)
{
    mDtFixed = sf::seconds(dtFixed / 60.0f);
    mStates.at(GameState::Welcome).reset(new WelcomeState(this));
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate >= mDtFixed) {
            timeSinceLastUpdate -= mDtFixed;
            processEvents();
            update(mDtFixed);
        }
        render();
    }
    return;
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStates.at(mState)->handleEvents(event);
    }

}

void Game::update(sf::Time dt) {
    mStates.at(mState)->update(dt);
}

void Game::render() {  
    mWindow.clear();
    mStates.at(mState)->render();
    mWindow.display(); 
}

sf::RenderWindow* Game::getWindow(){
    return &mWindow;
}

void Game::setPauseState(int score, int lives, bool newLevel, bool gameOver) {
    mNewLevel = newLevel;
    mScore = score;
    mState = GameState::Pause;
    if(newLevel)
        mStates.at(mState).reset(new PauseState(this, score, 3, mLevel, gameOver));
    else
        mStates.at(mState).reset(new PauseState(this, score, lives, mLevel, gameOver));
    mStates.at(mState)->update(mDtFixed);
}

void Game::setPlayState() {
    if (mNewLevel) {
        mState = GameState::Playing;
        mNewLevel = false;
        mStates.at(mState).reset(new PlayState(this, ++mLevel, mScore));
        mStates.at(mState)->update(mDtFixed);
    }
    else
        mState = GameState::Playing;
}



void Game::end(){
    mWindow.close();
    return;
}

Game::~Game(){

}
