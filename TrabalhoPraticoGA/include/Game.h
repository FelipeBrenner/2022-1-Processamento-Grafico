#pragma once

#include "Shader.h"
#include "Sprite.h"
#include "Character.h"
#include "Enemie.h"
#include "AudioManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

class Game {
  public:
    Game();
    ~Game();
    
    void start();
    void run();

    void createScene();
    void createBackground();
    void updateCharacter();
    void updateEnemies();
    void checkConflit();
	  void render();
    void reset();

    int loadTexture(string path);

  private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void resize(GLFWwindow* window, int width, int height);
    void initializeGraphics();
    void setupCamera2D();
    void addShader(string vFilename, string fFilename);
    void createCharacter();
    void createEnemie(int id);

    GLFWwindow* window;
    Shader* shader;
    glm::mat4 projection;

    vector <Sprite*> objects;
    Character* character;
    Character* background;
    vector <Enemie*> enemies;
};