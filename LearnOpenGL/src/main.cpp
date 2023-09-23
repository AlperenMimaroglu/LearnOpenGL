#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb_image.h"
#include "TextureItem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ProcessInput(GLFWwindow* window);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 1000.0f

// Camera
Camera camera(glm::vec3(0.0f, 0.5f, 5.0f));
bool firstMouse = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastX = SCREEN_WIDTH * 0.5f;
float lastY = SCREEN_HEIGHT * 0.5f;

// ------------Light Source------------
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // This sets the OpenGL context size when window is resized.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    Shader lightingShader("resources/shader/vertex.glsl", "resources/shader/fragment.glsl");

    lightingShader.Use();

    Model backpackModel("resources/Backpack/backpack.obj");

    // "Render loop" 
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        ProcessInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        lightingShader.SetMat4("model", model);
        lightingShader.SetMat4("projection", projection);
        lightingShader.SetMat4("view", view);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.Use();
        lightingShader.SetVec3("viewPos", camera.Position);
        lightingShader.SetFloat("material.shininess", 32.0f);

        // directional light
        lightingShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // spotLight
        lightingShader.SetVec3("spotLight.position", camera.Position);
        lightingShader.SetVec3("spotLight.direction", camera.Front);
        lightingShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("spotLight.constant", 1.0f);
        lightingShader.SetFloat("spotLight.linear", 0.09f);
        lightingShader.SetFloat("spotLight.quadratic", 0.032f);
        lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        backpackModel.Draw(lightingShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Ideally this function should be moved to another source file
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);
    float ypos = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}
