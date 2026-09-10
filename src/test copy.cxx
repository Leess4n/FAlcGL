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

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
    2, 3, 0,
};



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
        std::cout << "x: " << params[0]->GetImageElem(i) << " y: " << params[1]->GetImageElem(i) << "z: " << params[2]->GetImageElem(i) << std::endl;
        std::cout << "dx: " << (dt*(params[0]->GetImageElem(i)*params[1]->GetImageElem(i) - beta * params[2]->GetImageElem(i))) << std::endl;
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
    unsigned int I = 2, J = 100;
    std::cout << "[INFO] Init renderer." << std::endl;
    RecursiveFunction3DLineRenderer<float, XLorentz, YLorentz, ZLorentz> renderer = RecursiveFunction3DLineRenderer<float, XLorentz, YLorentz, ZLorentz>(I, J, 1.0f, 0.0f, 100.0f, dt, sigma, ro, beta);
    std::cout << "[INFO] Finished initializing." << std::endl;

    // Create and bind the vao
    VAO VAO = class VAO();
    VAO.Bind();

    // create the other array buffers
    VBO VBO = class VBO(vertices, sizeof(vertices), GL_STATIC_DRAW);

    // Configuring attribute reads
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));

    // create texture
    unsigned int texture;
    glGenTextures(1, &texture);

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // importing image for textures
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(TEXTURE("container.jpg"), &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // free the image data
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);

    // bind texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // importing image for textures
    width, height, nrChannels;
    data = stbi_load(TEXTURE("awesomeface.png"), &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // free the image data
    stbi_image_free(data);

    // Shader program
    Shader defaultShader = Shader(SHADER("default.vert"), SHADER("default.frag"));
    defaultShader.use();

    float faceness = 0.0f, faceSpeed = 0.001f;

    defaultShader.setInt("ourTexture", 0);
    defaultShader.setInt("faceTexture", 1);
    defaultShader.setFloat("faceness", faceness);

    Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), wWidth, wHeight);

    glEnable(GL_DEPTH_TEST);

    std::cout << "[INFO] Starting event loop." << std::endl;
	// while the window should not close (while you do not press the close button) process pending events
	while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (faceness<1.0f)
            faceness += faceSpeed;
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (faceness>0.0f)
            faceness -= faceSpeed;
        }
        defaultShader.setFloat("faceness", faceness);
        camera.processInput(window);
        // Specify color of the background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.Render(camera);
        renderer.SafeUpdateImagesBy(100);
        renderer.SafeUpdateVBOs();

        defaultShader.use();
        VAO.Bind();
        VBO.Bind();

        // send uniform matrices for 3d viewing
        camera.updateViewProj(45.0f, 0.5f, 100.0f);
        camera.viewToShader("view", defaultShader);
        camera.projToShader("projection", defaultShader);

        VAO.Bind();
        for(unsigned int i = 0; i<10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20 * i;
            model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));
            defaultShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Process pending events
		glfwPollEvents();
	}

    VAO.Delete();
    VBO.Delete();
	// destroy window (closes it) and terminates glfw
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}