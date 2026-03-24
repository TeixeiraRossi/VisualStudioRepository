#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Shader.h"
#include "Camera2D.h"
#include <filesystem>
#include <iostream>



Camera2D camera;

void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera.processScroll((float)yoffset);
}

int main()
{
    std::cout << std::filesystem::exists("quad.vert") << std::endl;
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Plotador", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glfwSetScrollCallback(window, scroll_callback);

    Renderer renderer;
    renderer.init();

    Shader shader("quad.vert", "domain.frag");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("zoom", camera.zoom);
        shader.setVec2("offset", camera.offsetX, camera.offsetY);

        renderer.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}