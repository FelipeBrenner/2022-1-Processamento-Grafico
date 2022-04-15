#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../../commonfiles/stb_image.h"
#include "../../../commonfiles/Shader.h"

// Protótipos de funções
int setupCircle();
int setupSprite();
int loadTexture(string path);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

const int nPoints = 100 + 1 + 1;
const float pi = 3.14159;
const float radius = 30;

static bool leftPressed, rightPressed, topPressed, downPressed;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho Prático GA - Felipe Brenner", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	Shader shader("../../../commonfiles/shaders/sprite.vs","../../../commonfiles/shaders/sprite.fs");

	GLuint texID = loadTexture("./images/pa.png");

	GLuint VAO = setupSprite();
	
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shader.ID);

	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);

	glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(projection));

	float x = radius;
	float y = radius;
	float xMov = 0.02;
	float yMov = 0.02;

	float xMain = 400;
	float yMain = 300;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(5);
		glPointSize(5);

		glBindVertexArray(VAO);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// // Circulo se movimentando

		// if(x < radius || x > width - radius) {	
		// 	xMov = -xMov;
		// }
		// if(y < radius || y > height - radius) {	
		// 	yMov = -yMov;
		// }
		// x += xMov;
		// y += yMov;

		// glm::mat4 model = glm::mat4(1);
		// model = glm::translate(model, glm::vec3(x, y, 0));
		// shader.setMat4("model", glm::value_ptr(model));
		// glUniform4f(colorLoc, 0.4f, 0.4f, 0.4f, 1.0f);
		// glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints);

		// // Circulo principal

		// if(leftPressed)
		// 	if(xMain>radius)
		// 		xMain -= 0.08f;
		
		// if(rightPressed)
		// 	if(xMain<width-radius)
		// 		xMain += 0.08f;

		// if(topPressed)
		// 	if(yMain<height-radius)
		// 		yMain += 0.08f;

		// if(downPressed)
		// 	if(yMain>radius)
		// 		yMain -= 0.08f;

		// glm::mat4 modelMain = glm::mat4(1);
		// modelMain = glm::translate(modelMain, glm::vec3(xMain, yMain, 0));
		// shader.setMat4("model", glm::value_ptr(modelMain));
		// glUniform4f(colorLoc, 0.4f, 0.4f, 0.4f, 1.0f);
		// glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints);

		//Criando a matriz de modelo usando a GLM
		glm::mat4 model = glm::mat4(1); //matriz identidade
		model = glm::translate(model, glm::vec3(400.0, 300.0, 0));
		model = glm::rotate(model, (float)glfwGetTime() /*glm::radians(90.0f)*/, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(200.0, 200.0, 1.0));
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));

		//Ativando o primeiro buffer de textura (0) e conectando ao identificador gerado
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rightPressed = true;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		leftPressed = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		topPressed = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		downPressed = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		rightPressed = false;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		leftPressed = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		topPressed = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		downPressed = false;
}

int setupCircle()
{
	GLfloat* vertices;
	vertices = new GLfloat[nPoints * 3];

	float angle = 0.0;
	float deltaAngle = 2 * pi / (float)(nPoints - 2);
	
	vertices[0] = 0.0;
	vertices[1] = 0.0;
	vertices[2] = 0.0;

	for (int i = 3; i < nPoints * 3; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i+1] = radius * sin(angle);
		vertices[i+2] = 0.0;

		angle += deltaAngle;
	}

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nPoints * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 

	return VAO;
}

int setupSprite()
{
	GLuint VAO;
	GLuint VBO, EBO;

	float vertices[] = {
		// posicoes          // cores          // coordenadas de textura
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // superior direito
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // inferior direito
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // inferior esquerdo
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // superior esquerdo
	};
	unsigned int indices[] = {
	0, 1, 3, // primeiro triangulo
	1, 2, 3  // segundo triangulo
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Primeiro atributo - Layout 0 - posi��o - 3 valores - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Segundo atributo - Layout 1 - cor - 3 valores - r, g, b
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Terceiro atributo - Layout 2 - coordenadas de textura - 2 valores - s, t (ou u, v)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //desvincula

	return VAO;
}

int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os par�metros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}