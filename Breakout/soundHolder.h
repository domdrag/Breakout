#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector> 
#include <memory>

namespace Sounds {
    enum ID {
        playerBounce, pillarBounce,
        easyHit, easyBreak,
        mediumHit, mediumBreak,
        hardHit, hardBreak,
        impenetrableHit,
        size
    };
}

class SoundHolder {
    friend class Parser;
public:
    SoundHolder();
    sf::SoundBuffer& getSound(Sounds::ID i) { return *mSounds[i]; }
private:
    std::vector<std::unique_ptr<sf::SoundBuffer> >  mSounds;
};

