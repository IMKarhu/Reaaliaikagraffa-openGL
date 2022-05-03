#include "gameobject.h"
#include <glm/gtx/transform.hpp>	// glm transform functions.

GameObject::GameObject(const char* const functionName)
    : Object(functionName) {
    // Initialize transform
	m_oritentation = glm::mat4(1.0f);
    setPosition(glm::vec3(0.0));
    setRotationy(0.0f);
    setRotationx(0.0f);
    setScaling(glm::vec3(1.0f));
}

GameObject::~GameObject() {
}

void GameObject::setPosition(const glm::vec3& position) {
    m_position = position;
}

void GameObject::setRotationy(const float angleInRadians) {
   m_angleInRadians.y = angleInRadians;
}

void GameObject::setRotationx(const float angleInRadians)
{
   m_angleInRadians.x = angleInRadians;
}

void GameObject::setRotationz(const float angleInRadians)
{
   m_angleInRadians.z = angleInRadians;
}


void GameObject::setScaling(const glm::vec3& scale) {
    m_scale = scale;
}

void GameObject::setFront(const glm::vec3& to)
{
    m_to = to;
}

void GameObject::setWorldUp(const glm::vec3& up)
{
    m_up = up;
}

const glm::vec3& GameObject::getPosition() const {
    return m_position;
}

float GameObject::getRotationy() const {
    return m_angleInRadians.y;
}

float GameObject::getRotationx() const
{
    return m_angleInRadians.x;
}

float GameObject::getRotationz() const
{
    return m_angleInRadians.z;
}

const glm::vec3& GameObject::getScaling() const {
    return m_scale;
}

glm::vec3& GameObject::getFront()
{
    return m_to;
}

glm::vec3& GameObject::getWorldUp()
{
    return m_up;
}

glm::mat4 GameObject::getModelMatrix() const {
    return glm::translate(glm::mat4(1.0f), m_position)
			* m_oritentation
            * glm::rotate(m_angleInRadians.x, glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(m_angleInRadians.y, glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(m_angleInRadians.z, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), m_scale);
}

void GameObject::setLookAt() {
	m_oritentation = glm::inverse(glm::lookAtRH(getPosition(),getPosition() + getFront(), getWorldUp()));
}

