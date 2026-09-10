#include "RecursiveFunctionManager.hxx"
template<real T, template <real> class RecursiveFunction>
template<typename... Args>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(size_t I, size_t J, const T x0, Args... args)
: FunctionManager<T, RecursiveFunction>(I, J)
{
    // create desired objects
    rule = new RecursiveFunction<T>(x0, args...);

    isFuncAllocated = true;
}

template<real T, template <real> class RecursiveFunction>
template<typename... Args>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(size_t I, size_t J, T **domain, const T x0, Args... args)
: FunctionManager<T, RecursiveFunction>(I, J, domain)
{
    // create objects giving the specified domain
    rule = new RecursiveFunction<T>(x0, args...);

    isFuncAllocated = true;
}

template<real T, template <real> class RecursiveFunction>
RecursiveFunctionManager<T, RecursiveFunction>::~RecursiveFunctionManager()
{
    // delete the rule and relation if they have been created in this instance
    if (isFuncAllocated)
    {
        delete rule;
    }
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::InitDomainAndX0(const T x0, const T x1)
{
    // Inits domain and the first value in the image
    this->PopulateDomainFromInterval(x0, x1);
    this->SetImageElem(rule->GetInitValue(), 0);
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::InitX0()
{
    this->SetImageElem(rule->GetInitValue(), 0);
}

template<real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::ModifyFromPrev(const unsigned int i, FunctionRelation<T> **params)
{
    this->ModifyFromPrevRecursiveFunc(*rule, i, params);
}

template<real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::ModifyFromPrev(const unsigned int i, const unsigned int j, FunctionRelation<T> **params)
{
    this->ModifyFromPrevRecursiveFunc(*rule, i, j, params);
}

template <real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::ModifyFromLast(const unsigned int i, FunctionRelation<T> **params)
{
    this->ModifyFromLastRecursiveFunc(*rule, i, params);
}

template<real T, template <real> class RecursiveFunction>
inline const T RecursiveFunctionManager<T, RecursiveFunction>::Getx0() const
{
    return rule->GetInitValue();
}