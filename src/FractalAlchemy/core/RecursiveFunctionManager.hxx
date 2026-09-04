#pragma once

#include <concepts>

#include "FunctionRule.hxx"
#include "RecursiveFunctionRule.hxx"
#include "FunctionRelation.hxx"

template <typename Derived, typename T>
concept RecursiveFunction = std::is_base_of<RecursiveFunctionRule<T>, Derived>::value;

template<real T, template <real> class RecursiveFunction>
class RecursiveFunctionManager
{
public:
    RecursiveFunctionManager(FunctionRelation<T> *relation, RecursiveFunction<T> *rule);
    template<typename... Args>
    RecursiveFunctionManager(size_t I, size_t J, const T x0, Args... args);
    template<typename... Args>
    RecursiveFunctionManager(size_t I, size_t J, T **domain, const T x0, Args... args);
    ~RecursiveFunctionManager();

    void InitDomainAndX0(const T x0, const T x1);
    void InitX0();
    void modifyFromLast(const unsigned int i, FunctionRelation<T> **params);
    void modifyFromPrev(const unsigned int i, FunctionRelation<T> **params);
    
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
    RecursiveFunction<T> *rule;

    bool isFuncAllocated = false;
};

#include "RecursiveFunctionManager_impl.hxx"