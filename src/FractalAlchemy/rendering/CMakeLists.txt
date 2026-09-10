add_library(FractalAlchemy_Rendering INTERFACE)

# specify header files
target_sources(FractalAlchemy_Rendering INTERFACE
                        RecursiveFunction3DLineRenderer.hxx)

target_include_directories(FractalAlchemy_Rendering
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FractalAlchemy
)