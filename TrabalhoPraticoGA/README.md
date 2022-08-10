## ⚙ Execução

Instalar GLFW, GLM e SDL:

```bash
sudo apt-get install libglfw3 libglfw3-dev libglm-dev libsdl2-dev
```

Instalar cmake e GLAD:

```bash
sudo apt-get install cmake
git clone https://github.com/Dav1dde/glad.git
cd glad
cmake ./
make
sudo cp -a include /usr/local/
```

Compilar e rodar:

```bash
make
./app
```

[![Watch the video](https://img.youtube.com/vi/Ni4ZEB0Hl6c/maxresdefault.jpg)](https://youtu.be/Ni4ZEB0Hl6c)
