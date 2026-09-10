#include "FunctionManager.hxx"

template<real T, template <real> class Function>
FunctionManager<T, Function>::FunctionManager(Function<T> *rule)
{
    this->rule = rule;
}

template <real T, template <real> class Function>
inline FunctionManager<T, Function>::FunctionManager(size_t I, size_t J) : FunctionRelation<T>(I, J)
{
}

template <real T, template <real> class Function>
inline FunctionManager<T, Function>::FunctionManager(size_t I, size_t J, T **domain) : FunctionRelation<T>(I, J, domain)
{
}

template <real T, template <real> class Function>
template <typename... Args>
FunctionManager<T, Function>::FunctionManager(size_t I, size_t J, Args... args) : FunctionRelation<T>(I, J)
{
    rule = new Function<T>(args...);

    isFuncAllocated = true;
}

template<real T, template <real> class Function>
template<typename... Args>
FunctionManager<T, Function>::FunctionManager(size_t I, size_t J, T **domain, Args... args) : FunctionRelation<T>(I, J, domain)
{
    rule = new Function<T>(args...);

    isFuncAllocated = true;
}

template<real T, template <real> class Function>
FunctionManager<T, Function>::~FunctionManager()
{
    if (isFuncAllocated)
    {
        delete rule;
    }
}

template<real T, template <real> class Function>
void FunctionManager<T, Function>::ModifyFromCurrent(const unsigned int i, FunctionRelation<T> **params)
{
    this->ModifyFromSimpleFunc(*rule, i, params);
}

template <real T, template <real> class Function>
inline const T FunctionManager<T, Function>::Getx0() const
{
    return rule->getInitValue();
}