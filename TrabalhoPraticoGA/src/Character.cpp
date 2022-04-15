#include <math.h>

#include "../include/Character.h"

Character::Character(Sprite* i_sprite, float i_x, float i_y) {
  this->sprite = i_sprite;
  this->x = i_x;
  this->y = i_y;
}

void Character::setSprite(Sprite* i_sprite) {
  this->sprite = i_sprite;
}

void Character::moveRight() {
	this->x += 0.08f;
  this->sprite->setTranslation(glm::vec3(this->x, this->y, 0.0));
}

void Character::moveLeft() {
	this->x -= 0.08f;
  this->sprite->setTranslation(glm::vec3(this->x, this->y, 0.0));
}

void Character::moveUp() {
	this->y += 0.08f;
  this->sprite->setTranslation(glm::vec3(this->x, this->y, 0.0));
}

void Character::moveDown() {
	this->y -= 0.08f;
  this->sprite->setTranslation(glm::vec3(this->x, this->y, 0.0));
}

int Character::loadTexture(string path)
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