#pragma once
#include "Box.h"

// Blob class derived from Box
class Blob : public Box
{
public:
	Blob(int id, glm::vec2 position, Pengine::ColorRGBA8 color, int type, Pengine::SoundEffect _boxLandingSoundEffect);
	~Blob();

	// returns true if box is to be deleted
	bool update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) override;

	void breatheAnimation() override;

	void highlight() override;

	void resetTextureID() override;

};

