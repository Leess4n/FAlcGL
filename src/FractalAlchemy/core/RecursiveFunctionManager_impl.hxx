template<real T, template <real> class RecursiveFunction>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(RecursiveFunction<T> *rule)
{
    this->rule = rule;
}

template<real T, template <real> class RecursiveFunction>
template<typename... Args>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(size_t I, size_t J, const T x0, Args... args)
: FunctionManager<T>(I, J)
{
    // create desired objects
    rule = new RecursiveFunction<T>(x0, args...);

    isFuncAllocated = true;
}

template<real T, template <real> class RecursiveFunction>
template<typename... Args>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(size_t I, size_t J, T **domain, const T x0, Args... args)
: FunctionManager<T>(I, J, domain)
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
    this->populateDomainFromInterval(x0, x1);
    this->setImageElem(rule->getInitValue(), 0);
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::InitX0()
{
    this->setImageElem(rule->getInitValue(), 0);
}

template<real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::modifyFromPrev(const unsigned int i, FunctionRelation<T> **params)
{
    this->modifyFromPrevRecursiveFunc(*rule, i, params);
}

template<real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::modifyFromLast(const unsigned int i, FunctionRelation<T> **params)
{
    this->modifyFromLastRecursiveFunc(*rule, i, params);
}


template<real T, template <real> class RecursiveFunction>
inline const T RecursiveFunctionManager<T, RecursiveFunction>::getx0() const
{
    return rule->getInitValue();
}