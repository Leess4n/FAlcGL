template<real T, template <real> class RecursiveFunction>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(FunctionRelation<T> *relation, RecursiveFunction<T> *rule)
{
    this->relation = relation;
    this->rule = rule;
}

template<real T, template <real> class RecursiveFunction>
template<typename... Args>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(size_t I, size_t J, const T x0, Args... args)
{
    rule = new RecursiveFunction<T>(x0, args...);
    relation = new FunctionRelation<T>(I, J);

    isFuncAllocated = true;
}

template<real T, template <real> class RecursiveFunction>
template<typename... Args>
RecursiveFunctionManager<T, RecursiveFunction>::RecursiveFunctionManager(size_t I, size_t J, T **domain, const T x0, Args... args)
{
    rule = new RecursiveFunction<T>(x0, args...);
    relation = new FunctionRelation<T>(I, J, domain);

    isFuncAllocated = true;
}

template<real T, template <real> class RecursiveFunction>
RecursiveFunctionManager<T, RecursiveFunction>::~RecursiveFunctionManager()
{
    if (isFuncAllocated)
    {
        delete rule;
        delete relation;
    }
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::InitDomainAndX0(const T x0, const T x1)
{
    relation->populateDomainFromInterval(x0, x1);
    relation->setImageElem(rule->getInitValue(), 0);
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::InitX0()
{
    relation->setImageElem(rule->getInitValue(), 0);
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::modifyFromPrev(const unsigned int i, FunctionRelation<T> **params)
{
    relation->modifyFromPrevRecursiveFunc(*rule, i, params);
}

template<real T, template <real> class RecursiveFunction>
void RecursiveFunctionManager<T, RecursiveFunction>::modifyFromLast(const unsigned int i, FunctionRelation<T> **params)
{
    relation->modifyFromLastRecursiveFunc(*rule, i, params);
}

template<real T, template <real> class RecursiveFunction>
inline FunctionRelation<T>* RecursiveFunctionManager<T, RecursiveFunction>::getRelation() const
{
    return relation;
}

template<real T, template <real> class RecursiveFunction>
inline const T RecursiveFunctionManager<T, RecursiveFunction>::getDomainElem(const unsigned int i) const
{
    return relation->getDomainElem(i);
}

template<real T, template <real> class RecursiveFunction>
inline const T RecursiveFunctionManager<T, RecursiveFunction>::getImageElem(const unsigned int i) const
{
    return relation->getImageElem(i);
}

template<real T, template <real> class RecursiveFunction>
inline T** RecursiveFunctionManager<T, RecursiveFunction>::getDomain() const
{
    return relation->getDomain();
}

template<real T, template <real> class RecursiveFunction>
inline T** RecursiveFunctionManager<T, RecursiveFunction>::getImage() const
{
    return relation->getImage();
}

template<real T, template <real> class RecursiveFunction>
inline const bool RecursiveFunctionManager<T, RecursiveFunction>::isDomainAssigned() const
{
    return relation->isDomainAssigned();
}

template<real T, template <real> class RecursiveFunction>
inline const bool RecursiveFunctionManager<T, RecursiveFunction>::isDomainAllocated() const
{
    return relation->isDomainAllocated();
}

template<real T, template <real> class RecursiveFunction>
inline const T RecursiveFunctionManager<T, RecursiveFunction>::getdx() const
{
    return relation->getdx();
}

template<real T, template <real> class RecursiveFunction>
inline const size_t RecursiveFunctionManager<T, RecursiveFunction>::getI() const
{
    return relation->getI();
}

template<real T, template <real> class RecursiveFunction>
inline const size_t RecursiveFunctionManager<T, RecursiveFunction>::getJ() const
{
    return relation->getJ();
}

template<real T, template <real> class RecursiveFunction>
inline const T RecursiveFunctionManager<T, RecursiveFunction>::getx0() const
{
    return rule->getInitValue();
}

template<real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::setDomainElem(const T x, const unsigned int i)
{
    relation->setDomainElem(x, i);
}

template<real T, template <real> class RecursiveFunction>
inline void RecursiveFunctionManager<T, RecursiveFunction>::setImageElem(const T x, const unsigned int i)
{
    relation->setImageElem(x, i);
}