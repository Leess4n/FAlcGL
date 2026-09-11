# FAlcGL

**FAlcGL** (Fractal Alchemy OpenGL) is a lightweight C++ library that allows the rendering of mathematical objects and dynamical systems in 3D using OpenGL.

[![FAlcGL Demo](lorentz_video.gif)](https://github.com/Leess4n/FAlcGL)

## Overview

FAlcGL renders trajectories in space defined by recursive functions; the example video showcases the chaotic behaviour of the
Lorenz attractor. The library separates the core functionality for storing and evaluating recursive functions from rendering:
- **Core**: Template-based mathematical engine handling functions, relations, and recursive behaviour
- **Display layer**: OpenGL for real-time 3D rendering

## Features

- **Flexibility**: Render arbitrary recursive systems including ODEs, maps, and custom trajectories
- **Template Design**: Heavy use of templates for custom numerical typing (float/double/compatible custom types)
- **OpenGL 3.3+ Pipeline**: Implementation of VAO, VBO, and EBO classes and GLSL 330 shaders
- **Multidimensional Support**: Automatic domain generation for systems of arbitrary dimension

## How to Build

```bash
git clone https://github.com/Leess4n/FAlcGL.git
cd FAlcGL

# Build with cmake (requires OpenGL, GLM, GLFW, stb_image)
mkdir build
cmake -S . -B build
cmake --build build
```

The default build produces the **Lorenz attractor** demo. The built executable is `FAlcGL.exe` (Windows) or `FAlcGL` (Unix) and is found
in `build/Debug/`.

## Demo: the Lorenz Attractor

The test program (`src/test.cxx`) visualizes the solution of Lorenz attractor:
```
dx/dt = σ(y − x)
dy/dt = x(ρ − z) − y
dz/dt = xy − βz
```

With parameters σ=10, ρ=28, β=8/3, and an integration step of dt=0.001.

The demo allows to split the stored data in jagged arrays. The first optional parameter controls how many times the data is split
and the second parameter controls how many points are stored in each split segment.
```bash
./build/Debug/FAlcGL                  # Default: data is split in 5 arrays, each with 5000 points
./build/Debug/FAlcGL 3 10000          # Custom: data is split in 3 arrays, each with 10000 points
```

Camera controls: Mouse to rotate view, WASD to move (hold SHIFT to go faster), scroll to zoom.

## Architecture

```
FAlcGL/
├── src/
│   ├── FractalAlchemy/          # Library dir
│   │   ├── core/                # Mathematical implementation (FunctionRule, RecursiveFunctionRule, FunctionRelation)
│   │   ├── display/             # OpenGL rendering (VAO, VBO, EBO, Shader, Camera)
│   │   └── rendering/           # Specialized renderers
│   └── test.cxx                 # Lorenz attractor demo
├── include/                     # Third-party headers (GLAD, GLFW, GLM, stb_image)
├── thirdparty/                  # GLAD and stb_image source
└── CMakeLists.txt               # Build configuration
```

## Building

**Dependencies:**
- OpenGL 3.3+ context
- GLFW 3.3+ (window/context creation)
- GLAD (OpenGL loader)
- GLM (mathematics)
- stb_image (texture loading, not used in current demo)

**Supported Platforms:** Linux, macOS, Windows (via CMake)

See `CMakeLists.txt` for exact dependency handling.