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

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 1.0f;

class Camera : public GameObject {
public:
    //camera Attributes
    glm::vec3 position;
    glm::vec3 Right;
    glm::vec3 worldup;
    glm::vec3 Front;
    //Euler Angles
    float Yaw;
    float Pitch;
    //camera options
    float MouseSensitivity;
    float MovementSpeed;

    Camera(float yaw = YAW, float pitch = PITCH)
        : GameObject(__FUNCTION__), MouseSensitivity(SENSITIVITY), MovementSpeed(SPEED) {
        Yaw = yaw;
        Pitch = pitch;
        worldup = getWorldUp();
        position = getPosition();
        //Front = glm::vec3(0.0f, 0.0f, -1.0f);
        float Fov = glm::half_pi<float>();
        m_projection = glm::perspectiveFov(Fov, 720.0f, 480.0f, 0.1f, 1000.0f);
        updateCameravectors();
    }

    ~Camera() {

    }

    const glm::mat4& getProjectionMatrix() const {
        return m_projection;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

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
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
        {
            position += getFront() * velocity;
           // std::cout << "your position: " << position.x << std::endl;
        }
        if (direction == BACKWARD)
        {
            position -= getFront() * velocity;
           // std::cout << "your position: " << position.x << std::endl;
        }
        if (direction == RIGHT)
        {
            position += velocity * glm::normalize(glm::cross(getFront(), getWorldUp()));
           // std::cout << "your position: " << position.y << std::endl;
        }
        if (direction == LEFT)
        {
            position -= velocity * glm::normalize(glm::cross(getFront(), getWorldUp()));
           // std::cout << "your position: " << position.y << std::endl;
        }
        setPosition(position);
    }


private:
    glm::mat4 m_projection;
    glm::vec3 m_position;
    

    void updateCameravectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        getFront() = glm::normalize(front);

        setRotationx(glm::radians(getFront().x));
        setRotationy(glm::radians(getFront().y));
        setRotationz(glm::radians(getFront().z));
        // also re-calculate the Right and Up vector
        //Right = glm::normalize(glm::cross(getFront(), getWorldUp()));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //getWorldUp() = glm::normalize(glm::cross(Right, getFront()));
    }
};
