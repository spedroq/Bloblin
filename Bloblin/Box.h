#pragma once

#include <Pengine/Vertex.h>
#include <Pengine/SpriteBatch.h>
#include <Pengine/InputManager.h>
#include <Pengine/Camera2D.h>
#include <Pengine/AudioEngine.h>

#include <glm/glm.hpp>

// set the types of blobs, 0 means the grid is empty
const enum BOX_TYPE {
	BLOB_TYPE,
	BOMB_TYPE,
	LASER_TYPE
};
// number of different types of boxes
const int NUM_TYPES_BOXES = 3;

// width of each tile
const int TILE_WIDTH = 32;

// number of pixels moved per step
const float FALL_STEP = 4.0f;
// number of steps to move an entire tile
const float FALL_STEPS_PER_TILE = 8.0f;

// number of pixels moved per step
const float SLIDE_LEFT_STEP = 4.0f;
// number of steps to move an entire tile
const float SLIDE_LEFT_STEPS_PER_TILE = 8.0f;

// number of pixels moved per step
const float SLIDE_RIGHT_STEP = 4.0f;
// number of steps to move an entire tile
const float SLIDE_RIGHT_STEPS_PER_TILE = 8.0f;

// number of breathing steps
const int BREATHING_STEPS = 100;

class Box
{
public:
	Box();
	virtual ~Box();

	// every class that derives from Box needs to implement the update function
	// returns true if box is to be deleted
	virtual bool update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) = 0;

	// every class that derives from Box needs to implement the breathing animation function
	virtual void breatheAnimation() = 0;

	// highlight the box
	// every class that derives from Box needs to implement the highlight animation function
	virtual void highlight() = 0;

	// reset the texture
	// every class that derives from Box needs to implement the reset texture function
	virtual void resetTextureID() = 0;

	// check if the box is being hovered
	// every class that derives from Box needs to implement the check hover function
	bool checkHover(Pengine::InputManager& inputManager, Pengine::Camera2D* camera);

	// perform a step of the fall
	bool fall();

	// perform a step of the slide left
	bool slideLeft();

	// perform a step in the slide right
	bool slideRight();

	// draws the boxes
	void draw(Pengine::SpriteBatch& boxSpriteBatch);

	// increment fall steps
	void addFallSteps(int fallSteps) { _fallSteps += fallSteps; }
	// increment slide left steps
	void addSlideLeftSteps(int slideLeftSteps) { _slideLeftSteps += slideLeftSteps; }

	// out of bounds fall animation
	int outOfBoundsFall();

	// accessors
	int getID() { return _id; }

	glm::vec2 getPosition() { return _position; }

	bool getFalling() { return _falling; }

	int getType() { return _type; }

	int getBoxType() { return _boxType; }

	bool getChecked() { return _checked; }

	bool getHighlighted() { return _highlighted; }

	Pengine::ColorRGBA8 getColor() { return _color; }

	// mutators
	void setPosition(glm::vec2 position) {  _position = position; }

	void setFalling(bool falling) { _falling = falling; }

	void setFallSteps(int fallSteps) { _fallSteps = fallSteps; }

	void setSlidingLeft(bool slidingLeft) { _slidingLeft = slidingLeft; }

	void setSlideLeftSteps(int slideLeftSteps) { _slideLeftSteps = slideLeftSteps; }

	void setSlidingRight(bool slidingRight) { _slidingRight = slidingRight; }

	void setSlideRightSteps(int slideRightSteps) { _slideRightSteps = slideRightSteps; }
	
	void setChecked(bool checked) { _checked = checked; }

	void setHighlighted(bool highlighted) {_highlighted = highlighted; }

	void setBreathing(int breathing) { _breathing = breathing; }

	void setTextureID(int textureID) { _textureID = textureID; }

	void setOutOfBoundsAnimationSteps(int outOfBoundsAnimationSteps) { _outOfBoundsAnimationSteps = outOfBoundsAnimationSteps; }

protected:
	// box id
	int _id;
	// position
	glm::vec2 _position;
	// falling flag
	bool _falling;
	// steps left in the fall
	int _fallSteps;
	// sliding left flag
	bool _slidingLeft;
	// steps left in the slide left
	int _slideLeftSteps;
	// sliding right flag
	bool _slidingRight;
	// steps right in the slide left
	int _slideRightSteps;
	// flag wether box was checked or not
	bool _checked;

	// flag wether the box is highlighted
	bool _highlighted;

	// do the falling annimation from being out of bounds
	int _outOfBoundsAnimationSteps;
	// breathing animation steps
	int _breathing;

	// landing sound effect
	Pengine::SoundEffect _boxLandingSoundEffect;

	// box Color
	Pengine::ColorRGBA8 _color;
	// box color type
	int _type;
	// box color type
	int _boxType;
	// texture id
	int _textureID;

};


