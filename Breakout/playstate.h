#pragma once

#include "gamestate.h"
#include "soundHolder.h"
#include "textureHolder.h"
#include "parser.h"
#include "entities.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <deque>

namespace {
	enum Font {
		sizeSmall = 35,
		sizeBig = 100
	};
	enum Hit {
		begin, brick, player, leftPillar,
		rightPillar, topPillar
	};

	// na ekran se pocne prikazivati countDownBegin - 1
	const uint8_t countDownBegin = 4;
	const uint8_t livesBegin = 3;
	const uint8_t maxLevel = 3;

	const float brickWidth = 42.0f;
	const float brickHeight = 14.0f;
	const float playerHeightShift = 30.0f;
	const float firstBrickHeightShift = 200.0f;
	const float initialBallVelocityX = 0.0f;
	const float initialBallVelocityY = 1000.0f;
	const float ballSpeed = 1000.0f;
	const float maxBallVelocityX = 700.0f;
	const float ballHeightShift = 50.0f;
}


class Game;

class PlayState : public GameState{
	friend class Parser;
	
public:
	PlayState(Game* pGame, int level, int score);
	void init() override {};
	void handleEvents(sf::Event event) override;
	void update(sf::Time dt = sf::Time::Zero) override;
	void render() override;

	void setTextures();
	void setPositions();
	void playSound(int sound);

private:
	sf::RenderWindow* mWindow;
	sf::Sprite mBackgroundSprite;
	sf::Font mFont;
	sf::Clock mClock; // za countdown

	TextureHolder mTextureHolder;
	SoundHolder mSoundHolder;
	Parser mParser;

	std::vector<Brick> mBricks;
	std::deque<sf::Sound> mSounds;
	Player mPlayer;
	Pillar mPillarLeft;
	Pillar mPillarRight;
	Pillar mPillarTop;
	Ball mBall;

	unsigned int mScore;
	unsigned int mMaxScore;
	uint8_t mLevel;

	uint8_t mCountDownValue = countDownBegin;
	uint8_t mLives = livesBegin;
	Hit mLastHit = begin;

	std::unique_ptr<sf::Text> mLivesText;
	std::unique_ptr<sf::Text> mScoreText;
	std::unique_ptr<sf::Text> mLevelText;
	std::unique_ptr<sf::Text> mCountDownText;

	float mBallInitialHeight;

	// postavlja parser
	int mRows;
	int mColumns;
	int mRowsSpace;
	int mColumnsSpace;

};
