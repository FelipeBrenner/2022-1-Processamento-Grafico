#include "Game.h"

static const float widthDefault = 1440.0f , heightDefault = 900.0f, widthChar = 70.0f, widthEnemie = 70.0f;
static float width = widthDefault, height = heightDefault;
static bool resized, leftPressed, rightPressed, upPressed, downPressed, started, gameOver, readyToStart = true;

AudioManager* audio = new AudioManager;

Game::Game() {}

Game::~Game() {}

void Game::start() {
  initializeGraphics();
	audio->init();
	audio->play_audio("audios/pa-love-yourself.mp3");
}

void Game::initializeGraphics()
{
	glfwInit();

	window = glfwCreateWindow(width, height, "Trabalho Prático GA - Felipe Brenner", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

  const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

  addShader("shaders/sprite.vs","shaders/sprite.fs");

	resized = true;
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

	if(!started && !leftPressed && !rightPressed && !upPressed && !downPressed)
		readyToStart = true;

	if(readyToStart && (leftPressed || rightPressed || upPressed || downPressed)) {
		started = true;
		readyToStart = false;
	}
}

void Game::resize(GLFWwindow *window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	glViewport(0, 0, width, height);
}

void Game::run() {
	clock_t tInicio;
	int idEnemie = 0;

	createScene();

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();

		if(started) {
			if(gameOver)
				break;

			if((clock() - tInicio) / 1000000 > 3 && idEnemie < 10 || idEnemie == 0) {
				idEnemie++;
				tInicio = clock();
				createEnemie(idEnemie);
			}

			updateCharacter();
			updateEnemies();
			checkConflit();
		}

		render();

    glfwSwapBuffers(window);
  }

	if(started && gameOver)
		reset();
}

void Game::createScene() {
	createBackground();
	createCharacter();
}

void Game::createBackground()
{
	Sprite *sprite;
	unsigned int texID;
	Character* background;
	
	sprite = new Sprite;
	texID = loadTexture("textures/background.png");

	sprite->setTexture(texID);
	sprite->setTranslation(glm::vec3(width/2, height/2, 0.0));
	sprite->setScale(glm::vec3(width*1.25, height*1.25, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	background = new Character(sprite, width/2, height/2, 0.02f);
	backgrounds.push_back(background);

	sprite = new Sprite;
	texID = loadTexture("textures/background2.png");

	sprite->setTexture(texID);
	sprite->setTranslation(glm::vec3(width/2, height/2, 0.0));
	sprite->setScale(glm::vec3(width/3, height/2, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	background = new Character(sprite, width/2, height/2, 0.04f);
	backgrounds.push_back(background);
}

void Game::createCharacter() {
	Sprite* sprite;
	int texID;

	sprite = new Sprite;
	texID = loadTexture("textures/pa.png");
	sprite->setTexture(texID);
	sprite->setTranslation(glm::vec3(width/2, height/2, 0));
	sprite->setScale(glm::vec3(widthChar, widthChar, 1.0));
	sprite->setShader(shader);
	objects.push_back(sprite);

	character = new Character(sprite, width/2, height/2, 0.08f);
}

void Game::createEnemie(int id) {
	Sprite* sprite;
	int texID;

	float xInitial = widthChar/2;
	float yInitial = rand() % (int)height - widthChar/2;

	sprite = new Sprite;
	texID = loadTexture("textures/" + std::to_string(id) + ".png");
	sprite->setTexture(texID);
	sprite->setTranslation(glm::vec3(xInitial, yInitial, 0));
	sprite->setScale(glm::vec3(widthEnemie, widthEnemie, 1.0));
	sprite->setShader(shader);
	objects.push_back(sprite);
	float speed = 0.03 + id * 0.005f;

	Enemie* enemie = new Enemie(sprite, xInitial, yInitial, speed, speed, id);
	enemies.push_back(enemie);

	const char* audioPath = ("audios/" + std::to_string(id) + ".mp3").c_str();
	audio->play_audio(audioPath);
}

void Game::updateCharacter() {
  if(leftPressed)
    if(character->x > widthChar/2) {
			character->moveLeft();
			for (int i = 0; i < backgrounds.size(); i++) {
				backgrounds[i]->moveRight();
			}
		}

	if(rightPressed)
    if(character->x < widthDefault - widthChar/2) {
			character->moveRight();
			for (int i = 0; i < backgrounds.size(); i++) {
				backgrounds[i]->moveLeft();
			}
		}

	if(upPressed)
    if(character->y < heightDefault - widthChar/2) {
			character->moveUp();
			for (int i = 0; i < backgrounds.size(); i++) {
				backgrounds[i]->moveDown();
			}
		}

	if(downPressed)
    if(character->y > widthChar/2) {
			character->moveDown();
			for (int i = 0; i < backgrounds.size(); i++) {
				backgrounds[i]->moveUp();
			}
		}
};

void Game::updateEnemies() {
	for (int i = 0; i < enemies.size(); i++) {
		if(enemies[i]->x > widthDefault - widthEnemie/2 || enemies[i]->x < widthEnemie/2)
			enemies[i]->setSpeedX(-enemies[i]->speedX);
			
		if(enemies[i]->y > heightDefault - widthEnemie/2 || enemies[i]->y < widthEnemie/2)
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
			const char* audioPath = ("audios/" + std::to_string(enemies[i]->id) + ".mp3").c_str();
			audio->play_audio(audioPath);

			gameOver = true;
			started = false;
			readyToStart = !leftPressed && !rightPressed && !upPressed && !downPressed;
		}
	}
}

void Game::render() {
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	if (resized) {
		setupCamera2D();
		resized = false;
	}
	
  for (int i = 0; i < objects.size(); i++) {
		objects[i]->update();
		objects[i]->draw();
	}
}

void Game::reset() {
	gameOver = false;
	objects.clear();
	enemies.clear();
	run();
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

void Game::setupCamera2D()
{
	projection = glm::ortho(0.0f, widthDefault, 0.0f, heightDefault, -1.0f, 1.0f);
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}