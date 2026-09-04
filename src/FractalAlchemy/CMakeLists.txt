add_library(FractalAlchemy_Templates INTERFACE)

# specify header files
target_sources(FractalAlchemy_Templates INTERFACE
                        core/FunctionRule.hxx
                        core/FunctionRelation.hxx
                        core/FunctionManager.hxx
                        core/RecursiveFunctionRule.hxx
                        core/RecursiveFunctionManager.hxx)

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