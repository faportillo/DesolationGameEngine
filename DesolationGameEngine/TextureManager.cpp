#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture) {
	SDL_Surface* tmpSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dst, NULL, NULL, flip);
}