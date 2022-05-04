#pragma once
#include "gameobject.h"     // Include base class
#include <glm/gtx/transform.hpp>	// glm transform functions.

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

 


class Camera : public GameObject {
public:

    Camera(float width, float height)
        : GameObject(__FUNCTION__) {
        scr_width = width;
        scr_heigth = height;
        Yaw = -90.0f;
        Pitch = 0.0f;
        Sensitivity = 0.015f;
        Speed = 1.0f;
        m_position = glm::vec3(0);
        worldup = glm::vec3(0.0f, 1.0f, 0.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        float Fov = glm::half_pi<float>();
        m_projection = glm::perspectiveFov(Fov, scr_width, scr_heigth, 0.1f, 1000.0f);
        updateCameravectors();
    }

    ~Camera() {

    }
    glm::mat4& getviewmatrix()
    {
       return glm::lookAt(m_position, m_position + Front, worldup);
    }
    const glm::mat4& getProjectionMatrix() const {
        return m_projection;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if(constrainPitch)
        {
            if(Pitch>89.0f)
            Pitch = 89.0f;
            if (Pitch < -89.0f)
            Pitch = -89.0f;
        }
       // setRotationx(Pitch);
       // setRotationy(Yaw);
        updateCameravectors();
    }

    void processKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = Speed * deltaTime;
        if (direction == FORWARD)
        {
            m_position += Front * velocity;
           // std::cout << "your position: " << position.x << std::endl;
        }
        if (direction == BACKWARD)
        {
            m_position -= Front * velocity;
           // std::cout << "your position: " << position.x << std::endl;
        }
        if (direction == RIGHT)
        {
            m_position += velocity * glm::normalize(glm::cross(Front, worldup));
           // std::cout << "your position: " << position.y << std::endl;
        }
        if (direction == LEFT)
        {
            m_position -= velocity * glm::normalize(glm::cross(Front, worldup));
           // std::cout << "your position: " << position.y << std::endl;
        }
        setPosition(m_position);
    }


private:
    glm::mat4 m_projection;
    glm::vec3 m_position;
    glm::vec3 worldup;
    glm::vec3 Front;
    float Yaw;
    float Pitch;
    float Speed;
    float Sensitivity;
    float scr_width;
    float scr_heigth;
    void updateCameravectors()
    {
        //Front = getFront();
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        setRotationx(glm::radians(Front.x));
        setRotationy(glm::radians(Front.y));
        setRotationz(glm::radians(Front.z));
       // setFront(Front);
        // also re-calculate the Right and Up vector
        //Right = glm::normalize(glm::cross(getFront(), getWorldUp()));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //getWorldUp() = glm::normalize(glm::cross(Right, getFront()));
    }
};
