#include "stdafx.h"

struct IFoo { virtual ~IFoo() {} };
class Foo : public IFoo
{
public:

   Foo() {}

   Foo(std::string s)
      : value(s)
   {}

   std::string value;
};
class Foo2 : public IFoo {};

SUITE(Registration)
{
   TEST(ShouldRegister)
   {
      using namespace Yokto;

      auto container = Container::Create();
      container->Register<IFoo>([](Container) { return new Foo; });
   }
}
