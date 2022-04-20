#include <time.h>
#include "Game.h"

static const float WIDTH = 1440.0f, HEIGHT = 900.0f, widthChar = 70.0f, widthEnemie = 70.0f;
static bool leftPressed, rightPressed, upPressed, downPressed;
static bool gameOver = false;

Game::Game() {}

Game::~Game() {}

void Game::start() {
  initializeGraphics();
}

void Game::initializeGraphics()
{
	glfwInit();

	window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho Prático GA - Felipe Brenner", nullptr, nullptr);
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

  addShader("shaders/sprite.vs","shaders/sprite.fs");

  int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void Game::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rightPressed = true;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		leftPressed = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		upPressed = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		downPressed = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		rightPressed = false;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		leftPressed = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		upPressed = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		downPressed = false;
}

void Game::run() {
	createCharacter();
	clock_t tInicio = clock();
	int idEnemie = 1;

	createEnemie(idEnemie);

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();

		if(!gameOver) {

			if((clock() - tInicio) / 1000000 > 5 && idEnemie < 10) {
				idEnemie++;
				tInicio = clock();
				createEnemie(idEnemie);
			}

			updateCharacter();
			updateEnemies();
			checkConflit();
			render();
		}

    glfwSwapBuffers(window);
  }
}

void Game::createCharacter() {
	Sprite* sprite;
	int texID;

	sprite = new Sprite;
	texID = loadTexture("textures/pa.png");
	sprite->setTexture(texID);
	sprite->setTranslation(glm::vec3(WIDTH/2, HEIGHT/2, 0));
	sprite->setScale(glm::vec3(widthChar, widthChar, 1.0));
	sprite->setShader(shader);
	objects.push_back(sprite);

	character = new Character(sprite, WIDTH/2, HEIGHT/2, 0.08f);
}

void Game::createEnemie(int id) {
	Sprite* sprite;
	int texID;

	float xInitial = widthChar/2;
	float yInitial = rand() % (int)HEIGHT - widthChar/2;

	sprite = new Sprite;
	texID = loadTexture("textures/" + std::to_string(id) + ".png");
	sprite->setTexture(texID);
	sprite->setTranslation(glm::vec3(xInitial, yInitial, 0));
	sprite->setScale(glm::vec3(widthEnemie, widthEnemie, 1.0));
	sprite->setShader(shader);
	objects.push_back(sprite);
	float speed = 0.03 + id * 0.005f;

	Enemie* enemie = new Enemie(sprite, xInitial, yInitial, speed, speed);
	enemies.push_back(enemie);
}

int Game::loadTexture(string path)
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

void Game::updateCharacter() {
  if(leftPressed)
    if(character->x > widthChar/2)
			character->moveLeft();

	if(rightPressed)
    if(character->x < WIDTH - widthChar/2)
			character->moveRight();

	if(upPressed)
    if(character->y < HEIGHT - widthChar/2)
			character->moveUp();

	if(downPressed)
    if(character->y > widthChar/2)
			character->moveDown();
};

void Game::updateEnemies() {
	for (int i = 0; i < enemies.size(); i++) {
		if(enemies[i]->x > WIDTH - widthEnemie/2 || enemies[i]->x < widthEnemie/2)
			enemies[i]->setSpeedX(-enemies[i]->speedX);
		if(enemies[i]->y > HEIGHT - widthEnemie/2 || enemies[i]->y < widthEnemie/2)
			enemies[i]->setSpeedY(-enemies[i]->speedY);

		enemies[i]->moveX();
		enemies[i]->moveY();
	}
}

void Game::checkConflit() {
	for (int i = 0; i < enemies.size(); i++) {
		if(
			character->x + widthChar/2 > enemies[i]->x - widthEnemie/2 && 
			character->x - widthChar/2 < enemies[i]->x + widthEnemie/2 && 
			character->y + widthChar/2 > enemies[i]->y - widthEnemie/2 && 
			character->y - widthChar/2 < enemies[i]->y + widthEnemie/2
		) {
			gameOver = true;
		}
	}
}

void Game::render() {
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
  setupCamera2D();
	
  for (int i = 0; i < objects.size(); i++) {
		objects[i]->update();
		objects[i]->draw();
	}
}

void Game::setupCamera2D()
{
	projection = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f);
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}