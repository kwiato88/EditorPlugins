namespace FirstNamespace
{
    class BaseClass
    {};
}

namespace SecondNamespace
{
    class DerivedClass : public FirstNamespace::BaseClass
    {};
    
    class BaseClass2
    {};

namespace NestedNamespace
{
    class SecondBaseClass
    {};
    
    class DervedFromBaseClass2 : public SecondNamespace::BaseClass2
    {};
}
    class DerivedFrom2Classes : public DerivedClass, public NestedNamespace::SecondBaseClass
    {};
}

int main()
{}
