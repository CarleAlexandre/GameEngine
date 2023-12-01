#include "../include/engine.hh"

void LoadTextureAtlas(fl::vec<Texture> *textAtlas) {
	textAtlas->push_back(LoadTexture("assets/texture/button.png"));
	textAtlas->push_back(LoadTexture("assets/texture/highlight.png"));
	textAtlas->push_back(LoadTexture("assets/texture/character.png"));
}

void UnloadTextureAtlas(fl::vec<Texture> &textAtlas) {
	for (uint32_t i = 0; i < textAtlas.size(); i++) {
		UnloadTexture(textAtlas[i]);
	}
}
