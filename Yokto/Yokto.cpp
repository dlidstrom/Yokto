// Yokto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Container.h"

using namespace Yokto;

struct IFoo {};
class Foo : public IFoo {};

int _tmain(int argc, _TCHAR* argv[])
{
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);

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
		assert(f1==f2.get());
	}

	return 0;
}
