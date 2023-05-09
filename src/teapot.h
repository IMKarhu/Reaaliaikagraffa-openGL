#pragma once
#include "gameobject.h"     // Include base class
#include "teapot_data.h"
#include "shader.h"
#include <glad/gl.h>		// Include glad

class Shader;

class Teapot : public GameObject {
public:
	Teapot();
	~Teapot();

    void render(Shader* shader, glm::vec3 viewPos, const glm::mat4& viewMatrix = glm::mat4(1.0f), const glm::mat4& projectionMatrix = glm::mat4(1.0f), GLuint textureId = 0);

private:
	GLuint m_positionsVbo;
	GLuint m_vao;
	GLuint m_texCoordsVbo;
	GLuint m_normalsVbo;// TODO: decladre handles to the VAO, and VBOs to positions, normals and texture coordinates.
	
};
