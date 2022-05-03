#include "shader.h"			// Include class header
#include <stdio.h>			// Include stdio.h, which contains printf-function
#include <kgfw/GLUtils.h>	// Include GLUtils for checkGLError

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) 
	: Object(__FUNCTION__)
	, m_shaderProgram(0) {
	//std::string vertexCode;
	//std::string fragmentCode;
	//std::ifstream vShaderFile;
	//std::ifstream fShaderFile;

	////ifstream objects can throw exceptions
	//vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	//fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	//try
	//{
	//	//open files
	//	vShaderFile.open(vertexShaderPath);
	//	fShaderFile.open(fragmentShaderPath);

	//	//read file's buffer contents into streams
	//	std::stringstream vShaderStream, fShaderStream;
	//	vShaderStream << vShaderFile.rdbuf();
	//	fShaderStream << fShaderFile.rdbuf();

	//	//close file handlers
	//	vShaderFile.close();
	//	fShaderFile.close();

	//	//convert stream into string
	//	vertexCode = vShaderStream.str();
	//	fragmentCode = fShaderStream.str();
	//}
	//catch (std::ifstream::failure& e)
	//{
	//	std::cout << "ERROR SHADER FILE NOT SUCCESFULLY READ" << std::endl;
	//}

	//const char* vShaderCode = vertexCode.c_str();
	//const char* fShaderCode = fragmentCode.c_str();
	/*std::string vertShaderSource = readFile(vertexShaderPath);
	std::string fragShaderSource = readFile(fragmentShaderPath);

	const char* vertShadersrc = vertShaderSource.c_str();
	const char* fragShadersrc = fragShaderSource.c_str();*/

	// Create and compile vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	checkGLError();
	glShaderSource(vertexShader, 1, &vertexShaderPath, NULL);
	checkGLError();
	glCompileShader(vertexShader);
	checkGLError();

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	checkGLError();
	if (!success) {
		// If failed, get error string using glGetShaderInfoLog-function.
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		checkGLError();
		printf("ERROR: Shader compilation failed: \"%s\"\n", infoLog);
	}

	// Create and compile fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	checkGLError();
	glShaderSource(fragmentShader, 1, &fragmentShaderPath, NULL);
	checkGLError();
	glCompileShader(fragmentShader);
	checkGLError();
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	checkGLError();
	if (!success) {
		// If failed, get error string using glGetShaderInfoLog-function.
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		checkGLError();
		printf("ERROR: Shader compilation failed: \"%s\"\n", infoLog);
	}

	// link shaders
	m_shaderProgram = glCreateProgram();
	checkGLError();
	glAttachShader(m_shaderProgram, vertexShader);
	checkGLError();
	glAttachShader(m_shaderProgram, fragmentShader);
	checkGLError();
	glLinkProgram(m_shaderProgram);
	checkGLError();
	// check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	checkGLError();
	if (!success) {
		// If failed, get error string using glGetProgramInfoLog-function.
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		checkGLError();
		printf("ERROR: Shader link failed: \"%s\"\n", infoLog);
	}

	// After linking, the shaders can be deleted.
	glDeleteShader(vertexShader);
	checkGLError();
	glDeleteShader(fragmentShader);
	checkGLError();
}

Shader::~Shader() {
	// Delete shader program
	glDeleteProgram(m_shaderProgram);
	checkGLError();
}

void Shader::bind() {
	glUseProgram(m_shaderProgram);
	checkGLError();
}

std::string Shader::readFile(const char *filePath)
{
	std::string content;
	std::ifstream filestream(filePath, std::ios::in);

	if (!filestream.is_open())
	{
		std::cerr << "could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}
	std::string line = "";
	while (!filestream.eof())
	{
		std::getline(filestream, line);
		content.append(line + "\n");
	}
	filestream.close();
	return content;
}


void Shader::setUniform(const std::string& name, float x, float y, float z) {
	GLint loc = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (loc < 0) {
		return; // Don't set the uniform value, if it not found
	}
	glUniform3f(loc, x, y, z);
	checkGLError();
}

void Shader::setUniform(const std::string& name, float x, float y, float z, float w) {
	GLint loc = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (loc < 0) {
		return; // Don't set the uniform value, if it not found
	}
	glUniform4f(loc, x, y, z, w);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		glUniform3f(loc, x, y, z); // Set as 3 component value in case of 4 component set failed.
	}

	checkGLError();
}

void Shader::setUniform(const std::string& name, const glm::mat4& m) {
    GLint loc = glGetUniformLocation(m_shaderProgram, name.c_str());
    if (loc < 0) {
        return; // Don't set the uniform value, if it not found
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    checkGLError();
}


void Shader::setUniform(const std::string& name, int value) {
	GLint loc = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (loc < 0) {
		return; // Don't set the uniform value, if it not found
	}
	glUniform1i(loc, value);
	checkGLError();
}

//void Shader::setUniform(const std::string& name, float ambientStrength = 0.1f)
//{
//	GLint loc = glGetUniformLocation(m_shaderProgram, name.c_str());
//	if (loc < 0)
//	{
//		return;
//	}
//	glUniform1i(loc, ambientStrength);
//	checkGLError();
//}

