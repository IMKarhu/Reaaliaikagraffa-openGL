#pragma once
#include "gameobject.h"     // Include base class
#include <glm/gtx/transform.hpp>	// glm transform functions.

//float left, float right, float bottom, float top
class Camera : public GameObject {
public:
    Camera()
        : GameObject(__FUNCTION__) {
        //m_projection = glm::ortho(left, right, bottom, top);
        float Fov = glm::half_pi<float>();
        m_projection = glm::perspectiveFov(Fov, 640.0f, 480.0f, 0.1f, 100.0f);
    }

    ~Camera() {

    }

    const glm::mat4& getProjectionMatrix() const {
        return m_projection;
    }

private:
    glm::mat4 m_projection;
   // float Fov = glm::half_pi<float>();
};
