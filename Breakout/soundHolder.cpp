#include "soundHolder.h"

SoundHolder::SoundHolder() {
    mSounds.resize(Sounds::size);
    for (int i = 0; i < Sounds::size; ++i) {
        mSounds[i].reset(new sf::SoundBuffer);
    }
}


