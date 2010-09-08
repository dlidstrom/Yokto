// Yokto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Container.h"

using namespace Yokto;

struct IFoo { virtual ~IFoo() {} };
class Foo : public IFoo {};
class Foo2 : public IFoo {};

int _tmain(int argc, _TCHAR* argv[])
{
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);

   try
   {
      boost::progress_timer timer;

      // ShouldRegister
      {
         auto container = Container::Create();
         container->Register<IFoo>([](Container) { return new Foo; });
      }
      // RegisteredInstanceIsResolved
      {
         auto container = Container::Create();
         auto f1 = new Foo;
         container->Register<IFoo>(f1);
         auto f2 = container->Resolve<IFoo>();
         assert(f2);
         assert(f1==f2.get());
      }
      // ThrowsIfCannotResolve
      {
         auto container = Container::Create();

         try
         {
            auto foo = container->Resolve<IFoo>();
            assert(false && "Should have thrown ResolutionException");
         }
         catch (ResolutionException)
         {
         }
      }
      // RegistersNamedInstances()
		{
			auto container = Container::Create();
         container->Register<IFoo>("foo", [](Container c) { return new Foo; });
         container->Register<IFoo>("foo2", [](Container c) { return new Foo2; });

			auto foo = container->ResolveNamed<IFoo>("foo");
			auto foo2 = container->ResolveNamed<IFoo>("foo2");

			assert(foo!=foo2);
			assert(std::dynamic_pointer_cast<IFoo>(foo));
			assert(std::dynamic_pointer_cast<IFoo>(foo2));
			assert(std::dynamic_pointer_cast<Foo>(foo));
         assert(std::dynamic_pointer_cast<Foo2>(foo2));
		}

      std::cout << "Tests finished successfully in ";
   }
   catch (std::exception& ex)
   {
      std::cerr << ex.what() << std::endl;
   }

   return 0;
}
