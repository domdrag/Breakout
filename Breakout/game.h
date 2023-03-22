#pragma once

#include "gamestate.h"

#include <SFML/Graphics.hpp>
#include <vector>

namespace {
	enum Window  {
		width = 700, height = 900
	};
	const float dtFixed = 0.2f;
	const uint8_t statesNum = 3;
}

class Game {

public:
	Game();
	~Game();
	void run();
	sf::RenderWindow* getWindow();
	void setPlayState();
	void setPauseState(int score, int lives, bool newLevel, bool gameOver);
	void end();

private:
	void processEvents();
	void update(sf::Time dt);
	void render();

	sf::RenderWindow mWindow;
	sf::Time mDtFixed;
	std::vector<std::unique_ptr<GameState> > mStates;
	GameState::State mState;

	bool mNewLevel = true;
	int mLevel = 2;
	int mScore = 0;
};

