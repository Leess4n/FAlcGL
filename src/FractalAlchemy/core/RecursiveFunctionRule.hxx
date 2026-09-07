#pragma once

#include <stdexcept>
#include <concepts>

#include "FunctionRule.hxx"
#include "FunctionRelation.hxx"

template<real T>
class FunctionRelation;

/*
Specific implementation of a class wrapper for a recursive function. This type of functions
evaluate a value recursively.
*/
template<real T>
class RecursiveFunctionRule : public FunctionRule<T>
{
public:
    RecursiveFunctionRule(const T x0);

    // function rule
    inline virtual const T next(FunctionRelation<T> **params, const unsigned int i) const; // Returns the value at index `i+1` using the values in `i` from the domains of the domains in params
    virtual const T operator()(const T x, const T dx); // iteratively goes through numbers from x0 to x in steps of dx to return the value of the function at x

    // getter functions
    inline virtual const T getInitValue();
    inline virtual const T getFromPreviousInImage(FunctionRelation<T> **params, const size_t i) const;  // returns the next value by calling the rule

private:

    // this is a placeholder
    const T x0; // initial conditions
};

#include "RecursiveFunctionRule_impl.hxx"