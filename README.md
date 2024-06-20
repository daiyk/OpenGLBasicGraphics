# OpenGL Basic Graphics

This project is a basic graphics implementation using OpenGL. It aims to provide a starting point for understanding and experimenting with graphics programming using OpenGL.

## Features

- [x] out-of-box usage
- [x] implement with modern stdandard c++ library
- [x] easy to understand and modify

## Getting Started
Please check the [Prerequisites](#prerequisites) before running this project.
To get started with this project, follow these steps:

1. Clone this repository, use the following command:
```
git clone https://github.com/daiyk/OpenGLBasicGraphics.git

```
2. Open the project in Visual Studio 2019 or later.
3. Build the project by pressing `Ctrl + B`, notice that this can take a while, since it will download all the dependencies.
4. Run the project by pressing `F5`.

## Prerequisites

Before running this project, make sure you have the following prerequisites installed:

- Visual Studio 2019 or later, and desktop development with C++ workload.
- vcpkg component checked in the visual studio installer's individual components list.
- Turn on "Use Vcpkg Manifest" in the project properties, you can find it in Project Properties -> Configuration Properties -> vcpkg.

## Dependencies
- OpenGL 4.6
- GLFW 3.3.4
- GLEW 2.1.0
- GLM 
- stb_image
All these dependencies are managed by vcpkg, and are installed automatically when you build the project, you don't need to install them manually. 


## Usage

To use this project, the project contains the following implementations and features:

1. Rendering pipeline setup and windows creation.
2. Shader implementation with vertex, fragment and geometry shaders.
3. 3D Model Loading and rendering based on Assimp.
4. Camera Implementation.
5. Texture Loading and rendering.
6. Lighting Implementation.
7. Advanced Shadows Implementation.
8. Skybox Implementation. 

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

All codes and the implementation is done by myself, and belongs to me. However, I would like to thank the following resources for helping me understand the concepts and providing guidance:
- [LearnOpenGL](https://learnopengl.com/)
- [OpenGL Tutorial](https://www.opengl-tutorial.org/)
- [Udemey OpenGL Course](https://www.udemy.com/course/graphics-with-modern-opengl/)
