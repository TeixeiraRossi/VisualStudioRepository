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
unsigned int triangleVAO = 0, squareVAO = 0;

int matrixId;
glm::mat4 scaleMatrix;


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

void CreateTriangle() {
    unsigned int triPositionVBO = 0, triColorVBO = 0;

    float vertexs[] = {
        +1.f, +0.f,             //+0.0f, +0.5f,
        -0.5f, +sqrt(3) / 2.f,    //-0.5f, -0.5f,
        -0.5f, -sqrt(3) / 2.f,    //+0.5f, -0.5f
    };

    GLubyte colors[] = {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255
    };

    glGenVertexArrays(1, &triangleVAO);

    glGenBuffers(1, &triPositionVBO);
    glGenBuffers(1, &triColorVBO);

    glBindVertexArray(triangleVAO);
    //ajuste do positionVBO
    glBindBuffer(GL_ARRAY_BUFFER, triPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    //ajuste do colorVBO
    glBindBuffer(GL_ARRAY_BUFFER, triColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(GLubyte), 0);
}

void CreateSquad() {
    unsigned int squarePositionVBO = 0, squareColorVBO = 0;
    unsigned int squareEBO;
    float vertexs[] = {
        +0.f, +1.f, // 0
        +0.f, -1.f, // 1
        +1.f, +0.f, // 2
        -1.f, +0.f  // 3
    };

    GLubyte colors[] = {
        0, 125, 255, // 0
        0, 125, 255, // 1
        0, 0, 255,   // 2
        85, 125, 255 // 3
    };

    unsigned int indexes[] = {
        //tri 1
        0, 1, 2,
        
        //tri 2
        0, 3, 1
    };

    glGenVertexArrays(1, &squareVAO);

    glGenBuffers(1, &squareEBO);

    glGenBuffers(1, &squarePositionVBO);
    glGenBuffers(1, &squareColorVBO);

    glBindVertexArray(squareVAO);

    //fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    //fill position
    glBindBuffer(GL_ARRAY_BUFFER, squarePositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //fill color
    glBindBuffer(GL_ARRAY_BUFFER, squareColorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(GLubyte), 0);
}

void initOpenGL() {
    CreateTriangle();
    CreateSquad();

    CompileAndLinkShaders();

    matrixId = glGetUniformLocation(programId, "Matrix");

}

void draw(float dt) {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //triangle
    {
        glm::mat4 modelToWorld(1.f);

        modelToWorld = glm::translate(modelToWorld, glm::vec3(-0.5f, 0.5f, 0.f));
        modelToWorld = glm::rotate(modelToWorld, glm::radians(60.f * dt), glm::vec3(0.f, 0.f, 1.f));
        modelToWorld *= scaleMatrix;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(modelToWorld));

        glBindVertexArray(triangleVAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //
        modelToWorld = glm::translate(glm::mat4(1.f), glm::vec3(+0.5f, 0.5f, 0.f));
        modelToWorld = glm::rotate(modelToWorld, glm::radians(-60.f * dt), glm::vec3(0.f, 0.f, 1.f));
        modelToWorld *= scaleMatrix;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(modelToWorld));

        glDrawArrays(GL_TRIANGLES, 0, 3);

    }

    //squad
    {
        glm::mat4 modelToWorld(1.f);

        modelToWorld = glm::translate(modelToWorld, glm::vec3(+0.5f, -0.5f, 0.f));
        modelToWorld = glm::rotate(modelToWorld, glm::radians(-60.f * dt), glm::vec3(0.f, 0.f, 1.f));
        modelToWorld *= scaleMatrix;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(modelToWorld));

        glBindVertexArray(squareVAO);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        modelToWorld = glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, 0.f));
        modelToWorld = glm::rotate(modelToWorld, glm::radians(+60.f * dt), glm::vec3(0.f, 0.f, 1.f));
        modelToWorld *= scaleMatrix;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(modelToWorld));

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    /*/glm::vec3 myColor(1.f, 0.f, 1.f);

    int colorId = glGetUniformLocation(programId, "triangleColor");

    glUniform3fv(colorId, 1, &myColor.x); */

    //int colorId = glGetUniformLocation(programId, "u_deltaTime");
    int matrixId = glGetUniformLocation(programId, "Matrix");

    float startTime = glfwGetTime();

    scaleMatrix = glm::scale(glm::mat4(1.f), glm::vec3(0.35f, 0.35f, 1.f));

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