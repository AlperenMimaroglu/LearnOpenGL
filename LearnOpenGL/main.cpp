#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Ideally this function should be moved to another source file
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // This sets the OpenGL context size when window is resized.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // "Render loop" 
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        
        // Poll events such as keyboard inputs until close command is given
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
