#include "entities.h"

#include <cmath>

void Entity::setTexture(sf::Texture const& texture) {
    mSprite.setTexture(texture);
    auto lb = mSprite.getLocalBounds();
    mSprite.setOrigin(lb.width / 2, lb.height / 2);

    mRadius = 1.0 * std::min(lb.width / 2, lb.height / 2);
    mCircle.setRadius(mRadius);
    mCircle.setFillColor(sf::Color(0, 255, 0, 0));
    mCircle.setOutlineThickness(1);
    mCircle.setOutlineColor(sf::Color::Yellow);
    mCircle.setOrigin(mRadius, mRadius);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mSprite, states);
    //target.draw(mCircle, states);
} 

float Entity::getWidth() {
    auto lb = mSprite.getLocalBounds();
    return lb.width;
}

float Entity::getHeight() {
    auto lb = mSprite.getLocalBounds();
    return lb.height;
}

Brick::Brick() : mHitPoints(0), mBreakScore(0), mSoundID(0), mTextureID(0)
{}

Brick::Brick(const sf::Texture& texture, int hit, int score, int textureID, int soundHitID) :
    mHitPoints(hit), mBreakScore(score), mTextureID(textureID), mSoundID(soundHitID)
{
    setTexture(texture);
}

void Brick::decreaseHitPoints() {
    if (mHitPoints > 0)
        --mHitPoints;
}

bool Brick::isDead() {
    return mHitPoints ? false : true;
}

bool Brick::canBeBroken() {
    return mHitPoints >= 0 ? true : false;
}

void Ball::update(sf::Time dt) {
    auto pos = getPosition();
    float x = pos.x + mVelocity.x * dt.asSeconds();
    float y = pos.y + mVelocity.y * dt.asSeconds();
    setPosition(x, y);
}


bool Ball::checkCoordinates(float window_width, float window_height) {
    float ballY = getPosition().y;

    if (ballY > window_height)
        return true;
    if (ballY < 0)
        return true;

    return false;
}

HitPlace Ball::checkCollision(float rect_x, float rect_y, float rect_width, float rect_height) {
    float circleDistanceX = std::abs(getPosition().x - rect_x);
    float circleDistanceY = std::abs(getPosition().y - rect_y);

    if (circleDistanceX > (rect_width / 2 + mRadius)) { return HitPlace::noHit; }
    if (circleDistanceY > (rect_height / 2 + mRadius)) { return HitPlace::noHit; }

    if (circleDistanceX <= (rect_width / 2)) {
        if (getPosition().y < rect_y)
            return HitPlace::top;
        else return HitPlace::bottom;
    }

    if (circleDistanceY <= (rect_height / 2)) {
        if (getPosition().x > rect_x)
            return HitPlace::right;
        else return HitPlace::left;
    }

    float cornerDistance_sq = std::pow((circleDistanceX - rect_width / 2), 2) +
        std::pow((circleDistanceY - rect_height / 2), 2);

    if (cornerDistance_sq <= (mRadius * mRadius)) {
        if (getPosition().x > rect_x && getPosition().y < rect_y)
            return HitPlace::topRight;
        if (getPosition().x < rect_x && getPosition().y < rect_y)
            return HitPlace::topLeft;
        if (getPosition().x < rect_x && getPosition().y > rect_y)
            return HitPlace::bottomLeft;
        if (getPosition().x > rect_x && getPosition().y > rect_y)
            return HitPlace::bottomRight;
    }
    else
        return HitPlace::noHit;
}

Ball::Ball(float velocityX, float velocityY) {
    mVelocity.x = velocityX;
    mVelocity.y = velocityY;
}

void Ball::resetDirection(bool coordX, bool coordY) {
    if(coordX)
        mVelocity.x = mVelocity.x - 2 * mVelocity.x;
    if(coordY)
        mVelocity.y = mVelocity.y - 2 * mVelocity.y;
}

void Ball::resetAbsoluteDirection(bool coordX, bool coordY) {
    if (coordX)
        mVelocity.x = std::abs(mVelocity.x);
    else
        mVelocity.x = -std::abs(mVelocity.x);
    if (coordY)
        mVelocity.y = std::abs(mVelocity.y);
    else
        mVelocity.y = -std::abs(mVelocity.y);
}

void Ball::setVelocity(float coordX, float coordY) {
    mVelocity.x = coordX;
    mVelocity.y = coordY;
}

sf::Vector2f Ball::getVelocity() {
    return mVelocity;
}

float Pillar::getWidth() {
    auto lb = mSprite.getLocalBounds();
    return lb.width* getScale().x;
}

float Pillar::getHeight() {
    auto lb = mSprite.getLocalBounds();
    return lb.height * getScale().y;
}