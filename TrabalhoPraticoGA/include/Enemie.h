#include "Sprite.h" 
#include <string>

using namespace std;

class Enemie {
  public:
    Enemie(Sprite* i_sprite, float i_x, float i_y, float i_speedX, float i_speedY);

    Sprite* sprite;
    float x = 0;
    float y = 0;
    float speedX = 0;
    float speedY = 0;

    void setSprite(Sprite* i_sprite);
    void setSpeedX(float i_speed);
    void setSpeedY(float i_speed);
    void moveX();
    void moveY();

    int loadTexture(string path); 
};