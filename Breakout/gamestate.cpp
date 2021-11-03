
#include "gamestate.h"
#include "game.h"
#include "playstate.h"

#include <string>

GameState::GameState() : mpGame(nullptr) {

}

GameState::GameState(Game* pGame) : mpGame(pGame) {

}

WelcomeState::WelcomeState(Game* pGame) : GameState(pGame)

{
	mFont.loadFromFile("data/fonts/Halo3.ttf");
	mTitle.reset(new sf::Text("Breakout clone by D.Dragas", mFont, Font::sizeSmall));
	mNote.reset(new sf::Text("Press enter to continue", mFont, Font::sizeSmall));

	auto lb1 = mTitle->getLocalBounds();
	auto lb2 = mNote->getLocalBounds();
	mTitle->setOrigin(lb1.width / 2, lb1.height / 2);
	mNote->setOrigin(lb2.width / 2, lb2.height / 2);

	mTitle->setPosition(Window::width / 2, Window::height / 2);
	mNote->setPosition(Window::width / 2, mTitle->getPosition().y + lb1.height);
}

void WelcomeState::handleEvents(sf::Event event){
	//Promijeni u switch naredbu
	if (event.type == sf::Event::KeyPressed){
		switch (event.key.code) {
		case sf::Keyboard::Key::Escape:
			mpGame->end();
			break;
		case sf::Keyboard::Key::Return:
			mpGame->setPlayState();
			break;
		}
	}
	if (event.type == sf::Event::Closed){
		mpGame->end();
	}
	return;
}

void WelcomeState::update(sf::Time dt){
	return;
}

void WelcomeState::render(){
	(mpGame->getWindow())->draw(*mTitle);
	(mpGame->getWindow())->draw(*mNote);
	return;
}

PauseState::PauseState(Game* pGame, int score, int lives, int level, bool gameOver) :
	GameState(pGame), mLives(lives), mScore(score), mLevel(level), mGameOver(gameOver)
{
	mFont.loadFromFile("data/fonts/Halo3.ttf");
	mScoreText.reset(new sf::Text("Score: " + std::to_string(mScore), mFont, Font::sizeSmall));
	mLivesText.reset(new sf::Text("Lives: " + std::to_string(mLives), mFont, Font::sizeSmall));
	mNote.reset(new sf::Text("Press Enter to continue", mFont, Font::sizeSmall));

	auto lb1 = mScoreText->getLocalBounds();
	auto lb2 = mLivesText->getLocalBounds();
	
	mScoreText->setOrigin(lb1.width / 2, lb1.height / 2);
	mLivesText->setOrigin(lb2.width / 2, lb2.height / 2);

	if (gameOver && mLives)
		mNote->setString("Congratulations!");
	else if(gameOver)
		mNote->setString("Game over!");

	auto lb3 = mNote->getLocalBounds();
	mNote->setOrigin(lb3.width / 2, lb3.height / 2);

	mScoreText->setPosition(Window::width/2, Window::height / 2);
	mLivesText->setPosition(Window::width / 2, mScoreText->getPosition().y + lb1.height);
	mNote->setPosition(Window::width / 2, mLivesText->getPosition().y + lb2.height);

}

void PauseState::handleEvents(sf::Event event){
	if (event.type == sf::Event::KeyPressed){
		switch(event.key.code){
			case sf::Keyboard::Key::Escape:
				mpGame->end();
				break;
			case sf::Keyboard::Key::Return:
				if(!mGameOver)
					mpGame->setPlayState();
				break;
		}	
	}
	if (event.type == sf::Event::Closed ){
		mpGame->end();
	}
	return;
}

void PauseState::update(sf::Time dt){
	return;
}

void PauseState::render(){
	(mpGame->getWindow())->draw(*mScoreText);
	if(mLives)
		(mpGame->getWindow())->draw(*mLivesText);
	(mpGame->getWindow())->draw(*mNote);
	return;
}
