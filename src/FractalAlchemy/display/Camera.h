#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class Camera
{
public:
    Camera(glm::vec3 pos, float width, float height);
    
    const void processInput(GLFWwindow* window);

    void updateViewProj(float FOVdeg, float near, float far);

    const void viewToShader(const char* uniformName, Shader shader) const;
    const void projToShader(const char* uniformName, Shader shader) const;
    const void updateSpeedAndSens();

private:
    void updatePos(glm::vec3 newPos);
    void translate(glm::vec3 offset);
    void updateOrientation(glm::vec3 newOrientation);
    void updateUp();

private:
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f), up;
    glm::mat4 view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
    float width, height, yaw, pitch, speed, sensitivity;
    bool onClick = false;

public:
    glm::vec3 pos;
};