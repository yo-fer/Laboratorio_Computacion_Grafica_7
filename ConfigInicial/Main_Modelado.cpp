/*
Práctica 4
Fecha de entrega: 06/09/2025
Fernando Pérez Cruz
422022569
*/

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

// Color por atributo constante en location 1
static constexpr GLint COLOR_ATTR_LOC = 1;
static inline void SetColorConst(const glm::vec3& c) {
	glDisableVertexAttribArray(COLOR_ATTR_LOC);
	glVertexAttrib3f(COLOR_ATTR_LOC, c.x, c.y, c.z);
}

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f, movY = 0.0f, movZ = -12.0f;
float rot = 0.0f;


int main() {
	if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return EXIT_FAILURE; }
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Fernando Perez", nullptr, nullptr);
	if (!window) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { std::cerr << "Failed to initialise GLEW\n"; return EXIT_FAILURE; }

	int sw, sh; glfwGetFramebufferSize(window, &sw, &sh);
	glViewport(0, 0, sw, sh);
	glEnable(GL_DEPTH_TEST);

	Shader shader("Shader/core.vs", "Shader/core.frag");

	float vertices[] = {
		// front
		-0.5f,-0.5f, 0.5f,
		0.5f,-0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,

		// back
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,

		// right
		0.5f,-0.5f, 0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f,-0.5f, 0.5f,

		// left
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		// bottom
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f, 0.5f,
		0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f,-0.5f,

		// top
		-0.5f, 0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)sw / (float)sh, 0.1f, 100.0f);

	// #### Colours ####
	const glm::vec3 SKY_BLUE = { 0.55f, 0.70f, 0.80f };
	const glm::vec3 GROUND_BLUE = { 0.40f, 0.58f, 0.68f };
	const glm::vec3 TURT_GREEN = { 0.431f, 0.655f, 0.0521f };
	const glm::vec3 TURT_GREEN2 = { 0.345f, 0.545f, 0.024f };
	const glm::vec3 SHELL_OR1 = { 0.627f, 0.243f, 0.043f };
	const glm::vec3 SHELL_OR2 = { 0.85f, 0.35f, 0.10f };
	const glm::vec3 BAND_YEL = { 0.729f, 0.588f, 0.016f };
	const glm::vec3 BELLY_CREAM = { 0.831f, 0.839f, 0.596f };
	const glm::vec3 BLACK = { 0.05f, 0.05f, 0.06f };

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		// Dibujando el fondo azul
		glClearColor(SKY_BLUE.x, SKY_BLUE.y, SKY_BLUE.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 1, 0));

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glm::mat4 model;

		// Dibujando el suelo
		SetColorConst(GROUND_BLUE);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 0.2f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Dibujando la tortuga
		glm::vec3 center = { 0.0f, 0.0f, 0.0f };

		// Cabeza
		SetColorConst(TURT_GREEN);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 1.4f, 2.2f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cuello
		SetColorConst(TURT_GREEN2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 1.0f, 1.4f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojos
		SetColorConst(BLACK);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.45f, 1.45f, 2.8f));
		model = glm::scale(model, glm::vec3(0.5f, 0.40f, 0.10f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(-0.45f, 1.45f, 2.8f));
		model = glm::scale(model, glm::vec3(0.5f, 0.40f, 0.10f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(-0.7f, 1.45f, 2.6f));
		model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.7f, 1.45f, 2.6f));
		model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Banda amarilla entre el caparazón
		SetColorConst(BAND_YEL);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 0.8f, -0.28f));
		model = glm::scale(model, glm::vec3(3.2f, 0.4f, 2.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Parte baja del caparazón
		SetColorConst(BELLY_CREAM);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 0.42f, -0.28f));
		model = glm::scale(model, glm::vec3(3.2f, 0.3f, 2.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Patas
		SetColorConst(TURT_GREEN);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(1.8f, 0.1f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(-1.8f, 0.1f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(1.8f, 0.1f, -1.8f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(-1.8f, 0.1f, -1.8f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cola
		SetColorConst(TURT_GREEN);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 0.8f, -2.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.4f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 1.0f, -2.4f));
		model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Parte superior del caparazón
		SetColorConst(SHELL_OR1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 1.35f, -0.28f));
		model = glm::scale(model, glm::vec3(3.0f, 0.7f, 2.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//model = glm::translate(model, center + glm::vec3(0.0f, 0.42f, -0.28f));
		//model = glm::scale(model, glm::vec3(3.2f, 0.3f, 2.8f));

		SetColorConst(SHELL_OR1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 2.0f, -0.3f));
		model = glm::scale(model, glm::vec3(2.5f, 0.75f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		SetColorConst(SHELL_OR1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 2.55f, -0.3f));
		model = glm::scale(model, glm::vec3(1.6f, 0.4f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/////////////////////////////

		SetColorConst(SHELL_OR2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(1.6f, 1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.8f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(-1.6f, 1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.8f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 1.9f, 1.4f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.6f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, center + glm::vec3(0.0f, 1.9f, -1.4f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.6f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) movY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) movY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot -= 0.1f;
}