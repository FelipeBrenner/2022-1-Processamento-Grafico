#include <math.h>

#include "Enemie.h"

Enemie::Enemie(Sprite* i_sprite, float i_x, float i_y, float i_speedX, float i_speedY) {
  this->sprite = i_sprite;
  this->x = i_x;
  this->y = i_y;
	this->speedX = i_speedX;
	this->speedY = i_speedY;
}

void Enemie::setSprite(Sprite* i_sprite) {
  this->sprite = i_sprite;
}

void Enemie::setSpeedX(float i_speed) {
	this->speedX = i_speed;
}

void Enemie::setSpeedY(float i_speed) {
	this->speedY = i_speed;
}

void Enemie::moveX() {
	this->x += speedX;
  this->sprite->setTranslation(glm::vec3(this->x, this->y, 0.0));
}

void Enemie::moveY() {
	this->y += speedY;
  this->sprite->setTranslation(glm::vec3(this->x, this->y, 0.0));
}

int Enemie::loadTexture(string path)
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