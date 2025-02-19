add_library(FractalAlchemy_Templates INTERFACE)

# specify header files
target_sources(FractalAlchemy_Templates INTERFACE
                        FunctionRule.hxx
                        FunctionRelation.hxx
                        RecursiveFunctionRule.hxx
                        RecursiveFunctionManager.hxx)

add_library(FractalAlchemy
                    Utility.cpp
                    VBO.cpp
                    EBO.cpp
                    VAO.cpp
                    Shader.cpp
                    Camera.cpp)

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