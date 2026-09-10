template<real T>
RecursiveFunctionRule<T>::RecursiveFunctionRule(const T x0)
    : FunctionRule<T>(), x0(x0)
{

}

template<real T>
inline const T RecursiveFunctionRule<T>::Next(FunctionRelation<T> **params, const unsigned int i) const
{
    // placeholder rule
    return params[0]->GetImageElem(i)+1;
}

template<real T>
inline const T RecursiveFunctionRule<T>::GetInitValue()
{
    return x0;
}

// Removable stuff

template<real T>
inline const T RecursiveFunctionRule<T>::GetFromPreviousInImage(FunctionRelation<T> **params, const size_t i) const
{
    return Next(params, i-1);
}