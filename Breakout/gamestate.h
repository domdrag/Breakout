#pragma once

#include <SFML/Graphics.hpp>

class Game;

class GameState
{
public:
    GameState();
    GameState(Game* pGame);
    virtual ~GameState() {}
    enum State{
	    Welcome,
        Playing,
        Pause,
        SIZE
    };
    virtual void init() = 0;
    virtual void handleEvents(sf::Event event) = 0;
    virtual void update(sf::Time dt = sf::Time::Zero) = 0;
    virtual void render() = 0;

protected:
    Game* const mpGame; 
       
};


class WelcomeState : public GameState{
public:
    WelcomeState(Game* pGame);
    void init() override {};
    void handleEvents(sf::Event event) override;
    void update(sf::Time dt = sf::Time::Zero) override;
    void render() override;

private:
    sf::Font mFont;
    std::unique_ptr<sf::Text> mTitle;
    std::unique_ptr<sf::Text> mNote;
};

class PauseState : public GameState{
public:
    PauseState(Game* pGame, int score, int lives, int level, bool gameOver);
    void init() override {};
    void handleEvents(sf::Event event) override;
    void update(sf::Time dt = sf::Time::Zero) override;
    void render() override;

private:
    int mLives;
    int mScore;
    int mLevel;
    bool mGameOver;

    sf::Font mFont;
    std::unique_ptr<sf::Text> mScoreText; 
    std::unique_ptr<sf::Text> mLivesText;
    std::unique_ptr<sf::Text> mNote;

};
