#pragma once

#include <stdexcept>
#include <concepts>

#include "FunctionRule.hxx"
#include "FunctionRelation.hxx"

template<real T>
class FunctionRelation;

template<real T>
class RecursiveFunctionRule : public FunctionRule<T>
{
public:
    RecursiveFunctionRule(const T x0);

    // function rule
    inline virtual const T next(const T x) const;
    inline virtual const T rule(FunctionRelation<T> **params, const unsigned int i) const;
    virtual const T operator()(const T x, const T dx);

    // getter functions
    inline virtual const T getInitValue();
    inline virtual const T getFromPreviousInImage(FunctionRelation<T> **params, const size_t i) const;

private:

    const T x0; // initial conditions
};

#include "RecursiveFunctionRule_impl.hxx"