# Yokto - Simple inversion of control for C++0x

This is a simple library for inversion of control, using C++0x features.

# Inversion of Control
Have a look at [Inversion of Control](http://en.wikipedia.org/wiki/Inversion_of_control) for an introduction to the concept.

## Quick Start

### Registering a factory

Here's a sample code showing how to register a factory function in the container:

      #include "Container.h"
	  
	  struct IFoo { virtual ~IFoo() {} };
	  class Foo : public IFoo {};
	  
      // ShouldRegister
      {
         auto container = Container::Create();
         container->Register<IFoo>([](Container) { return new Foo; });
      }

The above code shows how to register a factory function that simply returns
an instance of the Foo class.

### Using the container as a service locator

Now an example of how to resolve an instance:

      // RegisteredInstanceIsResolved
      {
         auto container = Container::Create();
         auto f1 = new Foo;
         container->Register<IFoo>(f1);
         auto f2 = container->Resolve<IFoo>();
         assert(f2);
         assert(f1==f2.get());
      }

This example actually shows how to register a pre-created instance, instead of the factory function.
Note that `container->Resolve<IFoo>()` will return a `std::shared_ptr<IFoo>`. The usage shown above,
where the container is used directly, is usually called [Service Locator](http://martinfowler.com/articles/injection.html#UsingAServiceLocator).

### More examples

For more examples, have a look at the library [tests](http://github.com/dlidstrom/Yokto/blob/master/Yokto/Yokto.cpp).
