#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>

#include"ExpressionParser.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera2D.h"
#include <filesystem>


Camera2D camera;

void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera.processScroll((float)yoffset);
}

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Plotador", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 800, 600);

    glfwSetScrollCallback(window, scroll_callback);

    static char input[256] = "z*z";
    static bool applyClicked = false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); 
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Renderer renderer;
    renderer.init();

    Shader shader("quad.vert", "domain.frag");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        //IMGUI UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Controle");
        ImGui::InputText("f(z) = ", input, IM_ARRAYSIZE(input));

        if (ImGui::Button("Aplicar")) {
            applyClicked = true;
        }
        if (applyClicked) {
            applyClicked = false;

            std::string expr = input;
            std::string glslExpr = generateGLSL(expr);

            std::ifstream file("domain.frag");
            std::stringstream buffer;
            buffer << file.rdbuf();

            std::string shaderCode = buffer.str();

            size_t pos = shaderCode.find("RETURN_EXPRESSION");

            if (pos != std::string::npos)
            {
                shaderCode.replace(pos, strlen("RETURN_EXPRESSION"), glslExpr);
            }
            else
            {
                std::cout << "ERRO: placeholder nao encontrado\n";
            }
            std::cout << shaderCode << std::endl;

            shader.reloadFromSource("quad.vert", shaderCode);

        }

        ImGui::Separator();
        ImGui::Text("Camera");

        ImGui::SliderFloat("Zoom", &camera.zoom, 0.1f, 5.0f);
        ImGui::SliderFloat2("Offset", &camera.offsetX, -5.0f, 5.0f);

        if (ImGui::Button("Reset Camera"))
        {
            camera.zoom = 1.0f;
            camera.offsetX = 0.0f;
            camera.offsetY = 0.0f;
        }

        ImGui::Separator();

        ImGui::Text("Funcao");
        const char* funcs[] = { "z^2", "sin(z)", "1/z" };
        static int currentFunc = 0;
        ImGui::Combo("Funcao", &currentFunc, funcs, IM_ARRAYSIZE(funcs));

        ImGui::Separator(); 
        ImGui::Text("Visual");

        static float gridScale = 2.0f;
        ImGui::SliderFloat("Grid Scale", &gridScale, 0.5f, 10.0f);

        ImGui::End();

        //Renderer
        shader.use();
        shader.setFloat("zoom", camera.zoom);
        shader.setVec2("offset", camera.offsetX, camera.offsetY);
        shader.setFloat("gridScale", gridScale);
        shader.setInt("functionId", currentFunc);

        renderer.draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}