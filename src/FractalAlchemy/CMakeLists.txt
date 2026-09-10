add_library(FractalAlchemy_Templates INTERFACE)
add_library(FractalAlchemy_Rendering INTERFACE)

add_library(FractalAlchemy
                    Utility.cpp
                    display/VBO.cpp
                    display/EBO.cpp
                    display/VAO.cpp
                    display/Shader.cpp
                    display/Camera.cpp)

include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/glad"
    "${CMAKE_SOURCE_DIR}/include/glm"
)

# include directories needed
target_include_directories(FractalAlchemy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(FractalAlchemy_Templates INTERFACE FA_compiler_flags)
target_link_libraries(FractalAlchemy INTERFACE FA_compiler_flags)
target_link_libraries(FractalAlchemy INTERFACE FractalAlchemy_Templates)
target_link_libraries(FractalAlchemy INTERFACE FractalAlchemy_Rendering)