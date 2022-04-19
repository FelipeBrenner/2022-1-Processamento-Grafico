#include "Sprite.h" 
#include <string>

using namespace std;

class Character {
  public:
    Character(Sprite* i_sprite, float i_x, float i_y, float i_speed);

    Sprite* sprite;
    float x = 0;
    float y = 0;
    float speed = 0;

    void setSprite(Sprite* i_sprite);
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    int loadTexture(string path); 
};