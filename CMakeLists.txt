cmake_minimum_required(VERSION 3.20)

project(FATest VERSION 0.1)

# link /lib directory
link_directories(${CMAKE_SOURCE_DIR}/lib)

# create interface library to give both FATest and FractalAlchemy the compiler features
add_library(FA_compiler_flags INTERFACE)
target_compile_features(FA_compiler_flags INTERFACE cxx_std_20)

# add the library subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/FractalAlchemy)

# configure the RootDir file to contain a macro to the absolute path of this directory (for resource management in /res dir)
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src/helpers)

# add the executable and the source of it
add_executable(FATest ${CMAKE_SOURCE_DIR}/src/test.cxx)

# setting module path for module finding
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# ASSIMP
# find_package(ASSIMP REQUIRED)
# message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# STB_IMAGE
add_library(STB_IMAGE "thirdparty/stb_image.cpp")

# GLAD - header only so add library
add_library(GLAD "thirdparty/glad.c")

# put all libraries in a variable
set(LIBS GLAD glfw3 opengl32 STB_IMAGE)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
)

# Link all remaining libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
# link FAlc and the compiler flags
target_link_libraries(${PROJECT_NAME} PUBLIC FractalAlchemy FA_compiler_flags)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
endif()

# Copy dlls
# if(WIN32)
# 	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory
# 		"${PROJECT_SOURCE_DIR}/dlls"
# 		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
# endif()