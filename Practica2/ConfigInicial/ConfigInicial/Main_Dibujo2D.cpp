/*
Práctica 2
Fernando Pérez Cruz
Fecha de entrega: 24 de agosto de 2025
422022569
*/

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
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 Fernando Perez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers

	// Coordenadas para dibujar ranita
	float vertices[] = {
		//c1 verde
		-0.2f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c2 café
		-0.2f,  0.0f, 0.0f,			0.5412f, 0.3255f, 0.0314f,  
		0.2f,  0.0f, 0.0f,			0.5412f, 0.3255f, 0.0314f,
		0.2f, -0.1f, 0.0f,			0.5412f, 0.3255f, 0.0314f,
		-0.2f, -0.1f, 0.0f,			0.5412f, 0.3255f, 0.0314f,

		// c3 verde
		0.2f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.5f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.5f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c4 verde
		-0.5f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c5 verde
		-0.2f,	-0.1f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	-0.1f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c6 rosa
		0.2f,	0.1f,	0.0f,		0.98f, 0.549f, 0.98f,
		0.4f,	0.1f,	0.0f,		0.98f, 0.549f, 0.98f,
		0.4f,	0.0f,	0.0f,		0.98f, 0.549f, 0.98f,
		0.2f,	0.0f,	0.0f,		0.98f, 0.549f, 0.98f,

		// c4 verde
		-0.5f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f,	-0.3f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c7 verde
		0.4f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.5f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.5f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.4f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c8 verde
		0.3f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.4f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.4f,	0.1f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.3f,	0.1f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c9 verde
		0.2f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.3f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.3f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c10 verde
		0.1f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.2f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		0.1f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c11 café
		0.2f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.3f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.3f,	0.1f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.2f,	0.1f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c12 verde
		-0.2f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.1f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.1f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c13 café
		-0.3f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.2f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.2f,	0.1f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.3f,	0.1f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c14 verde
		-0.4f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.3f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.3f,	0.1f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.4f,	0.1f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c15 rosa
		-0.4f,	0.1f,	0.0f,		0.98f, 0.549f, 0.98f,
		-0.2f,	0.1f,	0.0f,		0.98f, 0.549f, 0.98f,
		-0.2f,	0.0f,	0.0f,		0.98f, 0.549f, 0.98f,
		-0.4f,	0.0f,	0.0f,		0.98f, 0.549f, 0.98f,

		// c16 verde
		-0.3f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	0.4f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.2f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.3f,	0.3f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c17 verde
		-0.5f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.4f,	0.2f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.4f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f,	0.0f,	0.0f,		0.0f, 1.0f, 0.0f,

		// c18 cafe
		-0.5f,	-0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.5f,	-0.3,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.5f,	-0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.5f,	-0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c19 cafe
		-0.6f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.5f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.5f,	-0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.6f,	-0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c20 cafe
		-0.5f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.4f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.4f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.5f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c21 cafe
		-0.4f,	0.5f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.2f,	0.5f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.2f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.4f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c22 cafe
		-0.2f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.1f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.1f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.2f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c23 cafe
		-0.1f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.1f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.1f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		-0.1f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c24 cafe
		0.1f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.2f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.2f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.1f,	0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c25 cafe
		0.2f,	0.5f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.4f,	0.5f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.4f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.2f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c26 cafe
		0.4f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.5f,	0.4f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.5f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.4f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,

		// c27 cafe
		0.5f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.6f,	0.2f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.6f,	-0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
		0.5f,	-0.3f,	0.0f,		0.5412f, 0.3255f, 0.0314f,
	};

	unsigned int indices[] = {  // note that we start from 0!
		0,1,3,// second Triangle
		0,1,3,

	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        glPointSize(1);
        
		// Dibujando ranita
		
		for (int i = 0; i <= 108; i = i + 4) {
			glDrawArrays(GL_QUADS, i, 4);
		}
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}