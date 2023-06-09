#pragma once
#include <kgfw/Object.h>	// Include kgfw::Object to be used as a base class
#include <glm/glm.hpp>      // Include glm

class GameObject : public kgfw::Object {
public:
    GameObject(const char* const functionName);
    ~GameObject();

    //setters
    void setPosition(const glm::vec3& position);
    void setRotationy(const float angleInRadians);
    void setRotationx(const float angleInRadians);
    void setRotationz(const float angleInRadians);
    void setScaling(const glm::vec3& scale);
    void setFront(const glm::vec3& to);
    void setWorldUp(const glm::vec3& up);

    const glm::vec3& getPosition() const;
    float getRotationy() const;
    float getRotationx() const;
    float getRotationz() const;
    const glm::vec3& getScaling() const;
    glm::vec3& getFront();
    glm::vec3& getWorldUp();

    glm::mat4 getModelMatrix() const;
    glm::mat4 getViewMatrix();

	void setLookAt();
    
    
private:
    // Model position, rotation and scale
    glm::vec3 m_position;           // Store position of plane here
    glm::vec3 m_angleInRadians;        // Store angle of plane here
    glm::vec3 m_scale;              // Store scale of plane here
	glm::mat4 m_oritentation;
    glm::vec3 m_to;
    glm::vec3 m_up;
    
};
