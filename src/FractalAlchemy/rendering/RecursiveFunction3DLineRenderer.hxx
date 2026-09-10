#pragma once

#include <type_traits>

#include "core/RecursiveFunctionManager.hxx"
#include "core/RecursiveFunctionRule.hxx"

#include "display/VAO.h"
#include "display/VBO.h"
#include "display/Camera.h"
#include "display/Shader.h"
#include "display/Camera.h"

#include "helpers/RootDir.h"

#define SHADER(filename) (std::string(SHADER_DIR) + std::string(filename)).c_str()

template<
    real T,
    template<real> class RecursiveFunctionX,
    template<real> class RecursiveFunctionY,
    template<real> class RecursiveFunctionZ>
class RecursiveFunction3DLineRenderer
{
    // Block of static asserts to check that the rules are valid and inherit from the correct parent class
    // ===================================================================================================
    static_assert(
        std::derived_from<RecursiveFunctionX<T>, RecursiveFunctionRule<T>>,
        "RecursiveFunction must derive from RecursiveFunctionRule<T>"
    );
    static_assert(
        std::derived_from<RecursiveFunctionY<T>, RecursiveFunctionRule<T>>,
        "RecursiveFunction must derive from RecursiveFunctionRule<T>"
    );
    static_assert(
        std::derived_from<RecursiveFunctionZ<T>, RecursiveFunctionRule<T>>,
        "RecursiveFunction must derive from RecursiveFunctionRule<T>"
    );
    static_assert(
        std::is_convertible_v<T, GLfloat>,
        "Template for real number must be convertible to GLfloat."
    );

public:
    // Ctors
    RecursiveFunction3DLineRenderer();
    template<typename... Args>
    RecursiveFunction3DLineRenderer(size_t I, size_t J, const T x0, const T t0, const T tFinal, Args... args);

    void Render(Camera& camera);
    int UnsafeUpdateImagesBy(const unsigned int i, const unsigned int span);
    int SafeUpdateImagesBy(const unsigned int span);
    void UpdateImagesFromLastElement();
    void UnsafeUpdateVBOs(const unsigned int i, const unsigned int span);
    void SafeUpdateVBOs();

    Shader& GetShader();

private:
    RecursiveFunctionManager<T, RecursiveFunctionX>* managerX;
    RecursiveFunctionManager<T, RecursiveFunctionY>* managerY;
    RecursiveFunctionManager<T, RecursiveFunctionZ>* managerZ;

    std::vector<VAO> VAOs;
    std::vector<VBO> VBOsX;
    std::vector<VBO> VBOsY;
    std::vector<VBO> VBOsZ;
    Shader shader = Shader(SHADER("simple_3d_line.vert"), SHADER("simple_3d_line.frag"));

    // For safe plotting and updating
    unsigned int currentImageIndex = 0, updateSpan = 0;
    unsigned int I, J, totalSize;
};

#include "RecursiveFunction3DLineRenderer_impl.hxx"