# Simple C Raycaster game

This repository contains a simple game written in C using SDL2 library for visuals.

Try it [here](https://mantonic002.github.io/raycaster.github.io/) compiled as WebAssembly using emscripten.
## What's Inside?

- ### Simple 2D game turned into a pseudo 3D game using rays.
  
  ![movement](https://github.com/mantonic002/raycaster/assets/102149589/73c332eb-c4ea-4540-a966-a9a6bf7a5751)
  
- With different textures set with values ```1-3``` in ```map``` array in ```main.c``` file and doors set with value ```4```.
   
  ![textures](https://github.com/mantonic002/raycaster/assets/102149589/acd3a929-ebc4-40a8-bff0-4e93f73f8143)

- Doors can be opened by standing in front of them and pressing ```E```

  ![doors](https://github.com/mantonic002/raycaster/assets/102149589/cc24a84e-9241-4251-973e-c7640add8806)


### How to Use

1. **Clone the Repository:** Get the code onto your computer by copying this repository.
2. **Open Terminal:** Launch your terminal and navigate to the repository folder (`cd raycaster/`).
3. **Compile and Run:** `make`to compile, then start the game (`./game` or `make run`).

### What You Need

- **GCC Compiler:** You'll need a compiler to turn the code into a working program.
- **SDL2 Library:** To see the visual representation, you'll need the SDL2 library.


