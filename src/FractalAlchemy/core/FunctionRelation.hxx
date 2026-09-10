#pragma once

#include <concepts>

#include "FunctionRule.hxx"
#include "RecursiveFunctionRule.hxx"

template<real T>
class RecursiveFunctionRule;


/*
Function relation class. It stores the elements of the domain and the corresponding image
mimicking the behaviour of the mathematical concept of relation. For a function this means
relating an element from the domain to an element in the codomain.

This class stores domain and codomain as jagged arrays to provide a flexible system in case
of large arrays
*/
template<real T>
class FunctionRelation
{
public:
    // Ctors
    // =====
    FunctionRelation(size_t I, size_t J);             // Initializes a domain and image by allocating necessary memory. I*J is the size of the domain and image
    FunctionRelation(size_t I, size_t J, T **domain); // Initializes a domain and image by using a pointer to another FunctionRelation's domain
    ~FunctionRelation();                              // Default ctor

    // populate whole domain and image
    void PopulateDomainFromInterval(const T x0, const T x1);                                            // evenly spaces out points in the domain from a given interval 
    void PopulateDomainFromSpacing(const T x0, const T dx);
    void PopulateMultidimDomainFromInterval(const T x0, const T x1, unsigned int* shape, const unsigned int dim, unsigned int max_dims);
    void PopulateImageFromSimpleFunc(FunctionRule<T> &f);                                               // given a FunctionRule object creates an image calling the operator() of the function
    void PopulateImageFromSingleRecursiveFunc(RecursiveFunctionRule<T> &f, FunctionRelation<T> **func); // creates an image from a recursive function
    // modify image
    void ModifyFromSimpleFunc(FunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params);                              // modifies the image using the rule of `f` to the element of index `i` in the domain
    inline void ModifyFromPrevRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params); // modifies an element in the image at index `i` using rule `f` on the previous value
    inline void ModifyFromPrevRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, const unsigned int j, FunctionRelation<T> **params);
    inline void ModifyFromLastRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params); // modifies the last element in the image at index `i` using rule `f` on the previous value
    inline void ModifyFromLastRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, const unsigned int j, FunctionRelation<T> **params); 

    // getter functions
    inline const T GetDomainElem(const unsigned int i) const;
    inline const T GetDomainElem(const unsigned int i, const unsigned int j) const;
    inline const T GetImageElem(const unsigned int i) const;
    inline const T GetImageElem(const unsigned int i, const unsigned int j) const;
    inline T** GetDomain() const;
    inline T** GetImage() const;
    inline T* GetDomainArray(const unsigned int i) const;
    inline T* GetImageArray(const unsigned int i) const;
    inline const bool IsDomainAssigned() const;
    inline const bool IsDomainAllocated() const;
    inline const T Getdx() const;
    inline const size_t GetI() const;
    inline const size_t GetJ() const;

    // setter functions
    inline void SetDomainElem(const T x, const unsigned int i);
    inline void SetDomainElem(const T x, const unsigned int i, const unsigned int j);
    inline void SetImageElem(const T x, const unsigned int i);
    inline void SetImageElem(const T x, const unsigned int i, const unsigned int j);

protected:
    const int getSkip(unsigned int* shape, unsigned int dim, unsigned int max_dim) const;

private:
    // domain and image are jagged arrays of size I containing arrays of size J
    T **domain;
    T **image;

    // dimesions of arrays
    size_t I, J;
    // distances between consecutive elements in domain
    T dx;
    // variables for safety
    bool domainAssigned = false, domainAllocated = false, imageAssigned = false;
};

#include "FunctionRelation_impl.hxx"