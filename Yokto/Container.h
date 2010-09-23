#include "stdafx.h"

namespace Yokto
{
   class Container;
   typedef std::shared_ptr<Container> ContainerPtr;

   struct null_deleter
   {
      void operator()(void*) const {}
   };

   struct ResolutionException : std::runtime_error
   {
      ResolutionException()
         : std::runtime_error("Could not resolve type")
      {}
   };

   class Container
   {
      typedef std::reference_wrapper<const std::type_info> type_info_ref;
      typedef std::pair<std::string, type_info_ref> Key;

      struct key_lt
      {
         bool operator()(const Key& k1, const Key& k2) const
         {
            return k1.first<k2.first || (k1.first==k2.first && k1.second.get().before(k2.second.get()));
         }
      };

      //typedef std::function<boost::any (Container)> Factory;
      typedef boost::any Factory;
      typedef std::map<Key, Factory, key_lt> Factories;
      Factories factories;

   public:

      static ContainerPtr Create()
      {
         return ContainerPtr(new Container());
      }

      template<class T>
      void Register(std::function<T* (Container c)> f)
      {
         Register(std::string(), f);
      }

      template<class T>
      void Register(T* t)
      {
         Factory f = [t](Container c) { return std::shared_ptr<T>(t, null_deleter()); };
         Factories::key_type k = std::make_pair(std::string(), std::cref(typeid(T)));
         factories.insert(std::make_pair(k, f));
      }

      template<class T>
      void Register(const std::string& name, std::function<T* (Container c)> f)
      {
         Factory wrapped = [f, this](Container c) { return std::shared_ptr<T>(f(*this)); };
         Factories::key_type k = std::make_pair(name, std::cref(typeid(T)));
         factories.insert(std::make_pair(k, wrapped));
      }

      template<class T, class C>
      void Register(std::function<T* (Container c, const C& p)> f)
      {
      }

      template<class T>
      std::shared_ptr<T> Resolve()
      {
         return ResolveNamed<T>(std::string());
      }

      template<class T, class C>
      std::shared_ptr<T> Resolve(const C& c)
      {
         return ResolveNamed<T>(std::string(), c);
      }

      template<class T>
      std::shared_ptr<T> ResolveNamed(const std::string& name)
      {
         typedef std::shared_ptr<T> Type;

         Type result;

         Factories::key_type k = std::make_pair(name, std::cref(typeid(T)));
         Factories::const_iterator it = factories.lower_bound(k);
         if (it != factories.end())
         {
            Factory f = it->second;
            auto factory = boost::any_cast<std::function<T* (Container)>>(f);
            result.reset(factory(*this));
         }

         if (!result)
            throw ResolutionException();

         return result;
      }

      template<class T, class C>
      std::shared_ptr<T> ResolveNamed(const std::string& name, const C&)
      {
         return ResolveNamed<T>(name);
      }
   };
}
