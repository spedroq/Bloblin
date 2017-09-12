#pragma once
// basic glm includes, vectors etc
#include <glm/glm.hpp>
// glm ortho 
#include <glm/gtc/matrix_transform.hpp>

namespace Pengine {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		// update
		void update();
		// initializer
		void init(int screenWidth, int screenHeight);

		// get the world coordinates from screen coordinates
		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		// Mutators
		void setPosition(const glm::vec2& newPosition) { _position = newPosition; _needsMatrixUpdate = true; }
		void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }

		// Accessors
		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

	private:
		// screen size
		int _screenWidth, _screenHeight;

		// flag that tells us when to update the camera
		bool _needsMatrixUpdate;
		// scaling factor
		float _scale;
		// position vector
		glm::vec2 _position;
		// combined camera matrix
		glm::mat4 _cameraMatrix;
		// orthographic projection matrix
		glm::mat4 _orthoMatrix;
	};


}
