# Welcome to FAlc GL

<center>
    <img src="lorentz_video.gif" width="900" height="450">   
</center>

FAlcGL is a C++ library that displays mathematical objects in 3D using OpenGL. As of the last update, the library
allows to render any type of trajectory in space and evolve it in time. It makes use of jagged arrays that allow
to split large amounts of data across memory. It also makes it easy to create recursive functions (or systems
of recursive functions) and evaluate them on the domain of choice. The template classes of the core library
allow for flexibility in development.

## Build Instructions

The library can be built by running the cmake commands:

```sh
cmake -S . -B build
cmake --build build
```

By default this builds the file `src/text.cxx`.