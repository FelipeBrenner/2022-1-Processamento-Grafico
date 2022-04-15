#pragma once

#include "Shader.h"
#include "Sprite.h"
#include "Character.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <math.h>

using namespace std;

class Game {
  public:
    Game();
    ~Game();
    
    void start();
    void run();

    void update();
	  void render();

    int loadTexture(string path);

  private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void initializeGraphics();
    void setupCamera2D();
    void addShader(string vFilename, string fFilename);
    void createCharacter();

    GLFWwindow* window;
    Shader* shader;
    glm::mat4 projection;

    vector <Sprite*> objects;
    Character* character;
};