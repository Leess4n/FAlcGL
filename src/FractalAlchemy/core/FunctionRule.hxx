#pragma once

#include <concepts>
#include <type_traits>

template<typename T>
concept real = std::is_floating_point<T>::value;

/*
This is the base wrapper class that mimics the mathematical concept of a function. It has
basic components of continuous functions just as utils but it can be any function.
*/
template<real T>
class FunctionRule
{
public:
    FunctionRule() {};

    // mathematical rule of the function
    virtual inline const T operator()(const T x);

    inline const T d(const T x, const T dx = 0.001);                                     // Derivative at a point with precision `dx`
    const T intgr(const T x0, const T x1, const T dx = 0.001);                           // By default the integral is calculated with the trapezium method
    const T newtonSolve(const T x0, const unsigned int iter = 10, const T dx = 0.001);   // Gives a solution of the function using Newton's algorithm
};

#include "FunctionRule_impl.hxx"