#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

enum HitPlace : uint8_t {
    noHit, bottom, right, top, left,
    topRight, topLeft,
    bottomLeft, bottomRight
};


class Entity : public sf::Drawable, public sf::Transformable {
    friend class PlayState;
public:
    Entity() : mRadius(0) {}
    void setTexture(sf::Texture const& texture);
    virtual float getHeight();
    virtual float getWidth();

    virtual void update(sf::Time dt = sf::Time::Zero) {};
    virtual ~Entity() {}
protected:
    sf::Sprite mSprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::CircleShape mCircle;
    float mRadius;
};


class Player : public Entity {
public:
    Player() {};

};

class Brick : public Entity {
    friend class PlayState;
public:
    Brick();
    Brick(const sf::Texture& texture, int hit, int score, int textureID, int soundHitID);
    void decreaseHitPoints();
    bool isDead();
    bool canBeBroken();

private:
    int mHitPoints;
    int mBreakScore;
    int mTextureID;
    int mSoundID;
};


class Ball : public Entity {
public:
    Ball(float velocityX, float velocityY);
    bool checkCoordinates(float window_width, float window_height);
    HitPlace checkCollision(float rect_x, float rect_y, float rect_width, float rect_height);

    void update(sf::Time dt = sf::Time::Zero) override;
    void resetDirection(bool coordX, bool coordY);
    void resetAbsoluteDirection(bool coordX, bool coordY);
    void setVelocity(float coordX, float coordY);
    void smallRandomShift();

    sf::Vector2f getVelocity();
private:
    sf::Vector2f mVelocity;

    std::random_device mDevice;
    std::default_random_engine mEngine;
    std::uniform_int_distribution<int> mRandomGenerator;
};

class Pillar : public Entity {
public:
    Pillar() {};
    float getHeight() override;
    float getWidth() override;

};
