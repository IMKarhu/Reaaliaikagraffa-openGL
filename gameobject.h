#pragma once
#include <kgfw/Object.h>	// Include kgfw::Object to be used as a base class
#include <glm/glm.hpp>      // Include glm

class GameObject : public kgfw::Object {
public:
    GameObject(const char* const functionName);
    ~GameObject();

    void setPosition(const glm::vec3& position);
    void setRotationy(float angleInRadians);
    void setRotationx(float angleInRadians);
    void setScaling(const glm::vec3& scale);
    void setFront(const glm::vec3& to);
    void setWorldUp(const glm::vec3& up);

    const glm::vec3& getPosition() const;
    float getRotationy() const;
    float getRotationx() const;
    const glm::vec3& getScaling() const;
    glm::vec3& getFront();
    glm::vec3& getWorldUp();

    glm::mat4 getModelMatrix() const;

	void setLookAt();
    
    glm::vec3 m_to = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
private:
    // Model position, rotation and scale
    glm::vec3 m_position;           // Store position of plane here
    glm::vec3 m_angleInRadians;        // Store angle of plane here
    glm::vec3 m_scale;              // Store scale of plane here
	glm::mat4 m_oritentation;
   
    
};
