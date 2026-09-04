template<real T, template <real> class Function>
FunctionManager<T, Function>::FunctionManager(FunctionRelation<T> *relation, Function<T> *rule)
{
    this->relation = relation;
    this->rule = rule;
}

template<real T, template <real> class Function>
template<typename... Args>
FunctionManager<T, Function>::FunctionManager(size_t I, size_t J, Args... args)
{
    rule = new Function<T>(args...);
    relation = new FunctionRelation<T>(I, J);

    isFuncAllocated = true;
}

template<real T, template <real> class Function>
template<typename... Args>
FunctionManager<T, Function>::FunctionManager(size_t I, size_t J, T **domain, Args... args)
{
    rule = new Function<T>(args...);
    relation = new FunctionRelation<T>(I, J, domain);

    isFuncAllocated = true;
}

template<real T, template <real> class Function>
FunctionManager<T, Function>::~FunctionManager()
{
    if (isFuncAllocated)
    {
        delete rule;
        delete relation;
    }
}

template<real T, template <real> class Function>
void FunctionManager<T, Function>::InitDomainFromInterval(const T x0, const T x1)
{
    relation->populateDomainFromInterval(x0, x1);
}

template<real T, template <real> class Function>
void FunctionManager<T, Function>::InitDomainFromSpacing(const T x0, const T dx)
{
    relation->populateDomainFromSpacing(x0, dx);
}

template<real T, template <real> class Function>
void FunctionManager<T, Function>::InitMultidimDomainFromInterval(const T x0, const T x1, unsigned int* shape, const unsigned int dim, unsigned int max_dims)
{
    relation->populateMultidimDomainFromInterval(x0, x1, shape, dim, max_dims);
}

template<real T, template <real> class Function>
void FunctionManager<T, Function>::modifyFromCurrent(const unsigned int i, FunctionRelation<T> **params)
{
    relation->modifyFromSimpleFunc(*rule, i, params);
}

template<real T, template <real> class Function>
inline FunctionRelation<T>* FunctionManager<T, Function>::getRelation() const
{
    return relation;
}

template<real T, template <real> class Function>
inline const T FunctionManager<T, Function>::getDomainElem(const unsigned int i) const
{
    return relation->getDomainElem(i);
}

template<real T, template <real> class Function>
inline const T FunctionManager<T, Function>::getImageElem(const unsigned int i) const
{
    return relation->getImageElem(i);
}

template<real T, template <real> class Function>
inline T** FunctionManager<T, Function>::getDomain() const
{
    return relation->getDomain();
}

template<real T, template <real> class Function>
inline T** FunctionManager<T, Function>::getImage() const
{
    return relation->getImage();
}

template<real T, template <real> class Function>
inline const bool FunctionManager<T, Function>::isDomainAssigned() const
{
    return relation->isDomainAssigned();
}

template<real T, template <real> class Function>
inline const bool FunctionManager<T, Function>::isDomainAllocated() const
{
    return relation->isDomainAllocated();
}

template<real T, template <real> class Function>
inline const T FunctionManager<T, Function>::getdx() const
{
    return relation->getdx();
}

template<real T, template <real> class Function>
inline const size_t FunctionManager<T, Function>::getI() const
{
    return relation->getI();
}

template<real T, template <real> class Function>
inline const size_t FunctionManager<T, Function>::getJ() const
{
    return relation->getJ();
}

template<real T, template <real> class Function>
inline const T FunctionManager<T, Function>::getx0() const
{
    return rule->getInitValue();
}

template<real T, template <real> class Function>
inline void FunctionManager<T, Function>::setDomainElem(const T x, const unsigned int i)
{
    relation->setDomainElem(x, i);
}

template<real T, template <real> class Function>
inline void FunctionManager<T, Function>::setImageElem(const T x, const unsigned int i)
{
    relation->setImageElem(x, i);
}