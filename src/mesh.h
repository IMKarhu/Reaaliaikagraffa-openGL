#pragma once
#include "gameobject.h"
#include <glad/gl.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


#include <vector>

class Shader;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

class Mesh : public GameObject
{
public:

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	void render(Shader* shader, glm::vec3 viewPos, const glm::mat4& viewMatrix = glm::mat4(1.0f), const glm::mat4& projectionMatrix = glm::mat4(1.0f), GLuint textureId = 0);

	
private:
	GLuint m_VAO;
	GLuint m_EBO;
	GLuint m_VBO;
	size_t m_indiceCount;
	
};