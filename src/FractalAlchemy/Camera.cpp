#include "Camera.h"

#include<glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 pos, float width, float height)
{
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->orientation = glm::vec3(0.0f, 0.0f, -1.0f);  // Forward-facing direction
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);             // Up vector
    this->yaw = -90.0f;                                // Yaw for standard forward direction
    this->pitch = 0.0f;                                // Initial pitch
    this->speed = 100.0f;                                // Default speed
    this->sensitivity = 100.0f;                          // Mouse sensitivity
    this->onClick = false;                             // Initial mouse state
}

const void Camera::processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        translate(speed * orientation);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        translate(-speed * orientation);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        translate(speed * glm::cross(orientation, up));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        translate(-speed * glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		translate(speed * up);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		translate(-speed * up);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.001f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!onClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		onClick = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotY = sensitivity * (float)((mouseY / height) - 0.5f);
		float rotX = sensitivity * (float)((mouseX / width) - 0.5f);

        yaw   += rotX;
        pitch += rotY;

        // make sure that when pitch is out of bounds, screen doesn't get flipped

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 newOrient;
        newOrient.x = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newOrient.y = sin(glm::radians(pitch));
        newOrient.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        orientation = glm::normalize(newOrient);
        updateUp();

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		onClick = false;
	}
}

void Camera::updatePos(glm::vec3 newPos)
{
    pos = newPos;
}

void Camera::translate(glm::vec3 offset)
{
    pos += offset;
}

void Camera::updateOrientation(glm::vec3 newOrientation)
{
    orientation = glm::normalize(newOrientation);
    updateUp();
}

void Camera::updateUp()
{
    up = glm::normalize(glm::cross(orientation, glm::cross(orientation, glm::vec3(0.0f, 1.0f, 0.0f))));
}

void Camera::updateViewProj(float FOVdeg, float near, float far)
{
    view = glm::lookAt(pos, pos+orientation, up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), near, far);
}

const void Camera::viewToShader(const char* uniformName, Shader shader) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniformName), 1, GL_FALSE, glm::value_ptr(view));
}

const void Camera::projToShader(const char* uniformName, Shader shader) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniformName), 1, GL_FALSE, glm::value_ptr(projection));
}