#include "teapot.h"
#include <kgfw/GLUtils.h>           // Include GLUtils for checkGLError
#include "shader.h"
#include <glm/gtx/transform.hpp>	// glm transform functions.
#include <vector>
#include "teapot_data.h"


Teapot::Teapot()
    : GameObject(__FUNCTION__)
{
	// TODO:
	glGenVertexArrays(1, &m_vao);// 1. Create VAO
	checkGLError();
	glGenBuffers(1, &m_positionsVbo);// 2. Create VBO for positions. Use TEAPOT_POSITIONS -variable (declader in teapot_data.h) for positions data.
	checkGLError();
	glGenBuffers(1, &m_texCoordsVbo);// 3. Create VBO for texture coordinates Use TEAPOT_TEXTURE_COORDINATES -variable (declader in teapot_data.h) 
	checkGLError();//    for texture coordinates data.
	glGenBuffers(1, &m_normalsVbo);// 4. Create VBO for normals. Use TEAPOT_NORMALS variable (declader in teapot_data.h) 
	checkGLError();//    for normals data.

	//Bind vertex array object
	glBindVertexArray(m_vao);
	checkGLError();
	//set buffer data to m_vbo object(bind first, then set the data)
	glBindBuffer(GL_ARRAY_BUFFER, m_positionsVbo);
	checkGLError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(TEAPOT_POSITIONS), TEAPOT_POSITIONS, GL_STATIC_DRAW);
	checkGLError();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	checkGLError();
	glEnableVertexAttribArray(0);
	checkGLError();

	//set buffer data to m_texcoordsVbo(same here: bind first, then set the data)
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsVbo);
	checkGLError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(TEAPOT_TEXTURE_COORDINATES), TEAPOT_TEXTURE_COORDINATES, GL_STATIC_DRAW);
	checkGLError();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	checkGLError();
	glEnableVertexAttribArray(1);
	checkGLError();

	//set buffer data to m_normalsVbo(same here: bind first, then set the data)
	glBindBuffer(GL_ARRAY_BUFFER, m_normalsVbo);
	checkGLError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(TEAPOT_NORMALS), TEAPOT_NORMALS, GL_STATIC_DRAW);
	checkGLError();
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	checkGLError();
	glEnableVertexAttribArray(2);
	checkGLError();
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//checkGLError();
	//glBindVertexArray(0);
	//checkGLError();
}

Teapot::~Teapot()
{
	// TODO: Delete created VAO and VBOs here.
	// Delete Vertex Buffer Object
	glDeleteBuffers(1, &m_positionsVbo);
	checkGLError();
	glDeleteBuffers(1, &m_texCoordsVbo);
	checkGLError();
	glDeleteBuffers(1, &m_normalsVbo);
	checkGLError();
	// Delete Vertex Array Object
	glDeleteVertexArrays(1, &m_vao);
	checkGLError();
}

void Teapot::render(Shader* shader, glm::vec3 viewPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint textureId)
{
	shader->bind();// TODO: Bind the shader
	shader->setUniform("lightPos", 0.0f, 1.0f, 0.0f);
	shader->setUniform("LightColor", 1.0f, 1.0f, 1.0f);
	shader->setUniform("viewPos", viewPos.x, viewPos.y, viewPos.z);
	shader->setUniform("MVP", projectionMatrix * glm::inverse(viewMatrix) * getModelMatrix());// TODO: Set uniform values to the shader (MVP Matrix)
	shader->setUniform("M", getModelMatrix());
   	
	if (textureId > 0)	// TODO: Set texture uniform to the shader
	{
		shader->setUniform("texture0", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	glBindVertexArray(m_vao);	// TODO: Bind VAO.
	checkGLError();

	glDrawArrays(GL_TRIANGLES, 0, TEAPOT_NUM_VERTICES);	// TODO: Draw teapot using glDrawArrays with TEAPOT_NUM_VERTICES.
	checkGLError();
}

