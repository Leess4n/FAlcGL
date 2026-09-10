add_library(FractalAlchemy_Templates INTERFACE)

# specify header files
target_sources(FractalAlchemy_Templates INTERFACE
                        FunctionRule.hxx
                        FunctionRelation.hxx
                        RecursiveFunctionRule.hxx
                        FunctionManager.hxx
                        RecursiveFunctionManager.hxx)