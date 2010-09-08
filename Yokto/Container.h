#include "stdafx.h"

namespace Yokto
{
	class Container;
	typedef std::shared_ptr<Container> ContainerPtr;

	struct null_deleter
	{
		void operator()(void*) const {}
	};

	class Container
	{
		struct type_info_lt
		{
			bool operator()(const std::type_info& t1, const std::type_info& t2) const
			{
				return t1.before(t2) < 0;
			}
		};

		typedef std::reference_wrapper<const std::type_info> type_info_ref;
		typedef std::function<boost::any (Container)> Factory;
		typedef std::map<type_info_ref, Factory, type_info_lt> Factories;
		Factories factories;

		template<class T>
		static std::function<std::shared_ptr<T> (Container)> make_wrapper(T* t)
		{
			auto f = [t](Container c) { return std::shared_ptr<T>(t, null_deleter()); };
			return f;
		}

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
			Factories::value_type v = std::make_pair(std::cref(typeid(T)), make_wrapper<T>(t));
			factories.insert(v);
		}

		template<class T>
		std::shared_ptr<T> Resolve()
		{
			typedef std::shared_ptr<T> Type;

			Type result;

			Factories::const_iterator it = factories.find(typeid(T));
			if (it != factories.end())
			{
				Factory f = it->second;
				result = boost::any_cast<Type>(f(*this));
			}

			return result;
		}
	};
}
