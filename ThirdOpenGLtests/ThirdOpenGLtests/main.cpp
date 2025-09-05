#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include<string>
#include<fstream>
#include<sstream>

#include"Error.h"

/*
JANELA FEITA PELO OPENGL
Possui valores de -1 a 1, nos eixos xyz.
*/

unsigned int programId;
unsigned int cubeVAO = 0;
unsigned int indexOffSet;

int matrixId;
glm::mat4 projection(1.f);


std::string ReadProgramSource(const std::string filePath) {
    std::fstream file(filePath);

    std::stringstream ss;
    std::string line;

    while (getline(file, line))
    {
        ss << line << "\n";
    }
    file.close();

    return ss.str();
}

void CompileAndLinkShaders() {
    //1. Criamos os Nossos Objetos:
    //  programa = Vertex Shader  + Fragment Shader
    programId = glCreateProgram();
    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    //2. Passamos as strings para com codigosGLSL
    //  para o tipo const char* = codigo fonte final
    std::string vsCode, fsCode;
    vsCode = ReadProgramSource("Main.vert");
    fsCode = ReadProgramSource("Main.frag");

    const char* vsFinalCode = vsCode.c_str();
    const char* fsFinalCode = fsCode.c_str();

    //3. Copiamos o codigo fonte final
    //  para o shader anteriormente criado
    glShaderSource(vertexShaderId, 1, &vsFinalCode, NULL);
    glShaderSource(fragmentShaderId, 1, &fsFinalCode, NULL);

    //4. compilamos os shaders
    glCompileShader(vertexShaderId);
    glCompileShader(fragmentShaderId);

    //5. Anexamos os Shaders compilados ao programa
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    //6. Link
    glLinkProgram(programId);

    //7. Delete
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    //8. Utilizar o programa
    glUseProgram(programId);
}

void createCube() {
    
    
    glm::vec3 points[] =
    {
        //front
        glm::vec3(-0.5f, +0.5f, +0.5f), //0
        glm::vec3(-0.5f, -0.5f, +0.5f), //1
        glm::vec3(+0.5f, -0.5f, +0.5f), //2
        glm::vec3(+0.5f, +0.5f, +0.5f), //3

        //back
        glm::vec3(+0.5f, +0.5f, -0.5f), //4
        glm::vec3(+0.5f, -0.5f, -0.5f), //5
        glm::vec3(-0.5f, -0.5f, -0.5f), //6
        glm::vec3(-0.5f, +0.5f, -0.5f)  //7
    };
    glm::vec3 vertices[] =
    {
        //front
        points[0], //0
        points[1], //1
        points[2], //2
        points[3], //3

        //back
        points[4], //4
        points[5], //5
        points[6], //6
        points[7], //7

        //right
        points[3], //8
        points[2], //9
        points[5], //10
        points[4], //11

        //left
        points[7], //12
        points[6], //13
        points[1], //14
        points[0], //15

        //top
        points[7], //16
        points[0], //17
        points[3], //18
        points[4], //19

        //bottom
        points[1], //20
        points[6], //21
        points[5], //22
        points[2]  //23
    };
    GLubyte colors[] =
    {
        //front red
        255, 0 ,0,
        255, 0 ,0,
        255, 0 ,0,
        255, 0 ,0,

        //back green
        0, 255, 0,
        0, 255, 0,
        0, 255, 0,
        0, 255, 0,

        //right blue
        0, 0, 255,
        0, 0, 255,
        0, 0, 255,
        0, 0, 255,

        //left yellow
        255, 255, 0,
        255, 255, 0,
        255, 255, 0,
        255, 255, 0,

        //top magenta
        255, 0, 255,
        255, 0, 255,
        255, 0, 255,
        255, 0, 255,

        //bottom cyan
        0, 255, 255,
        0, 255, 255,
        0, 255, 255,
        0, 255, 255
    };
    unsigned int indices[] =
    {
        0, 1, 2, 0, 2, 3, //front
        4, 5, 6, 4, 6, 7, //back
        8, 9, 10, 8, 10, 11, //right
        12, 13, 14, 12, 14, 15, //left
        16, 17, 18, 16, 18, 19, //top
        20, 21, 22, 20, 22, 23  //bottom
    };

    unsigned int bufferId = 0;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &bufferId);

    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(indices), 0, GL_STATIC_DRAW);

    unsigned int currentOffSet = 0;
    glBufferSubData(GL_ARRAY_BUFFER, currentOffSet, sizeof(vertices), vertices);

    currentOffSet += sizeof(vertices);
    glBufferSubData(GL_ARRAY_BUFFER, currentOffSet, sizeof(colors), colors);

    currentOffSet += sizeof(colors);
    glBufferSubData(GL_ARRAY_BUFFER, currentOffSet, sizeof(indices), indices);

    indexOffSet = currentOffSet;

    glBindVertexArray(cubeVAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(GLubyte), (void*)sizeof(vertices));
    }
    glBindVertexArray(0);

}

void initOpenGL() {
    createCube();

    CompileAndLinkShaders();

    matrixId = glGetUniformLocation(programId, "matrix");
    glEnable(GL_DEPTH_TEST);

    projection = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 10.f);
    projection = projection * glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.f));
}

void draw(float dt) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 model = glm::rotate(glm::mat4(1.f), -dt, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, 1.75f * dt, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, 0.75f * dt, glm::vec3(0.f, 0.f, 1.f));

    glm::mat4 finalMatrix = projection * model;

    glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    glBindVertexArray(cubeVAO);
    {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)indexOffSet);
    }
    glBindVertexArray(0);
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "EU VOU FICAR MALUKOOOOOOOOO", NULL, NULL);
    if (!window)
    {
        fatalError("GLFW Window could not be created");
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fatalError("Error loadind GLEW extensions");
    }

    initOpenGL();

    float startTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        /* Render here */
        draw(dt);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}