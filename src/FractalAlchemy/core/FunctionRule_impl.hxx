template <real T>
inline const T FunctionRule<T>::operator()(const T x)
{
    // placeholder method
    return x;
}

template <real T>
inline const T FunctionRule<T>::d(const T x, const T dx)
{
    const T df = this->operator()(x + dx) - this->operator()(x);
    return df/dx;
}

template <real T>
const T FunctionRule<T>::intgr(const T x0, const T x1, const T dx)
{
    T A = 0.0;
    const T iter = (x1 - x0)/dx; // Number of iterations

    // Iteratively sum the infinitesimal areas
    for(int i = 0; i < iter; i++)
    {
        A += (this->operator()(x0 + (i*dx)) + this->operator()(x0 + ((i+1)*dx))) * dx / 2;
    }

    return A;
}

template <real T>
const T FunctionRule<T>::newtonSolve(const T x0, const unsigned int iter, const T dx)
{
    T sol = x0;

    // Iteratively approximate solution
    for(int i = 0; i <= iter; i++)
    {
        sol -= this->operator()(sol)/this->der(sol, dx);
    }
    return sol;
}