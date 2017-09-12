#pragma once

#include "Box.h"

// Bomb class derived from Box
class Bomb : public Box
{
public:
	Bomb(int id, glm::vec2 position, Pengine::ColorRGBA8 color, int type, Pengine::SoundEffect boxLandingSoundEffect);
	~Bomb();

	// returns true if box is to be deleted
	bool update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) override;

	void breatheAnimation() override;

	void highlight() override;

	void resetTextureID() override;

};

