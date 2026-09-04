#pragma once

#include <concepts>
#include <iostream>

#include "FunctionRule.hxx"
#include "RecursiveFunctionRule.hxx"

template<real T>
class RecursiveFunctionRule;

template<real T>
class FunctionRelation
{
public:
    // Ctors
    FunctionRelation(size_t I, size_t J);
    FunctionRelation(size_t I, size_t J, T **domain);
    ~FunctionRelation();

    // populate whole domain and image
    void populateDomainFromInterval(const T x0, const T x1);
    void populateDomainFromSpacing(const T x0, const T dx);
    void populateMultidimDomainFromInterval(const T x0, const T x1, unsigned int* shape, const unsigned int dim, unsigned int max_dims);
    void populateImageFromSimpleFunc(FunctionRule<T> &f);
    void populateImageFromSingleRecursiveFunc(RecursiveFunctionRule<T> &f, FunctionRelation<T> **func);
    inline void modifyFromSimpleFunc(FunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params);
    inline void modifyFromPrevRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params);
    inline void modifyFromLastRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params);
 
    // getter functions
    inline const T getDomainElem(const unsigned int i) const;
    inline const T getImageElem(const unsigned int i) const;
    inline T** getDomain() const;
    inline T** getImage() const;
    inline const bool isDomainAssigned() const;
    inline const bool isDomainAllocated() const;
    inline const T getdx() const;
    inline const size_t getI() const;
    inline const size_t getJ() const;

    // setter functions
    inline void setDomainElem(const T x, const unsigned int i);
    inline void setImageElem(const T x, const unsigned int i);

protected:
    const int getSkip(unsigned int* shape, unsigned int dim, unsigned int max_dim) const;

protected:
    // Declare domain and image of function as ptrs
    T **domain;
    T **image;

    size_t I, J;
    T dx;
    bool domainAssigned = false, domainAllocated = false, imageAssigned = false;
};

#include "FunctionRelation_impl.hxx"