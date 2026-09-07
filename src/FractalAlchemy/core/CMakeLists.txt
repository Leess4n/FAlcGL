add_library(FractalAlchemy_Templates INTERFACE)

# specify header files
target_sources(FractalAlchemy_Templates INTERFACE
                        FunctionRule.hxx
                        FunctionRelation.hxx
                        RecursiveFunctionRule.hxx)

add_library(FractalAlchemy INTERFACE)

# include directories needed
target_include_directories(FractalAlchemy INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(FractalAlchemy_Templates INTERFACE FA_compiler_flags)
target_link_libraries(FractalAlchemy INTERFACE FA_compiler_flags)
target_link_libraries(FractalAlchemy INTERFACE FractalAlchemy_Templates)