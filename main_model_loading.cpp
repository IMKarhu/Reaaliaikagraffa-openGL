#include <stdio.h>			// Include stdio.h, which contains printf-function
#include <kgfw/Object.h>	// Include kgfw::Object
#include <glad/gl.h>		// Include glad
#include <GLFW/glfw3.h>		// Include glfw
#include <kgfw/GLUtils.h>	// Include GLUtils for checkGLError
#include "shader.h"			// Include Shader-class.
#include <vector>			// Include std::vector
#include "camera.h"			// Include Camera-class.
#include "teapot.h"			// Include Teapot-class.
#include "texture.h"		// Include Texture-class.
#include "mesh.h"	
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>// Include Mesh-class.
#include <iostream>
// Include STB-image library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 480;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Mesh* processMesh(aiMesh* mesh)
{
	std::vector<Vertex> vertices;//TODO 1: add data containers for vertices and indices
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex; //temporable container for the data of each loop
		glm::vec3 vector;//TODO 2: load data from the Assimp mesh to our containers

		vector.x = mesh->mVertices[i].x;
		vector.y= mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	//Now retrieve the corresponding vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	return new Mesh(vertices, indices);//TODO 3: return with new Mesh of our own
}
void processNode(std::vector<Mesh*>* meshes, aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		// the node object only contains indices to index the actual objects in the scene.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes->push_back(processMesh(mesh));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(meshes, node->mChildren[i], scene);
	}
}

std::vector<Mesh*> loadMeshes(const std::string& path)
{
	std::vector<Mesh*> meshes;//TODO 1: create the container that will be returned by this function
	//read file with Assimp
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("Error loading model file \"%s\": \"%s\" ", path.c_str(), importer.GetErrorString());
		return meshes;
	}
	// retrieve the directory path of the filepath
	std::string dir = path.substr(0, path.find_last_of('/'));
	//TODO 2: process Assimp's root node recursively
	processNode(&meshes, scene->mRootNode, scene);
	return meshes;
}


class Application : public kgfw::Object
{
public:
	Application()
		: Object(__FUNCTION__)
		, m_shader(0) {
		const char* vertexShaderSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 in_position;\n"
			"layout (location = 1) in vec2 in_texCoord;\n"
			"layout (location = 2) in vec3 in_normal;\n"
			"uniform mat4 MVP;\n"
			"uniform mat4 M;\n"
			"out vec3 normal;"
			"out vec2 texCoord;"
			"out vec3 fragPos;"
			"out vec3 viewPos;"
			"void main()\n"
			"{\n"
			"   texCoord = in_texCoord;\n"
			"   gl_Position = MVP * vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
			"	fragPos = vec3(M * vec4(in_position, 1.0));\n"
			"	normal = mat3(transpose(inverse(M))) * in_normal;\n"
			"}";
	
		const char* fragmentShaderSource =
			"#version 330 core\n"
			"uniform sampler2D texture0;\n"
			"uniform vec3 lightPos;\n"
			"uniform vec3 viewPos;\n"

			"in vec3 normal;"
			"in vec2 texCoord;"
			"in vec3 fragPos;\n"
			"out vec4 FragColor;\n"

			"void main()\n"
			"{\n"
			//ambient
			"vec3 lightColor = vec3(1.0);\n"
			"vec4 objectColor = texture2D(texture0, texCoord);\n"
			"float ambientStrength = 0.1;\n"
			"vec3 ambient = ambientStrength * lightColor;\n"
			//diffuse
			"vec3 lightDir = normalize(lightPos - fragPos);\n"
			"vec3 norm = normalize(normal);\n"
			"float diff = max(dot(norm, lightDir), 0.0);\n"
			"vec3 diffuse = diff * lightColor;\n"
			//specular
			"float specularStrength = 0.8;\n"
			"vec3 viewDir = normalize(viewPos - fragPos);\n"
			"vec3 reflectDir = reflect(-lightDir, norm);\n"
			"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);\n"
			"vec3 specular = specularStrength * spec * lightColor;\n"

			"vec3 result = (ambient + diffuse) * objectColor.xyz + specular;\n"
			"   FragColor = vec4(result, objectColor.w);\n"
			"}";

		// Build and compile our shader program
		m_shader = new Shader(vertexShaderSource, fragmentShaderSource);

		// Create ortho-projective camera with screen size 640x480
		m_camera = new Camera();
		//// Set camera transform (view transform)
		m_camera->setPosition(glm::vec3(0.0f, 0.4f, 0.4f));
		m_camera->setLookAt();

		auto meshes = loadMeshes("../torus.obj");
		m_meshes.push_back(meshes[0]);
		m_meshes[0]->setScaling(glm::vec3(0.005f));
		m_meshes[0]->setPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
		meshes = loadMeshes("../torusknot.obj");
		m_meshes.push_back(meshes[0]);
		m_meshes[1]->setScaling(glm::vec3(0.02f));
		m_meshes[1]->setPosition(glm::vec3(-0.5f, 0.0f, -1.0f));
		meshes = loadMeshes("../suzanne.obj");
		m_meshes.push_back(meshes[0]);
		m_meshes[2]->setScaling(glm::vec3(0.03f));
		m_meshes[2]->setPosition(glm::vec3(0.5f, 0.0f, 0.0f));
		meshes = loadMeshes("../cylinder.obj");
		m_meshes.push_back(meshes[0]);
		m_meshes[3]->setScaling(glm::vec3(0.03f));
		m_meshes[3]->setPosition(glm::vec3(0.5f, 0.0f, -1.0f));
		meshes = loadMeshes("../Cube.obj");
		m_meshes.push_back(meshes[0]);
		m_meshes[4]->setScaling(glm::vec3(1.0f));
		m_meshes[4]->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	
		// Load the data for our texture using stb-image stbi_load-function
		int width, height, nrChannels;
		GLubyte* data = stbi_load("fabric_pattern_07_col_1_2k.png", &width, &height, &nrChannels, 0);
		m_textures.push_back(new Texture(width, height, nrChannels, data));
		 data = stbi_load("Wood_table_1k.png", &width, &height, &nrChannels, 0);
		 m_textures.push_back(new Texture(width, height, nrChannels, data));
		 data = stbi_load("Wood_table_2k.png", &width, &height, &nrChannels, 0);
		 m_textures.push_back(new Texture(width, height, nrChannels, data));
		 data = stbi_load("red_bricks_04_diff_2k.png", &width, &height, &nrChannels, 0);
		 m_textures.push_back(new Texture(width, height, nrChannels, data));
		 data = stbi_load("Ground.png", &width, &height, &nrChannels, 0);
		 m_textures.push_back(new Texture(width, height, nrChannels, data));
		


		// Enable depth buffering
		glEnable(GL_DEPTH_TEST);

		
	}

	~Application() {
		// Delete shader
		delete m_shader;
		m_shader = 0;

		// Delete Camera
		delete m_camera;
		m_camera = 0;

		// Delete teapot
		for (auto teapot : m_meshes)
		{
			delete teapot;
		}
		
		//delete textures
		for (auto texture : m_textures)
		{
			delete texture;
		}
	}

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);;

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		m_camera->ProcessMouseMovement(xoffset, yoffset);
	}

	void processInput(GLFWwindow *window, float deltaTime)
	{
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_camera->processKeyboard(FORWARD, deltaTime);
			printf("W pressed\n");
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_camera->processKeyboard(BACKWARD, deltaTime);
			printf("S pressed\n");
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_camera->processKeyboard(LEFT, deltaTime);
			printf("A pressed\n");
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_camera->processKeyboard(RIGHT, deltaTime);
			printf("D pressed\n");
		}
		
	}

	void render(GLFWwindow* window) {
		// Query the size of the framebuffer (window content) from glfw.
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Setup the opengl wiewport (i.e specify area to draw)
		glViewport(0, 0, width, height);
		checkGLError();
		// Set color to be used when clearing the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		checkGLError();
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkGLError();
		glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 0.0f);
		
			m_meshes[0]->render(m_shader, m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_textures[0]->getTextureId());
			m_meshes[1]->render(m_shader, m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_textures[1]->getTextureId());
			m_meshes[2]->render(m_shader, m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_textures[2]->getTextureId());
			m_meshes[3]->render(m_shader, m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_textures[3]->getTextureId());
			m_meshes[4]->render(m_shader, m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_textures[4]->getTextureId());
		
		
	}

	void update(float deltaTime) {
		//m_teapot->setRotationZ(m_teapot->getRotationZ() + deltaTime);
		
	}

private:
	Shader*						m_shader;		// Pointer to the Shader object
	Camera*         			m_camera;		// Camera.
	Teapot*						m_teapot;		// Teapot
	std::vector<Texture*> m_textures;
	std::vector<Teapot*> m_teapots;
	std::vector<Mesh*> m_meshes;
};


// Global pointer to the application
Application* g_app = 0;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	g_app->mouse_callback(window, xposIn, yposIn);
}
void processInput(GLFWwindow* window, float deltaTime)
{
	g_app->processInput(window, deltaTime);
}
void updateallInputs(float deltaTime)
{
	g_app->update(deltaTime);

}


int main(void) {
	// Set c++-lambda as error call back function for glfw.
	glfwSetErrorCallback([](int error, const char* description) {
		fprintf(stderr, "Error %d: %s\n", error, description);
	});

	// Try to initialize glfw
	if (!glfwInit()) {
		return -1;
	}

	// Create window and check that creation was succesful.
	GLFWwindow* window = glfwCreateWindow(720, 480, "OpenGL window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	

	// Set current context
	glfwMakeContextCurrent(window);
	// Load GL functions using glad
	gladLoadGL(glfwGetProcAddress);

	// Create application
	g_app = new Application();
	glfwSetCursorPosCallback(window, mouse_callback);
	// Specify the key callback as c++-lambda to glfw
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Close window if escape is pressed by the user.
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	});
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Get time using glfwGetTime-function, for delta time calculation.
	float prevTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		
		// Render the game frame and swap OpenGL back buffer to be as front buffer.
		g_app->render(window);
		glfwSwapBuffers(window);

		// Poll other window events.
		glfwPollEvents();

		// Compute application frame time (delta time) and update application
		float curTime = (float)glfwGetTime();
		float deltaTime = curTime - prevTime;
		prevTime = curTime;
		g_app->processInput(window, deltaTime);
		g_app->update(deltaTime);
		
	}

	// Delete application
	delete g_app;
	g_app = 0;

	// Destroy window
	glfwDestroyWindow(window);

	// Terminate glfw
	glfwTerminate();

	return 0;
}





