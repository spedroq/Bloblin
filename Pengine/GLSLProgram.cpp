#include "GLSLProgram.h"

// include the errors file
#include "PengineErrors.h"

// read the file
#include <fstream>
#include <vector>

namespace Pengine {
	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	// compile the shaders
	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.
		//Get a program object.
		_programID = glCreateProgram();

		// https://www.khronos.org/opengl/wiki/Shader_Compilation for further documentation
		// set the type of shader
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		// catch any errors
		if (_vertexShaderID == 0) {
			fatalError("Vertex shader failed to be created!");
		}

		// set the type of shader
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		// catch any errors
		if (_fragmentShaderID == 0) {
			// print off the error
			fatalError("Fragment shader failed to be created!");
		}

		// compile the shaders
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);



	}

	// link the shaders
	void GLSLProgram::linkShaders() {

		//Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		// error checking
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			// print out the errors
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link!");
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		// free up resources
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	// compile shader
	void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {

		// specify the files
		std::ifstream vertexFile(filePath);
		// check for any errors
		if (vertexFile.fail()) {
			// print off the errors
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		// read each line in the file
		while (std::getline(vertexFile, line)) {
			// store it in the fileContents
			fileContents += line + "\n";
		}

		// close the file
		vertexFile.close();
		const char* constentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &constentsPtr, nullptr);
		// compile the shader
		glCompileShader(id);

		// shader error handling
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog and exit with failure
			glDeleteShader(id); // Don't leak the shader.

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile!");
		}
	}

	// add attribute from glsl
	void GLSLProgram::addAttribute(const std::string& attributeName) {
		// add the attribute, then increment _numAttributes after the line of code (_numAttributes++)
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	// get the uniform location
	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {

		// get the uniform location
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());

		// error checking
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}

		return location;

	}

	void GLSLProgram::use() {
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}
}