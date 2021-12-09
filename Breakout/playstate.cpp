#include "playstate.h"
#include "game.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <thread>

void PlayState::setTextures() {
    mPillarLeft.setTexture(mTextureHolder.getTexture(Textures::pillar));
    mPillarRight.setTexture(mTextureHolder.getTexture(Textures::pillar));
    mPillarTop.setTexture(mTextureHolder.getTexture(Textures::pillar2));
    mBackgroundSprite.setTexture(mTextureHolder.getTexture(Textures::background));
    mBall.setTexture(mTextureHolder.getTexture(Textures::ball));
    mPlayer.setTexture(mTextureHolder.getTexture(Textures::player));
}

void PlayState::setPositions() {
    mPillarLeft.setScale(1, Window::height / mPillarLeft.getHeight());
    mPillarRight.setScale(1, Window::height / mPillarRight.getHeight());
    mPillarTop.setScale(Window::width / mPillarTop.getWidth(), 1);
    mPillarLeft.setPosition(mPillarLeft.getWidth() / 2, mPillarLeft.getHeight() / 2);
    mPillarRight.setPosition(Window::width - mPillarRight.getWidth() / 2, Window::height / 2);
    mPillarTop.setPosition(Window::width / 2, mPillarTop.getHeight() / 2);

    float x = mBackgroundSprite.getLocalBounds().width;
    float y = mBackgroundSprite.getLocalBounds().height;
    mBackgroundSprite.setScale(Window::width / x, Window::height / y);

    mPlayer.setPosition(Window::width / 2, Window::height - playerHeightShift);

    float tempTextWidth;
    float tempTextHeight;

    mScoreText->setPosition(mPillarLeft.getWidth(), mPillarTop.getHeight());
    tempTextWidth = mLivesText->getLocalBounds().width;
    mLivesText->setPosition(Window::width - mPillarRight.getWidth() - tempTextWidth, mPillarTop.getHeight());

    tempTextHeight = mLivesText->getLocalBounds().height;
    tempTextWidth = mLevelText->getLocalBounds().width;
    mLevelText->setPosition(Window::width - mPillarRight.getWidth() - tempTextWidth, 
                            mPillarTop.getHeight() + 2 * tempTextHeight);

    tempTextWidth = mCountDownText->getLocalBounds().width;
    tempTextHeight = mCountDownText->getLocalBounds().height;
    mCountDownText->setOrigin(tempTextWidth / 2, tempTextHeight / 2);
    mCountDownText->setPosition(Window::width / 2, Window::height / 2);

    float spaceWidth = Window::width - 2 * mPillarLeft.getWidth();
    float fullBricks = mColumns * brickWidth + (mColumns - 1) * (mColumnsSpace);
    float firstBrickX = mPillarLeft.getWidth() + (spaceWidth - fullBricks) / 2 + (brickWidth / 2);
    float firstBrickY = mPillarTop.getHeight() + firstBrickHeightShift;

    float coordX;
    float coordY;
    for (int idx = 0; idx < mRows; ++idx) {
        for (int idy = 0; idy < mColumns; ++idy) {
            coordX = firstBrickX + idy * (brickWidth + mColumnsSpace);
            coordY = firstBrickY + idx * (brickHeight + mRowsSpace);
            mBricks[idx * mColumns + idy].setPosition(coordX, coordY);
            mMaxScore += mBricks[idx * mColumns + idy].mBreakScore;
        }
    }

    mBallInitialHeight = coordY + ballHeightShift;
    mBall.setPosition(Window::width / 2, mBallInitialHeight);
}

PlayState::PlayState(Game* pGame, int level, int score) :
    GameState(pGame), mWindow(pGame->getWindow()), mLevel(level), mScore(score), mMaxScore(score),
    mBall(initialBallVelocityX, initialBallVelocityY)
    
{
    mFont.loadFromFile("data/fonts/Halo3.ttf");
    mScoreText.reset(new sf::Text("Score: " + std::to_string(mScore), mFont, Font::sizeSmall));
    mLivesText.reset(new sf::Text("Lives: " + std::to_string(mLives), mFont, Font::sizeSmall));
    mLevelText.reset(new sf::Text("Level: " + std::to_string(mLevel), mFont, Font::sizeSmall));
    mCountDownText.reset(new sf::Text(std::to_string(mCountDownValue-1), mFont, Font::sizeBig));

    mParser.setDocument(mLevel);
    mParser.parseTextures(mTextureHolder);
    mParser.parseSounds(mSoundHolder);
    mParser.parseBricks(this);

    setTextures();
    setPositions();

    mCountDownText->setFillColor(sf::Color::Red);
    mWindow->setMouseCursorVisible(false);
    mFont.loadFromFile("data/fonts/Halo3.ttf");

    // prazne cigle su samo bile potrebne pri odredivanju pozicija
    // nakon toga nam vise ne trebaju
    mBricks.erase(std::remove_if(mBricks.begin(), mBricks.end(),
        [](decltype(*mBricks.begin()) pBrick) {
            if (!pBrick.mHitPoints)
                return true;
            else
                return false;
        }), mBricks.end());
}

void PlayState::handleEvents(sf::Event event) {
    switch (event.type) {
        case sf::Event::Closed:
            mWindow->close();
            break;
    }
}

void PlayState::playSound(int sound) {
    std::size_t soundSize = mSounds.size();
    mSounds.resize(soundSize + 1);
    mSounds.emplace_back(mSoundHolder.getSound(static_cast<Sounds::ID>(sound)));
    mSounds.back().play();
}

void PlayState::update(sf::Time dt) {
    // upravljanje brodom
    float playerY = mPlayer.getPosition().y;
    float mouseX = sf::Mouse::getPosition(*mWindow).x;

    if (mouseX > 0 && mouseX < Window::width)
        mPlayer.setPosition(mouseX, playerY);
    else if (mouseX <= 0)
        mPlayer.setPosition(0, playerY);
    else
        mPlayer.setPosition(Window::width, playerY);

    // rjesavanje countdown-a
    if (mCountDownValue == countDownBegin) {
        mClock.restart();
        --mCountDownValue;
        return;
    }

    sf::Time time = mClock.getElapsedTime();
    if (time.asSeconds() > 1 && mCountDownValue) {
        mCountDownText->setString(std::to_string(--mCountDownValue));
        auto lb = mCountDownText->getLocalBounds();
        mCountDownText->setOrigin(lb.width / 2, lb.height / 2);
        mCountDownText->setPosition(Window::width / 2, Window::height / 2);
        if (!mCountDownValue)
            mCountDownText->setString("");
        mClock.restart();
    }

    // ako countdown nije gotov izlazimo
    if (mCountDownValue)
        return;

    // ako je zvuk prestao tada ga brisemo iz spremnika
    if(!mSounds.empty()){
        if (mSounds.front().getStatus() == sf::Sound::Stopped) {
            mSounds.pop_front();
        }
    }
 
    // kolizija ball-brick
    auto it = std::find_if(mBricks.begin(), mBricks.end(),
        [&](decltype(*mBricks.begin()) pBrick) {
            float brickX = pBrick.getPosition().x;
            float brickY = pBrick.getPosition().y;
            HitPlace hitPlace = mBall.checkCollision(brickX, brickY, brickWidth, brickHeight);
            switch (hitPlace) {
                case HitPlace::noHit:
                    return false;
                case HitPlace::bottom:
                    mBall.resetDirection(false, true);
                    return true;
                case HitPlace::right:
                    mBall.resetDirection(true, false);
                    return true;
                case HitPlace::top:
                    mBall.resetDirection(false, true);
                    return true;
                case HitPlace::left:
                    mBall.resetDirection(true, false);
                    return true;
                case HitPlace::topRight:
                    mBall.resetAbsoluteDirection(true, false);
                    return true;
                case HitPlace::topLeft:
                    mBall.resetAbsoluteDirection(false, false);
                    return true;
                case HitPlace::bottomLeft:
                    mBall.resetAbsoluteDirection(false, true);
                    return true;
                case HitPlace::bottomRight:
                    mBall.resetAbsoluteDirection(true, true);
                    return true;
            }
        });

    // rjesavanje kolizije ball-brick
    if (it != mBricks.end()) {
        Brick& brick = *it;

        brick.decreaseHitPoints();
        if (brick.isDead()) {
            playSound(brick.mSoundID + 1);
            mScore += brick.mBreakScore;
            mScoreText->setString("Score: " + std::to_string(mScore));
            mBricks.erase(it);
        }
        else {
            playSound(brick.mSoundID);
            if(brick.canBeBroken())
                brick.setTexture(mTextureHolder.getTexture(static_cast<Textures::ID>(++brick.mTextureID)));
        }
        mBall.smallRandomShift();
        mLastHit = Hit::brick;
    }

    // kolizija ball-pillar
    bool leftPillarCol = mBall.checkCollision(mPillarLeft.getPosition().x, mPillarLeft.getPosition().y, mPillarLeft.getWidth(), mPillarLeft.getHeight());
    bool rightPillarCol = mBall.checkCollision(mPillarRight.getPosition().x, mPillarRight.getPosition().y, mPillarRight.getWidth(), mPillarRight.getHeight());
    bool topPillarCol = mBall.checkCollision(mPillarTop.getPosition().x, mPillarTop.getPosition().y, mPillarTop.getWidth(), mPillarTop.getHeight());

    // rjesavanje kolizije ball-pillar
    if (leftPillarCol) {
        // rjesavanje previse sound-ova
        if (mLastHit == Hit::leftPillar) {
            mBall.update(dt);
            return;
        }
        playSound(Sounds::pillarBounce);
        if(mBall.getVelocity().y < 0)
            mBall.resetAbsoluteDirection(true, false);
        else
            mBall.resetAbsoluteDirection(true, true);
        mLastHit = Hit::leftPillar;
    }

    if (rightPillarCol) {
        // rjesavanje previse sound-ova
        if (mLastHit == Hit::rightPillar) {
            mBall.update(dt);
            return;
        }
        playSound(Sounds::pillarBounce);
        if (mBall.getVelocity().y < 0) 
            mBall.resetAbsoluteDirection(false, false);
        
        else 
            mBall.resetAbsoluteDirection(false, true);
        
        mLastHit = Hit::rightPillar;
    }


    if (topPillarCol) {
        playSound(Sounds::pillarBounce);
        mBall.resetDirection(false, true);
        mLastHit = Hit::topPillar;
    }

    // rjesavanje kolizije ball-player
    if (mBall.checkCollision(mPlayer.getPosition().x, mPlayer.getPosition().y, mPlayer.getWidth(), mPlayer.getHeight())) {
        // rjesavanje previse sound-ova
        if (mLastHit == Hit::player) {
            mBall.update(dt);
            return;
        }

        playSound(Sounds::playerBounce);

        float diff = mPlayer.getPosition().x - mBall.getPosition().x;
        float maxDiff = mPlayer.getWidth() / 2;
        float coordX = (-maxBallVelocityX / maxDiff) * diff;
        // brzina lopte je konstanta
        float coordY = -std::sqrt(std::pow(ballSpeed, 2) - std::pow(coordX, 2));

        mBall.setVelocity(coordX, coordY);

        mLastHit = Hit::player;


    }

    // level je uspjesno zavrsen
    if (mScore == mMaxScore) {
        if (mLevel == maxLevel) {
            mpGame->setPauseState(mScore, mLives, false, true);
        }
        else
            mpGame->setPauseState(mScore, mLives, true, false);
        return;
    }
    
    // lopta je izasla van prostora
    if (mBall.checkCoordinates(Window::width, Window::height)) {
        --mLives;
        if (mLives == 0) {
            mpGame->setPauseState(mScore, mLives, false, true);
            return;
        }
        mBall.setPosition(Window::width / 2, mBallInitialHeight);
        mBall.setVelocity(initialBallVelocityX, initialBallVelocityY);
        mLivesText->setString("Lives: " + std::to_string(mLives));
        mCountDownValue = countDownBegin;
        mCountDownText->setString(std::to_string(mCountDownValue-1));
        auto lb = mCountDownText->getLocalBounds();
        mCountDownText->setOrigin(lb.width / 2, lb.height / 2);
        mCountDownText->setPosition(Window::width / 2, Window::height / 2);
        mpGame->setPauseState(mScore, mLives, false, false);
        mLastHit = Hit::begin;
        return;
    }

    mBall.update(dt);

}

void PlayState::render() {
    mWindow->draw(mBackgroundSprite);

    mWindow->draw(mPlayer);

    mWindow->draw(mPillarLeft);
    mWindow->draw(mPillarRight);
    mWindow->draw(mPillarTop);

    for (auto& brick : mBricks)
        mWindow->draw(brick);

    mWindow->draw(mBall);

    mWindow->draw(*mScoreText);
    mWindow->draw(*mLivesText);
    mWindow->draw(*mLevelText);
    mWindow->draw(*mCountDownText);
}


