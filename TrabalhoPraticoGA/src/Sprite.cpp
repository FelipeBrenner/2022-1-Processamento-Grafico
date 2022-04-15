#include "../include/Sprite.h"

Sprite::Sprite()
{
	setup();
}

void Sprite::setup() {
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  texID = -1;
  model = glm::mat4(1);
  scale = glm::vec3(1.0f, 1.0f, 1.0f);
  translation = glm::vec3(0.0f, 0.0f, 0.0f);
  rotation = 0.0f;
  shader = NULL;
}

void Sprite::setTexture(int texID)
{
	this->texID = texID;
}

void Sprite::handleRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) model = glm::mat4(1);
	model = glm::rotate(model, angle, axis);
}

void Sprite::handleTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) model = glm::mat4(1);
	model = glm::translate(model, displacements);
}

void Sprite::handleScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) model = glm::mat4(1);
	model = glm::scale(model, scaleFactors);
	scale = scaleFactors;
}

void Sprite::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shader->ID, "tex_buffer"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sprite::update()
{
	handleTranslation(translation);
	handleRotation(rotation, glm::vec3(0.0f, 0.0f, 1.0f), false);
	handleScale(scale, false);

	GLint modelLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}
