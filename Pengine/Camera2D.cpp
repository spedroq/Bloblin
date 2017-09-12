#include "Camera2D.h"

namespace Pengine {

	Camera2D::Camera2D() : _position(0.0f, 0.0f), _cameraMatrix(1.0f), _orthoMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(true), _screenWidth(500), _screenHeight(500)
	{


	}


	Camera2D::~Camera2D()
	{
	}

	// initialize the camera
	void Camera2D::init(int screenWidth, int screenHeight) {
		// set the screen dimentions
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		// init the ortho matrix
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}


	// update camera
	void Camera2D::update() {

		// check if the camera needs to be updated
		if (_needsMatrixUpdate) {

			// Camera Translation
			// translate the camera accordingly
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			// use translate to move the entire view port
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			// Camera Scale
			// scale the view accordingly
			glm::vec3 scale(_scale, _scale, 0.0f);
			// use scale to scale the entire view port
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			

			// reset flag
			_needsMatrixUpdate = false;
		}
	}

	// convert screen coordinates to world coordinates
	// receives screen coordinates
	// returns world coordinates
	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {

		// invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;

		// convert coordinate system (so that 0 is the center)
		// from 0,0 -> _screenWidth, _screenHeight
		// to -_screenWidth/2, -_screenHeight/2 -> _screenWidth/2, _screenHeight/2
		screenCoords -= glm::vec2(_screenWidth / 2.0f, _screenHeight / 2.0f);

		// camera scale
		screenCoords /= _scale;

		// translate with the camera position
		screenCoords += _position;

		// return the world coordinates
		return screenCoords;
	}

}



