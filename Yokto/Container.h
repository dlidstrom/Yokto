#include "stdafx.h"

namespace Yokto
{
	class Container;
	typedef std::shared_ptr<Container> ContainerPtr;

	class Container
	{
		struct type_info_lt
		{
			bool operator()(const std::type_info& t1, const std::type_info& t2)
			{
				return t1.before(t2) < 0;
			}
		};

		typedef std::reference_wrapper<const std::type_info> type_info_ref;
		typedef std::function<boost::any (Container)> Factory;
		typedef std::map<type_info_ref, Factory, type_info_lt> Factories;
		Factories factories;

	public:

		static ContainerPtr Create()
		{
			return ContainerPtr(new Container());
		}

		template<class T>
		void Register(std::function<T* (Container c)> f)
		{
		}

		template<class T>
		void Register(T* t)
		{
			Factories::value_type v = std::make_pair(std::cref(typeid(T)), [t](Container c) { return t; });
			factories.insert(v);
		}

		template<class T>
		std::shared_ptr<T> Resolve()
		{
			typedef std::shared_ptr<T> Type;
			Factories::const_iterator it = factories.find(typeid(T));
			return it != factories.end() ? boost::any_cast<Type>(it->second(*this)) : Type();
		}
	};
}
