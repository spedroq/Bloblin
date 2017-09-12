#pragma once
#include <string>
#include <Windows.h>
// including glew.h sets up glew and includes all the other files needed for openGL
#include <GL/glew.h>

namespace Pengine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		// compile the shaders
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		// link the shaders
		void linkShaders();
		// add attribute from gl_sl
		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);


		void use();
		void unuse();

	private:

		// number of attributes
		int _numAttributes;

		// compile shaders
		void compileShader(const std::string& filePath, GLuint shaderID);

		// finished product
		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}

