template<real T>
RecursiveFunctionRule<T>::RecursiveFunctionRule(const T x0)
    : FunctionRule<T>(), x0(x0)
{

}

template<real T>
inline const T RecursiveFunctionRule<T>::next(FunctionRelation<T> **params, const unsigned int i) const
{
    // placeholder rule
    return params[0]->getImageElem(i)+1;
}

template<real T>
const T RecursiveFunctionRule<T>::operator()(const T x, const T dx)
{
    T f = x0;
    for (T i = 0; i<(unsigned int)(x/dx); i++)
    {
        f = next(f);
    }

    return f;
}

template<real T>
inline const T RecursiveFunctionRule<T>::getInitValue()
{
    return x0;
}

// Removable stuff

template<real T>
inline const T RecursiveFunctionRule<T>::getFromPreviousInImage(FunctionRelation<T> **params, const size_t i) const
{
    return next(params, i-1);
}