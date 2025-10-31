#include<SDL3/SDL.h>
#include<GL/glew.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>


//vertex shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;
uniform mat4 uTransform;

void main() {
    gl_Position = uTransform * vec4(aPos, 0.0, 1.0);
    vTexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, vTexCoord);
}
)";

// Função utilitária para compilar shaders
GLuint compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info[512];
		glGetShaderInfoLog(shader, 512, nullptr, info);
		std::cerr << "Erro ao compilar shader: " << info << std::endl;
	}
	return shader;
}

GLuint loadTexture(const char* path) {
	int w, h, channels;
	unsigned char* data = stbi_load(path, &w, &h, &channels, 4);
	if (!data) {
		std::cerr << "Erro ao carregar textura: " << path << std::endl;
		return 0;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//filtros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
	return tex;
}


const int MAP_W = 10;
const int MAP_H = 10;

int map[MAP_W][MAP_H] = {
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,1,1,1,1,0,1},
	{1,0,1,0,0,0,0,1,0,1},
	{1,0,1,0,1,1,0,1,0,1},
	{1,0,1,0,1,1,0,1,0,1},
	{1,0,1,0,0,0,0,1,0,1},
	{1,0,1,1,1,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
		std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
		return -1;
	}
	//atributos do contexto opengl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//cria janela com o suporte opengl
	SDL_Window* window = SDL_CreateWindow("Pac-Man Clone", 860, 600, SDL_WINDOW_OPENGL);

	if (!window) {
		std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	//cria o contexto opengl
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cerr << "Erro ao criar contexto Gl: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	//inicializa glew
	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		std::cerr << "Erro GLEW: " << glewGetErrorString(glewErr) << std::endl;
		return -1;
	}
	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;


	// Compila e linka shaders
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	GLuint shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);
	glDeleteShader(vs);
	glDeleteShader(fs);

	float vertices[] = {
		// pos        // tex
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.0f, 1.0f
	};
	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//carrega texturas

	GLuint texWall = loadTexture("assets/wall.png");
	GLuint texFloor = loadTexture("assets/floor.png");
 
	glUseProgram(shader);
	GLint uTransform = glGetUniformLocation(shader, "uTransform");

	bool running = true;
	SDL_Event  e;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT)
				running = false;
			if (e.type == SDL_EVENT_KEY_DOWN)
				running = false;
		}

		glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(VAO);

		for (int y = 0; y < MAP_H; y++) {
			for (int x = 0; x < MAP_W; x++) {
				if (map[y][x] == 1) {
					float px = (x / (float)MAP_W) * 2.0f - 1.0f + 0.1f;
					float py = (y / (float)MAP_H) * -2.0f + 1.0f - 0.1f;
					float scale = 0.18f;

					float transform[16] = {
						scale, 0, 0, 0,
						0, scale, 0, 0,
						0, 0, 1, 0,
						px, py, 0, 1
					};
					glUniformMatrix4fv(uTransform, 1, GL_FALSE, transform);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
		}

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}