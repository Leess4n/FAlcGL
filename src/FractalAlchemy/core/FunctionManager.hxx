#pragma once

#include <concepts>

#include "FunctionRule.hxx"
#include "FunctionRelation.hxx"

template <typename Derived, typename T>
concept Function = std::is_base_of<FunctionRule<T>, Derived>::value;

template<real T, template <real> class Function>
class FunctionManager: public FunctionRelation<T>
{
public:
    FunctionManager(Function<T> *rule);
    FunctionManager(size_t I, size_t J);
    FunctionManager(size_t I, size_t J, T **domain);
    template<typename... Args>
    FunctionManager(size_t I, size_t J, Args... args);
    template<typename... Args>
    FunctionManager(size_t I, size_t J, T **domain, Args... args);
    ~FunctionManager();

    void ModifyFromCurrent(const unsigned int i, FunctionRelation<T> **params);
    
    // getter functions
    inline const T Getx0() const;

private:
    Function<T> *rule;

    bool isFuncAllocated = false;
};

#include "FunctionManager_impl.hxx"