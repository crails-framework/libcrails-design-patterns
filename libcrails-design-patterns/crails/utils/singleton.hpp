#ifndef  SINGLETON_HPP
# define SINGLETON_HPP

# include <crails/utils/backtrace.hpp>
# include <string>

# define SINGLETON(type) \
public:\
  typedef Singleton<type> singleton; \
private:\
  friend class Singleton<type>; \
  static constexpr const char* _singleton_typename = #type;

# define SINGLETON_IMPLEMENTATION(type, from_type) \
public:\
  typedef SingletonImplementation<type, from_type> singleton;\
private:\
  friend class Singleton<from_type>; \
  friend class SingletonImplementation<type, from_type>;

template<typename TYPE>
class Singleton
{
public:
  template<typename... Args>
  static void initialize(Args... args)
  {
    if (!(ptr))
      ptr = new TYPE(args...);
    else
      throw boost_ext::runtime_error(std::string("Singleton ") + TYPE::_singleton_typename + " was already initialized");
  }

  template<typename IMPLEMENTATION, typename... Args>
  static void implement(Args... args)
  {
    if (!(ptr))
      ptr = new IMPLEMENTATION(args...);
    else
      throw boost_ext::runtime_error(std::string("Singleton ") + TYPE::_singleton_typename + " was already initialized");
  }

  static void finalize(void)
  {
    if (ptr)
    {
      delete ptr;
      ptr = 0;
    }
  }

  static TYPE*  get(void) { return (ptr); }

  static TYPE&  require(void)
  {
    if (!ptr)
      throw boost_ext::runtime_error(std::string("Singleton ") + TYPE::_singleton_typename + " is not initialized");
    return *ptr;
  }

private:
  static TYPE* ptr;
};

template<typename TYPE, typename FROM>
class SingletonImplementation
{
public:
  template<typename... Args>
  static void initialize(Args... args)
  {
    FROM::singleton::template implement<TYPE>(args...);
  }

  static void finalize(void)
  {
    FROM::singleton::finalize();
  }

  static TYPE* get(void) { return reinterpret_cast<TYPE*>(FROM::singleton::get()); }

  static TYPE& require(void) { return reinterpret_cast<TYPE&>(FROM::singleton::require()); }
};

template<typename TYPE>
struct SingletonInstantiator
{
  template<typename... Args>
  SingletonInstantiator(Args... args)
  {
    TYPE::singleton::initialize(args...);
  }

  SingletonInstantiator(const SingletonInstantiator&) = delete;

  ~SingletonInstantiator()
  {
    TYPE::singleton::finalize();
  }

  TYPE* operator->() const { return TYPE::singleton::get(); }

  TYPE& operator*() const { return *(reinterpret_cast<TYPE*>(TYPE::singleton::get())); }
};

template<typename TYPE> TYPE* Singleton<TYPE>::ptr = 0;

#endif
