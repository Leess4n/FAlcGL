#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <stdexcept>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "core/FunctionRule.hxx"
#include "core/FunctionRelation.hxx"
#include "core/FunctionManager.hxx"
#include "core/RecursiveFunctionRule.hxx"
#include "core/RecursiveFunctionManager.hxx"

#include "display/VAO.h"
#include "display/EBO.h"
#include "display/VBO.h"
#include "display/Shader.h"
#include "display/Camera.h"

#include "rendering/RecursiveFunction3DLineRenderer.hxx"

#include "RootDir.h"

#define SHADER(filename) (std::string(SHADER_DIR) + std::string(filename)).c_str()
#define TEXTURE(filename) (std::string(ROOT_DIR) + std::string("res/textures/") + std::string(filename)).c_str()


// First create instances of the Lorentz Attractor's differential equations. The parameters of the equation are stored in the instances
// ====================================================================================================================================

// Create instance of the function returning the dx/dt: dx/dt= sigma*(y-x)
//                                                      ==================
template<real T>
class XLorentz : public RecursiveFunctionRule<T>
{
public:
    XLorentz(const T x0, const T dt, const T sigma, const T ro, const T beta) : RecursiveFunctionRule<T>(x0), dt(dt), sigma(sigma), ro(ro), beta(beta) {};
    inline const T Next(FunctionRelation<T> **params, const unsigned int i) const override
    {
        return params[0]->GetImageElem(i) + (dt*sigma*(params[1]->GetImageElem(i) - params[0]->GetImageElem(i)));
    }
private:
    const T dt, sigma, ro, beta;
};

// Create instance of the function returning the dy/dt: dy/dt= x*(ro-z) - y
//                                                      ===================
template<real T>
class YLorentz : public RecursiveFunctionRule<T>
{
public:
    YLorentz(const T x0, const T dt, const T sigma, const T ro, const T beta) : RecursiveFunctionRule<T>(x0), dt(dt), sigma(sigma), ro(ro), beta(beta) {};
    inline const T Next(FunctionRelation<T> **params, const unsigned int i) const override
    {
        return params[1]->GetImageElem(i) + (dt*(params[0]->GetImageElem(i) * (ro-(params[2]->GetImageElem(i))) - (params[1]->GetImageElem(i))));
    }
private:
    const T dt, sigma, ro, beta;
};

// Create instance of the function returning the dz/dt: dz/dt= xy - beta*z
//                                                      ==================
template<real T>
class ZLorentz : public RecursiveFunctionRule<T>
{
public:
    ZLorentz(const T x0, const T dt, const T sigma, const T ro, const T beta) : RecursiveFunctionRule<T>(x0), dt(dt), sigma(sigma), ro(ro), beta(beta) {};
    inline const T Next(FunctionRelation<T> **params, const unsigned int i) const override
    {
        // std::cout << "x: " << params[0]->GetImageElem(i) << " y: " << params[1]->GetImageElem(i) << "z: " << params[2]->GetImageElem(i) << std::endl;
        // std::cout << "dx: " << (dt*(params[0]->GetImageElem(i)*params[1]->GetImageElem(i) - beta * params[2]->GetImageElem(i))) << std::endl;
        return params[2]->GetImageElem(i) + (dt*(params[0]->GetImageElem(i)*params[1]->GetImageElem(i) - beta * params[2]->GetImageElem(i)));
    }
private:
    const T dt, sigma, ro, beta;
};


int main(int argc, char *argv[])
{
	// Initialize glfw
	glfwInit();

	// Specify openGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tell glfw we are only using the core module (no legacy functions from previous versions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const unsigned int wHeight = 1000, wWidth = 1500;

	// create window object and check if it has been created correctly
	GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "FAlc View", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Give the context to the window
	glfwMakeContextCurrent(window);

	// load glad (used to create the view port for openGL)
	gladLoadGL();

	// specify where the viewport should be
    glViewport(0, 0, wWidth, wHeight);

    // Create the Lorentz renderer
    // ===========================
    float dt = 0.001f, sigma = 10.0f, ro = 28.0f, beta = 8.0f/3.0f;
    unsigned int I = 5, J = 5000;
    if (argv[1] && argv[2]){
        I = std::stof(argv[1]);
        J = std::stof(argv[2]);
    }
    
    RecursiveFunction3DLineRenderer<float, XLorentz, YLorentz, ZLorentz> renderer = RecursiveFunction3DLineRenderer<float, XLorentz, YLorentz, ZLorentz>(I, J, 1.0f, 0.0f, 100.0f, dt, sigma, ro, beta);

    glm::vec3 center = glm::vec3(-1,25,2), orientation;
    Camera camera = Camera(glm::vec3(-40.0f, 25.0f, 39.0f), wWidth, wHeight);

    glEnable(GL_DEPTH_TEST);

	// while the window should not close (while you do not press the close button) process pending events
	while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();
        camera.processInput(window);
        // Specify color of the background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        orientation = center - camera.getCurrentPosition();
        camera.translate(0.0005f * glm::cross(glm::normalize(glm::vec3(0.0f,1.0f,1.0f)), orientation));
        camera.updateOrientation(orientation);

        // send uniform matrices for 3d viewing
        camera.updateViewProj(45.0f, 0.5f, 100.0f);
        camera.viewToShader("view", renderer.GetShader());
        camera.projToShader("projection", renderer.GetShader());
        
        renderer.Render(camera);
        renderer.SafeUpdateImagesBy(1);
        renderer.SafeUpdateVBOs();

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Process pending events
		glfwPollEvents();
	}

	// destroy window (closes it) and terminates glfw
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}