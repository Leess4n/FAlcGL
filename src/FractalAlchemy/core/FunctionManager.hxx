#pragma once

#include <concepts>

#include "FunctionRule.hxx"
#include "FunctionRelation.hxx"

template <typename Derived, typename T>
concept Function = std::is_base_of<FunctionRule<T>, Derived>::value;

template<real T, template <real> class Function>
class FunctionManager
{
public:
    FunctionManager(FunctionRelation<T> *relation, Function<T> *rule);
    template<typename... Args>
    FunctionManager(size_t I, size_t J, Args... args);
    template<typename... Args>
    FunctionManager(size_t I, size_t J, T **domain, Args... args);
    ~FunctionManager();

    void InitDomainFromInterval(const T x0, const T x1);
    void InitDomainFromSpacing(const T x0, const T dx);
    void InitMultidimDomainFromInterval(const T x0, const T x1, unsigned int* shape, const unsigned int dim, unsigned int max_dims);
    void modifyFromCurrent(const unsigned int i, FunctionRelation<T> **params);
    
    // getter functions
    inline FunctionRelation<T>* getRelation() const;
    inline const T getDomainElem(const unsigned int i) const;
    inline const T getImageElem(const unsigned int i) const;
    inline T** getDomain() const;
    inline T** getImage() const;
    inline const bool isDomainAssigned() const;
    inline const bool isDomainAllocated() const;
    inline const T getdx() const;
    inline const size_t getI() const;
    inline const size_t getJ() const;
    inline const T getx0() const;

    // setter functions
    inline void setDomainElem(const T x, const unsigned int i);
    inline void setImageElem(const T x, const unsigned int i);

private:
    FunctionRelation<T> *relation;
    Function<T> *rule;

    bool isFuncAllocated = false;
};

#include "FunctionManager_impl.hxx"