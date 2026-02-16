#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Zorro Geometrico Color", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// --- ARREGLO DE VÉRTICES ACTUALIZADO ---
	float vertices[] = {

	 //Oreja Izquierda
	 -0.4f,  0.8f, 0.0f,   0.89f, 0.35f, 0.13f,
	 -0.2f,  0.4f, 0.0f,   0.89f, 0.35f, 0.13f,
	 -0.1f,  0.6f, 0.0f,   0.89f, 0.35f, 0.13f,
	 
	 //Oreja Derecha 
	 0.4f,  0.8f, 0.0f,   0.89f, 0.35f, 0.13f,
	 0.2f,  0.4f, 0.0f,   0.89f, 0.35f, 0.13f,
	 0.1f,  0.6f, 0.0f,   0.89f, 0.35f, 0.13f,

	 //Frente / Mitad superior cara T1
	 -0.2f,  0.6f, 0.0f,   0.95f, 0.50f, 0.15f,
	 0.2f,  0.6f, 0.0f,   0.95f, 0.50f, 0.15f,
	 -0.2f,  0.4f, 0.0f,   0.95f, 0.50f, 0.15f,

	 // Frente / Mitad superior cara T2 
	 0.2f,  0.6f, 0.0f,   0.95f, 0.50f, 0.15f,
	 0.2f,  0.4f, 0.0f,   0.95f, 0.50f, 0.15f,
	 -0.2f,  0.4f, 0.0f,   0.95f, 0.50f, 0.15f,

	 // Mejillas 
	 -0.4f,  0.4f, 0.0f,   0.95f, 0.95f, 0.95f,
	 0.4f,  0.4f, 0.0f,   0.95f, 0.95f, 0.95f,
	 0.0f,  0.0f, 0.0f,   0.95f, 0.95f, 0.95f,
	 
	 // Pecho central
	 -0.2f,  0.0f, 0.0f,   0.85f, 0.85f, 0.85f,
	 0.2f,  0.0f, 0.0f,   0.85f, 0.85f, 0.85f,
	 0.0f, -0.4f, 0.0f,   0.75f, 0.75f, 0.75f,

	 // Lomo superior izquierdo
	 -0.2f,  0.0f, 0.0f,   0.95f, 0.65f, 0.20f,
	 -0.4f, -0.2f, 0.0f,   0.95f, 0.65f, 0.20f,
	 0.0f, -0.4f, 0.0f,   0.95f, 0.65f, 0.20f,

	 // Cuerpo inferior izquierdo
	 -0.4f, -0.2f, 0.0f,   0.85f, 0.25f, 0.10f,
	 0.0f, -0.4f, 0.0f,   0.85f, 0.25f, 0.10f,
	 0.0f, -0.8f, 0.0f,   0.85f, 0.25f, 0.10f,

	 // Cola
	 0.0f, -0.4f, 0.0f,   0.80f, 0.80f, 0.80f,
	 0.0f, -0.8f, 0.0f,   0.95f, 0.95f, 0.95f,
	 0.2f, -0.6f, 0.0f,   0.80f, 0.80f, 0.80f,

	 // Nariz
	 -0.05f, 0.05f, 0.0f,  0.10f, 0.10f, 0.10f,
	 0.05f, 0.05f, 0.0f,  0.10f, 0.10f, 0.10f,
	 0.00f, 0.00f, 0.0f,  0.10f, 0.10f, 0.10f
	};

	// --- ARREGLO DE ÍNDICES ACTUALIZADO ---
	unsigned int indices[] = {
	 0, 1, 2,      // Oreja Izquierda
	 3, 4, 5,      // Oreja Derecha
	 6, 7, 8,      // Frente 1
	 9, 10, 11,    // Frente 2
	 12, 13, 14,   // Mejillas
	 15, 16, 17,   // Pecho
	 18, 19, 20,   // Lomo superior
	 21, 22, 23,   // Cuerpo inferior
	 24, 25, 26,   // Cola
	 27, 28, 29    // Nariz 
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Atributo Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Fondo
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}