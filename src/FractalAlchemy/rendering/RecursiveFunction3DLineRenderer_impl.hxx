#include "RecursiveFunction3DLineRenderer.hxx"

template<
    real T,
    template<real> class RecursiveFunctionX,
    template<real> class RecursiveFunctionY,
    template<real> class RecursiveFunctionZ>
template<typename... Args>
RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::RecursiveFunction3DLineRenderer(size_t I, size_t J, const T x0, const T t0, const T tFinal, Args... args)
{
    this->managerX = new RecursiveFunctionManager<T, RecursiveFunctionX>(I, J, x0, args...);
    this->managerY = new RecursiveFunctionManager<T, RecursiveFunctionY>(I, J, this->managerX->GetDomain(), x0, args...);
    this->managerZ = new RecursiveFunctionManager<T, RecursiveFunctionZ>(I, J, this->managerX->GetDomain(), x0, args...);
    this->I = this->managerX->GetI();
    this->J = this->managerX->GetJ();
    this->totalSize = this->I*this->J;

    this->managerX->InitDomainAndX0(t0, tFinal); // initialize the domain and the inital condition
    this->managerY->InitX0();                    // only intialize initial condition because domain has been initialized in previous object
    this->managerZ->InitX0();                    // only intialize initial condition because domain has been initialized in previous object

    // Populate from the start the images
    this->UnsafeUpdateImagesBy(1, I*J - 1);


    // Create and bind the vao
    VAOs.reserve(I);
    VBOsX.reserve(I);
    VBOsY.reserve(I);
    VBOsZ.reserve(I);

    // initialize the VAOs and VBOs
    for (unsigned int i=0;i<I;i++)
    {
        VAOs.emplace_back();
        VAOs.back().Bind();

        VBOsX.emplace_back(
            managerX->GetImageArray(i), J * sizeof(T), GL_DYNAMIC_DRAW
        );

        VBOsY.emplace_back(
            managerY->GetImageArray(i), J * sizeof(T), GL_DYNAMIC_DRAW
        );

        VBOsZ.emplace_back(
            managerZ->GetImageArray(i), J * sizeof(T), GL_DYNAMIC_DRAW
        );

        // Configuring attribute reads
        VAOs.back().LinkAttrib(VBOsX[i], 0, 1, GL_FLOAT, 1*sizeof(float), (void*)0);
        VAOs.back().LinkAttrib(VBOsY[i], 1, 1, GL_FLOAT, 1*sizeof(float), (void*)0);
        VAOs.back().LinkAttrib(VBOsZ[i], 2, 1, GL_FLOAT, 1*sizeof(float), (void*)0);
    }
}

template<
    real T,
    template<real> class RecursiveFunctionX,
    template<real> class RecursiveFunctionY,
    template<real> class RecursiveFunctionZ>
void RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::Render(Camera& camera)
{
    this->shader.use();

    glm::mat4 model(1.0f);

    this->shader.setMat4("model", model);
    camera.viewToShader("view", this->shader);
    camera.projToShader("projection", this->shader);

    for (size_t i = 0; i < this->managerX->GetI(); ++i)
    {
        VAOs[i].Bind();
        glLineWidth(1.0f);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(J));
    }

    VAOs[I - 1].Unbind();
}

template<
    real T,
    template<real> class RecursiveFunctionX,
    template<real> class RecursiveFunctionY,
    template<real> class RecursiveFunctionZ>
int RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::UnsafeUpdateImagesBy(const unsigned int i, const unsigned int span)
{
    /*
    This function safely clips the span if it goes out of bounds of the managers as a default behaviour.
    */
    assert(span <= totalSize);
    assert(span > 0);

    // Clip the span
    // =============
    int clippedSpan = span;
    if (i + span > totalSize)
    {
        clippedSpan = totalSize - i;
    }
    
    // Initiaize the parameter list
    // ============================
    FunctionRelation<T> *params[3] = {&*(this->managerX), &*(this->managerY), &*(this->managerZ)};

    // calculater remainders in start column
    int startColumn = (unsigned int)(i / this->J);
    int startRemainder = i % this->J;
    int startMissing = this->J - (startRemainder);

    // calculate remainders in end column
    int endColumn = (unsigned int)((i+clippedSpan) / this->J);
    int endRemainder = (i+clippedSpan) % this->J; 
        
    // if the span is less than the amount needed to jump to the next array then
    // return after a simple for loop.
    if (clippedSpan<=startMissing)
    {
        for (unsigned int n=0; n<clippedSpan; n++)
        {
            this->managerX->ModifyFromPrev(startColumn, startRemainder + n, params);
            this->managerY->ModifyFromPrev(startColumn, startRemainder + n, params);
            this->managerZ->ModifyFromPrev(startColumn, startRemainder + n, params);
        }
    }
    // Otherwise perform 3 loops
    else
    {
        for (unsigned int m=0; m<startMissing; m++)
        {
            this->managerX->ModifyFromPrev(startColumn, startRemainder + m, params);
            this->managerY->ModifyFromPrev(startColumn, startRemainder + m, params);
            this->managerZ->ModifyFromPrev(startColumn, startRemainder + m, params);
        }
        for (unsigned int n=1; n<endColumn-startColumn; n++)
        {
            for (unsigned int m=0; m<this->J; m++)
            {
                this->managerX->ModifyFromPrev(startColumn + n, m, params);
                this->managerY->ModifyFromPrev(startColumn + n, m, params);
                this->managerZ->ModifyFromPrev(startColumn + n, m, params);
            }
        }
        for (unsigned int m=0; m<endRemainder; m++)
        {
            this->managerX->ModifyFromPrev(endColumn, m, params);
            this->managerY->ModifyFromPrev(endColumn, m, params);
            this->managerZ->ModifyFromPrev(endColumn, m, params);
        }
    }

    return clippedSpan;
}

template<
    real T,
    template<real> class RecursiveFunctionX,
    template<real> class RecursiveFunctionY,
    template<real> class RecursiveFunctionZ>
int RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::SafeUpdateImagesBy(const unsigned int span)
{    
    // do not allow for multiple cycles around the array
    assert(span <= totalSize);
    assert(span > 0);
    
    updateSpan += span;
    int firstUnsafeSpan = 0;
    // std::cout << "ATTENTION "<<totalSize << std::endl;
    // std::cout << "image index = "<< currentImageIndex << "next index " << (currentImageIndex + span) % totalSize << std::endl;  
    if (currentImageIndex == 0)
    {
        UpdateImagesFromLastElement();
        if (span>1)
            UnsafeUpdateImagesBy(1, span-1);
    }
    else if (currentImageIndex + span > totalSize)
    {
        firstUnsafeSpan = UnsafeUpdateImagesBy(currentImageIndex, span);
        // std::cout <<"first unsafe span"<<firstUnsafeSpan<<std::endl;
        UpdateImagesFromLastElement();
        UnsafeUpdateImagesBy(1, span - firstUnsafeSpan - 1);
    }    
    else
    {
        UnsafeUpdateImagesBy(currentImageIndex, span);
    }
    currentImageIndex = (currentImageIndex + span) % totalSize;
    
    return updateSpan;
}

template <
    real T,
    template <real> class RecursiveFunctionX,
    template <real> class RecursiveFunctionY,
    template <real> class RecursiveFunctionZ>
void RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::UpdateImagesFromLastElement()
{
    FunctionRelation<T> *params[3] = {&*(this->managerX), &*(this->managerY), &*(this->managerZ)};

    this->managerX->ModifyFromLast(0, params);
    this->managerY->ModifyFromLast(0, params);
    this->managerZ->ModifyFromLast(0, params);
}

template <
    real T,
    template <real> class RecursiveFunctionX,
    template <real> class RecursiveFunctionY,
    template <real> class RecursiveFunctionZ>
void RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::UnsafeUpdateVBOs(const unsigned int i, const unsigned int span)
{
    assert(span <= totalSize);
    assert(span > 0);

    int clippedSpan = span;
    if (i + span > totalSize)
    {
        clippedSpan = totalSize - i;
    }
    // std::cout << i + clippedSpan << std::endl;

    int I = this->managerX->GetI(), J = this->managerX->GetJ();
    assert(
        ((i<I*J && clippedSpan < I*J),
        "The index i and the span must be within the bounds I*J")
    );
    // calculater remainders in start column
    int startColumn = (unsigned int)(i / J);
    int startRemainder = i % J;
    int startMissing = J - (startRemainder);

    // calculate remainders in end column
    int endColumn = (unsigned int)((i+clippedSpan) / J);
    int endRemainder = (i+clippedSpan) % J;
    if (endColumn == I)
    {
        endColumn = I-1;
        endRemainder = (i+clippedSpan-1) % J;
    }

    // size of each array element
    unsigned int size = sizeof(T);
    unsigned int currentColumn = 0;
    for (unsigned int n=0; n<I; n++)
    {
        currentColumn = (startColumn + n) % I;
        VBOsX[n].SubData(this->managerX->GetImageArray(currentColumn) + startRemainder, 0, startMissing*size, GL_DYNAMIC_DRAW);
        VBOsY[n].SubData(this->managerY->GetImageArray(currentColumn) + startRemainder, 0, startMissing*size, GL_DYNAMIC_DRAW);
        VBOsZ[n].SubData(this->managerZ->GetImageArray(currentColumn) + startRemainder, 0, startMissing*size, GL_DYNAMIC_DRAW);

        currentColumn = (startColumn + n + 1) % I;
        VBOsX[n].SubData(this->managerX->GetImageArray(currentColumn), startMissing*size, startRemainder*size, GL_DYNAMIC_DRAW);
        VBOsY[n].SubData(this->managerY->GetImageArray(currentColumn), startMissing*size, startRemainder*size, GL_DYNAMIC_DRAW);
        VBOsZ[n].SubData(this->managerZ->GetImageArray(currentColumn), startMissing*size, startRemainder*size, GL_DYNAMIC_DRAW);

    }
}

template <
    real T,
    template <real> class RecursiveFunctionX,
    template <real> class RecursiveFunctionY,
    template <real> class RecursiveFunctionZ>
void RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::SafeUpdateVBOs()
{
    UnsafeUpdateVBOs(currentImageIndex, updateSpan);
    updateSpan = 0;
    return ;
}

template <
    real T,
    template <real> class RecursiveFunctionX,
    template <real> class RecursiveFunctionY,
    template <real> class RecursiveFunctionZ>
Shader& RecursiveFunction3DLineRenderer<T, RecursiveFunctionX, RecursiveFunctionY, RecursiveFunctionZ>::GetShader()
{
    return this->shader;
}
