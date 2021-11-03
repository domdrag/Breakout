#pragma once

#include <SFML/Graphics.hpp>
#include <vector> 
#include <memory>

namespace Textures {
    enum ID {
        background, player, ball, pillar, pillar2,
        easyBrick, mediumBrick1, mediumBrick2,
        hardBrick1, hardBrick2, hardBrick3, impenetrableBrick,
        size
    };
}

class TextureHolder {
    friend class Parser;
public:
    TextureHolder();
    sf::Texture& getTexture(Textures::ID i) const { return *mTextures[i]; }
private:
    std::vector<std::unique_ptr<sf::Texture> >  mTextures;
}; 
