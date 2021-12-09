#include "parser.h"
#include "playstate.h"
#include "textureHolder.h"
#include "soundHolder.h"

#include <string>
#include <cstdlib>


void Parser::parseSounds(SoundHolder& soundHolder) {
    tinyxml2::XMLElement* pXML = mDocument.FirstChildElement();
    auto& mSounds = soundHolder.mSounds;

    uint8_t id = 0;
    while (pXML) {
        if (const char* p = pXML->Attribute("HitSound")) {
            mSounds[id++]->loadFromFile(p);
            if (const char* p = pXML->Attribute("BreakSound")) {
                mSounds[id++]->loadFromFile(p);
            }
            if (pXML->NextSiblingElement()) {
                pXML = pXML->NextSiblingElement();
            }
            else {
                pXML = pXML->Parent()->NextSiblingElement();
            }
        }

        else if (const char* p = pXML->Attribute("BreakSound")) {
            mSounds[id++]->loadFromFile(p);
            if (pXML->NextSiblingElement())
                pXML = pXML->NextSiblingElement();
            else
                pXML = pXML->Parent()->NextSiblingElement();
        }

        else if (pXML->FirstChildElement()) {
            pXML = pXML->FirstChildElement();
        }

        else {
            if (pXML->NextSiblingElement())
                pXML = pXML->NextSiblingElement();
            else
                pXML = pXML->Parent()->NextSiblingElement();
        }

    }
}

void Parser::parseTextures(TextureHolder& textureHolder) {
    tinyxml2::XMLElement* pXML = mDocument.FirstChildElement();
    uint8_t id = 0;
    auto& mTextures = textureHolder.mTextures;

    while (pXML) {
        if (const char* p = pXML->Attribute("Texture")) {
            mTextures[id++]->loadFromFile(p);
            if (pXML->NextSiblingElement())
                pXML = pXML->NextSiblingElement();
            else
                pXML = pXML->Parent()->NextSiblingElement();
        }

        else if (pXML->FirstChildElement("Texture")) {
            pXML = pXML->FirstChildElement("Texture");
            while (true) {
                mTextures[id++]->loadFromFile(pXML->GetText());
                if (pXML->NextSiblingElement("Texture"))
                    pXML = pXML->NextSiblingElement("Texture");
                else
                    break;
            }

            pXML = pXML->Parent()->NextSiblingElement();
        }

        else if (pXML->FirstChildElement()) {
            pXML = pXML->FirstChildElement();
        }

        else {
            if (pXML->NextSiblingElement())
                pXML = pXML->NextSiblingElement();
            else
                pXML = pXML->Parent()->NextSiblingElement();
        }

    }
}

void Parser::setDocument(int level) {
    std::string level_name = "levels/level" + std::to_string(level) + ".xml";
    mDocument.LoadFile(level_name.c_str());
}

void Parser::parseBricks(PlayState* state){

    XMLElement* pXML = mDocument.FirstChildElement("Level");
    state->mRows = std::atoi(pXML->Attribute("RowCount")); 
    state->mColumns = std::atoi(pXML->Attribute("ColumnCount"));
    state->mRowsSpace = std::atoi(pXML->Attribute("RowSpacing"));
    state->mColumnsSpace = std::atoi(pXML->Attribute("ColumnSpacing"));
    pXML = pXML->FirstChildElement("BrickTypes");// Level -> BrickTypes
    pXML = pXML->FirstChildElement("BrickType"); // BrickTypes -> BrickType

    //spremanje svojstava cigli
    int8_t id = 0;
    while (true) {
        if (*(pXML->Attribute("Id")) == 'I')
            break;
        mBrickFeatures[id][Feature::hitPoints] = std::atoi(pXML->Attribute("HitPoints"));
        mBrickFeatures[id][Feature::breakScore] = std::atoi(pXML->Attribute("BreakScore"));
        ++id;
        pXML = pXML->NextSiblingElement();
    }

    // impenetrable svojstva
    mBrickFeatures[id][Feature::hitPoints] = -1;
    mBrickFeatures[id][Feature::breakScore] = 0;

    pXML = pXML->Parent()->ToElement(); // BrickType -> BrickTypes
    pXML = pXML->NextSiblingElement("Bricks"); 
    std::vector<char> letters;
    std::string bricksText(pXML->GetText());
    std::string wantedChars = "HMSI_";

    letters.reserve(bricksText.size());

    // skupimo sva slova iz <Bricks>
    for (const auto& letter : bricksText) {
        if (wantedChars.find(letter) != std::string::npos) {
            letters.push_back(letter);
        }
    }

    auto& mBricks = state->mBricks;
    const auto& mTextureHolder = state->mTextureHolder;

    pXML = pXML->PreviousSiblingElement("BrickTypes"); // Bricks -> BrickTypes
    XMLElement* pXML_backup = pXML;
    mBricks.reserve(letters.size());

    // gradimo cigle
    // emplace_back + new nije pametno ako se desi realokacija
    for(const char& letter : letters){
        switch (letter) {
            case 'S':
                mBricks.emplace_back(mTextureHolder.getTexture(Textures::easyBrick),
                    mBrickFeatures[Bricks::easy][Feature::hitPoints],
                    mBrickFeatures[Bricks::easy][Feature::breakScore], Textures::easyBrick, Sounds::easyHit);
                break;

            case 'M':
                mBricks.emplace_back(mTextureHolder.getTexture(Textures::mediumBrick1),
                    mBrickFeatures[Bricks::medium][Feature::hitPoints],
                    mBrickFeatures[Bricks::medium][Feature::breakScore], Textures::mediumBrick1, Sounds::mediumHit);
                break;

            case 'H':
                mBricks.emplace_back(mTextureHolder.getTexture(Textures::hardBrick1),
                    mBrickFeatures[Bricks::hard][Feature::hitPoints],
                    mBrickFeatures[Bricks::hard][Feature::breakScore], Textures::hardBrick1, Sounds::hardHit);
                break;

            case 'I':
                mBricks.emplace_back(mTextureHolder.getTexture(Textures::impenetrableBrick),
                    mBrickFeatures[Bricks::impenetrable][Feature::hitPoints],
                    mBrickFeatures[Bricks::impenetrable][Feature::breakScore], 
                    Textures::impenetrableBrick, Sounds::impenetrableHit);
                break;
            case '_':
                mBricks.emplace_back();
                break;

        }

    }

}
