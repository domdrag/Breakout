#pragma once

#include "tinyxml2.h"

using namespace tinyxml2;

class PlayState;
class TextureHolder;
class SoundHolder;

namespace {
	enum Bricks {
		easy, medium, hard, impenetrable
	};
	enum Feature {
		hitPoints, breakScore
	};
}

class Parser {
public:
	Parser() : mBrickFeatures{} {}
	void setDocument(int level);
	void parseBricks(PlayState* state);
	void parseTextures(TextureHolder& textureHolder);
	void parseSounds(SoundHolder& soundHolder);

private:
	XMLDocument mDocument;
	int16_t mBrickFeatures[4][2];

};