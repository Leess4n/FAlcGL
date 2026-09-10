#pragma once

#include "FunctionRule.hxx"
#include "RecursiveFunctionRule.hxx"
#include "FunctionRelation.hxx"
#include "FunctionManager.hxx"


/*
Class that manages a function rule and a function relation. In this way a function is linked to
its domain and image.
*/
template<real T, template<real> class RecursiveFunction>
class RecursiveFunctionManager : public FunctionManager<T, RecursiveFunction>
{
    static_assert(
        std::derived_from<RecursiveFunction<T>, RecursiveFunctionRule<T>>,
        "RecursiveFunction must derive from RecursiveFunctionRule<T>"
    );
public:
    // Ctors
    template<typename... Args>
    RecursiveFunctionManager(size_t I, size_t J, const T x0, Args... args);               // Initializes a domain and creates an instance of the RecursiveFunction class specified in the template of the RecursiveFunctionManager class
    template<typename... Args>
    RecursiveFunctionManager(size_t I, size_t J, T **domain, const T x0, Args... args);   // Uses a domain from another function and creates an instance of the RecursiveFunction class specified in the template of the RecursiveFunctionManager class
    ~RecursiveFunctionManager();                                                          // Destructor

    void InitDomainAndX0(const T x0, const T x1);                                         // Inits the domain and first value in image
    void InitX0();                                                                        // Inits first value in image
    inline void ModifyFromLast(const unsigned int i, FunctionRelation<T> **params);              // Modifies the element at index i using the last elements of the images of params
    inline void ModifyFromPrev(const unsigned int i, FunctionRelation<T> **params);              // Modifies the element at index i using the elements at index i-1 from the images of params
    inline void ModifyFromPrev(const unsigned int i, const unsigned int j, FunctionRelation<T> **params);              // Modifies the element at index i using the elements at index i-1 from the images of params
    
    // getter functions
    inline const T Getx0() const;

private:
    // The function rule and relation
    RecursiveFunction<T> *rule;

    // variables for safety
    bool isFuncAllocated = false;
};

#include "RecursiveFunctionManager_impl.hxx"