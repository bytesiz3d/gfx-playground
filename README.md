# gfx-playground
- This is a playground for testing OpenGL features, or starting bigger projects.
- It contains:
    - [GLAD](https://github.com/Dav1dde/glad), for loading OpenGL functions.
        - P.S. GLAD needs Python :/
    - [GLFW](https://github.com/glfw/glfw), for handling windows.
  
## Startup guide
- Clone the repository (you may add to it later as you see fit)
    - **--recurse-submodules** initializes third-party libraries.
      ```
      git clone https://github.com/XL3/gfx-playground --recurse-submodules my_project
      ```
- Build using [CMake](https://cmake.org/), an out-of-source build (*building to a different directory*) is always welcome.
    - An example build command using MinGW would be:
      ```
      cmake -G "MinGW Makefiles" -B build && cmake --build build
      ```
