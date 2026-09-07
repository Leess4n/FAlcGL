#include "FunctionRelation.hxx"
template<real T>
FunctionRelation<T>::FunctionRelation(size_t I, size_t J)
{
    this->I = I;
    this->J = J;

    // specify size of jagged array
    domain = new T*[I];
    image = new T*[I];
    for (unsigned int i=0; i<I; i++)
    {
        // specify size of the array contained
        domain[i] = new T [J];
        image[i] = new T [J];
        // std::cout << "Array assigned" << std::endl;
    }

    domainAllocated = true;
}

template<real T>
FunctionRelation<T>::FunctionRelation(size_t I, size_t J, T **domain)
{
    this->I = I;
    this->J = J;
    // set the domain (it will be destroyed in another instance, be careful with this)
    this->domain = domain;

    // just set the image
    image = new T*[I];
    for (unsigned int i=0; i<I; i++)
    {
        image[i] = new T [J];
    }
}

template<real T>
FunctionRelation<T>::~FunctionRelation()
{
    // delete image
    for (unsigned int i=0; i<I; i++)
    {
        delete[] image[i];
    }
    delete[] image;

    // if memory was allocated in this instance delete it
    if (domainAllocated)
    {
        for (unsigned int i=0; i<I; i++)
        {
            delete[] domain[i];
        }
        delete[] domain;
    }
}

template<real T>
void FunctionRelation<T>::PopulateDomainFromInterval(const T x0, const T x1)
{
    // iteratively assign values to the domain
    const T dx = (x1-x0)/((T)I*(T)J);
    for (unsigned int i=0; i<I; i++)
    { 
        for (unsigned int j=0; j<J; j++)
        {
            domain[i][j] = x0 + (dx*((i*(T)J)+j));
        }
    }

    // domain is assigned
    domainAssigned = true;
    this->dx = dx;
}

template<real T>
void FunctionRelation<T>::PopulateDomainFromSpacing(const T x0, const T x1)
{
    for (unsigned int i=0; i<I; i++)
    { 
        for (unsigned int j=0; j<J; j++)
        {
            domain[i][j] = x0 + (dx*((i*(T)J)+j));
        }
    }

    // domain is assigned
    domainAssigned = true;
    this->dx = dx;
}

template<real T>
void FunctionRelation<T>::PopulateMultidimDomainFromInterval(const T x0, const T x1, unsigned int* shape, const unsigned int dim, unsigned int max_dims)
{
    int expected_elem_num = 1;
    for (int i=0; i<max_dims; i++)
    {
        expected_elem_num *= shape[i];
    }
    if (I*J!=expected_elem_num)
    {
        std::cout << "calling err\n";
        std::cerr << "Bad split of domain. The size of the domain " << I*J << " is not precisely the product of sizes along the specified directions " << expected_elem_num << std::endl;
    }

    const int count = shape[dim];
    const int skip = getSkip(shape, dim, max_dims);
    const T dx = (x1-x0)/(T)count;
    for (unsigned int i=0; i<I*J; i++)
    {
        this->SetDomainElem(x0 + (T)((T)dx*static_cast<int>(i/count)), (i * skip)%(I*J) + ((int)(i * skip)/(int)(I*J)));
    }

    // domain is assigned
    domainAssigned = true;
    this->dx = dx;
}

template<real T>
void FunctionRelation<T>::PopulateImageFromSimpleFunc(FunctionRule<T> &f)
{
    // give a value in the image for every element in the domain
    for (unsigned int i=0; i<I; i++)
    { 
        for (unsigned int j=0; j<J; j++)
        {
            image[i][j] = f(GetDomainElem((unsigned int)(i*J)+j));
        }
    }

    imageAssigned = true;
}

template<real T>
void FunctionRelation<T>::PopulateImageFromSingleRecursiveFunc(RecursiveFunctionRule<T> &f, FunctionRelation<T> **func)
{
    // initialize first value
    image[0][0] = f.getInitValue();
    //initialize all elements from previous values
    for (unsigned int j=1; j<J; j++)
    {
        image[0][j] = f.getFromPreviousInImage(func, (unsigned int)(j));
    }
    for (unsigned int i=1; i<I; i++)
    { 
        for (unsigned int j=0; j<J; j++)
        {
            image[i][j] = f.getFromPreviousInImage(func, (unsigned int)((i*J)+j));
        }
    }

    imageAssigned = true;
}

template <real T>
inline void FunctionRelation<T>::ModifyFromSimpleFunc(FunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params)
{
    // setImageElem(f());
}

template <real T>
inline void FunctionRelation<T>::ModifyFromPrevRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params)
{
    // set image element using specified index
    this->SetImageElem(f.getFromPreviousInImage(params, i), i);
}

template<real T>
inline void FunctionRelation<T>::ModifyFromLastRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params)
{
    // set image element using the last element in the image
    this->SetImageElem(f.getFromPreviousInImage(params, (unsigned int)(I*J)), i);
}

template<real T>
inline const T FunctionRelation<T>::GetDomainElem(const unsigned int i) const
{
    return domain[(unsigned int)(i/J)][(unsigned int)(i%J)];
}

template<real T>
inline const T FunctionRelation<T>::GetImageElem(const unsigned int i) const
{
    return image[(unsigned int)(i/J)][(unsigned int)(i%J)];
}

// Return image and domains as ptrs. Use with care
// ===============================================
template<real T>
inline T** FunctionRelation<T>::GetDomain() const
{
    return domain;
}

template<real T>
inline T** FunctionRelation<T>::GetImage() const
{
    return image;
}

template<real T>
inline const bool FunctionRelation<T>::IsDomainAssigned() const
{
    return domainAssigned;
}

template<real T>
inline const bool FunctionRelation<T>::IsDomainAllocated() const
{
    return domainAllocated;
}

template<real T>
inline const T FunctionRelation<T>::Getdx() const
{
    return dx;
}

template<real T>
inline const size_t FunctionRelation<T>::GetI() const
{
    return I;
}

template<real T>
inline const size_t FunctionRelation<T>::GetJ() const
{
    return J;
}

template<real T>
inline void FunctionRelation<T>::SetDomainElem(const T x, const unsigned int i)
{
    domain[(unsigned int)(i/J)][(unsigned int)(i%J)] = x;
}

template<real T>
inline void FunctionRelation<T>::SetImageElem(const T x, const unsigned int i)
{
    image[(unsigned int)(i/J)][(unsigned int)(i%J)] = x;
}

template<real T>
const int FunctionRelation<T>::getSkip(unsigned int* shape, unsigned int dim, unsigned int max_dim) const
{
    assert(max_dim>dim);
    int skip = 1;
    for(int i=0; i<dim; i++)
    {
        skip*=shape[i];
    }
    return skip;
}