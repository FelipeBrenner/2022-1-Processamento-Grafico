#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite {
  public:
    Sprite();
    ~Sprite() {};

    void setup();

    void setTexture(int texID);
    
    void setShader(Shader* shader) { this->shader = shader; shader->Use(); }
	  void setTranslation( glm::vec3 translation) { this->translation = translation; }
    void setScale(glm::vec3 scale) { this->scale = scale; }
    void setRotation(float rotation) { this->rotation = rotation; }

	  void handleTranslation(glm::vec3 displacements, bool reset = true);
	  void handleScale(glm::vec3 scaleFactors, bool reset = true);
    void handleRotation(float angle, glm::vec3 axis, bool reset = true);

    void draw();
	  void update();

  protected:
    int texID;
    glm::mat4 model;
    glm::vec3 scale, translation;
    float rotation;
    Shader* shader;
    GLuint VAO;
};