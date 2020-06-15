#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, dstRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:
	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(const char* path) {
		setTex(path);
	}

	SpriteComponent(const char* path, bool isAnimated) {
		animated = isAnimated;

		Animation idle_down = Animation(0, 1, 100);
		Animation walk_down = Animation(0, 3, 100);
		Animation idle_left = Animation(1, 1, 100);
		Animation walk_left = Animation(1, 3, 100);
		Animation idle_right = Animation(2, 1, 100);
		Animation walk_right = Animation(2, 3, 100);
		Animation idle_up = Animation(3, 1, 100);
		Animation walk_up = Animation(3, 3, 100);

		animations.emplace("IdleDown", idle_down);
		animations.emplace("WalkDown", walk_down);
		animations.emplace("IdleLeft", idle_down);
		animations.emplace("WalkLeft", walk_left);
		animations.emplace("IdleRight", idle_down);
		animations.emplace("WalkRight", walk_right);
		animations.emplace("IdleUp", idle_down);
		animations.emplace("WalkUp", walk_up);

		play("IdleDown");

		setTex(path);
	}

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path) {

		texture = TextureManager::LoadTexture(path);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {
		if (animated) {
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		srcRect.y = animIndex * transform->height;

		dstRect.x = static_cast<int>(transform->position.x);
		dstRect.y = static_cast<int>(transform->position.y);
		dstRect.w = transform->width * transform->scale;
		dstRect.h = transform->height * transform->scale;
	}

	void draw() override {
		TextureManager::draw(texture, srcRect, dstRect, spriteFlip);
	}

	void play(const char* animName) {
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};