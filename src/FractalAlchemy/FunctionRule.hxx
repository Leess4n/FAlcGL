#pragma once

#include <concepts>
#include <type_traits>

template<typename T>
concept real = std::is_floating_point<T>::value;

template<real T>
class FunctionRelation;

template<real T>
class FunctionRule
{
public:
    FunctionRule() {};

    virtual inline const T operator()(const T x);
    inline virtual const T rule(FunctionRelation<T> **params, const unsigned int i) const;

    inline const T d(const T x, const T dx = 0.001);
    const T intgr(const T x0, const T x1, const T dx = 0.001);                  // By default the integral is calculated with the trapezium method
    const T newtonSolve(const T x0, const unsigned int iter = 10, const T dx = 0.001);
};

#include "FunctionRule_impl.hxx"