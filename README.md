## 💻 Repositório

Repositório criado para o desenvolvimento e entrega das atividades da cadeira de Processamento Gráfico do curso de Ciência da Computação da Unisinos.

- [Shaders & Buffer](https://github.com/FelipeBrenner/2022-1-Processamento-Grafico/tree/master/Lista1)
- [Sistemas de Coordenadas e Câmera 2D](https://github.com/FelipeBrenner/2022-1-Processamento-Grafico/tree/master/Lista2)
- [Introdução à OpenGL Moderna – Transformações em Objetos](https://github.com/FelipeBrenner/2022-1-Processamento-Grafico/tree/master/Lista3)

- [Trabalho Prático GA](https://github.com/FelipeBrenner/2022-1-Processamento-Grafico/tree/master/TrabalhoPraticoGA)

## ⚙ Execução

Para as listas em OpenGL, a execução é a seguinte abaixo (meu SO é Linux):

```bash
# na pasta src do respectivo exercício, gerar o executável
g++ Source.cpp ../../../commonfiles/glad.c -ldl -lglfw
# e o executar
./a.out
```

Para os projetos em OpenCV, tendo ele instalado corretamente na sua máquina, a execução é a seguinte abaixo (meu SO é Linux):

```bash
# gerar o executável
g++ -std=c++14 arquivo-a-executar.cpp `pkg-config --libs --cflags opencv4` -o app
# e o executar
./app
```
