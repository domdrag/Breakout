#include "textureHolder.h"

TextureHolder::TextureHolder() {
    mTextures.resize(Textures::size);
    for (int i = 0; i < Textures::size; ++i) {
        mTextures[i].reset(new sf::Texture);
    }
}



 