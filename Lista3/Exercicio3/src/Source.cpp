#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Biblioteca manual pra implementação dos shaders
#include "../../../commonfiles/Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;
// Número de vértices que terá em volta do ponto central + 1 + o ponto central
const int nPoints = 100 + 1 + 1;
const float pi = 3.14159;
const float radius = 30;

int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 3 - Felipe Brenner", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros das funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("../../../commonfiles/shaders/orthoModel.vs","../../../commonfiles/shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	
	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não estão nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shader.ID);

	// Criando a matriz de projeção usando a GLM
	glm::mat4 projection = glm::mat4(1); // matriz identidade
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(projection));

	float x = 400;
	float y = 300;
	float xMov = 0.02;
	float yMov = 0.02;

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(5);
		glPointSize(5);

		glBindVertexArray(VAO);

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glm::mat4 model = glm::mat4(1); // Matriz identidade

		if(x < radius || x > width - radius) {	
			xMov = -xMov;
		}

		if(y < radius || y > height - radius) {	
			yMov = -yMov;
		}

		x += xMov;
		y += yMov;

		model = glm::translate(model, glm::vec3(x, y, 0));
		shader.setMat4("model", glm::value_ptr(model));

		glUniform4f(colorLoc, 0.4f, 0.4f, 0.4f, 1.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints);

		glViewport(0, 0, width, height);

		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - é chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat* vertices;

	vertices = new GLfloat[nPoints * 3];

	float angle = 0.0;
	float deltaAngle = 2 * pi / (float)(nPoints - 2);
	
	// Adicionar o centro
	vertices[0] = 0.0; // x
	vertices[1] = 0.0; // y
	vertices[2] = 0.0; // z

	for (int i = 3; i < nPoints * 3; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i+1] = radius * sin(angle);
		vertices[i+2] = 0.0;

		angle += deltaAngle;
	}

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, nPoints * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}