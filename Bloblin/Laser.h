#pragma once

#include "Box.h"

// Laser class derived from Box
class Laser : public Box
{
public:
	Laser(int id, glm::vec2 position, Pengine::ColorRGBA8 color, int type, Pengine::SoundEffect boxLandingSoundEffect);
	~Laser();

	// returns true if box is to be deleted
	bool update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) override;

	void breatheAnimation() override;

	void highlight() override;

	void resetTextureID() override;

};

