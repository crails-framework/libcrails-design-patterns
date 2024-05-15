#include <crails/utils/singleton.hpp>

#undef NDEBUG
#include <cassert>

class SingletonTest
{
  SINGLETON(SingletonTest)
  SingletonTest() { exists = true; }
  ~SingletonTest() { exists = false; }
public:
  static bool exists;
};

bool SingletonTest::exists = false;

class SingletonWithParams
{
  SINGLETON(SingletonWithParams)
  SingletonWithParams() { variable = 0; }
  SingletonWithParams(int value) { variable = value; }
public:
  int variable;
};

class ImplementableSingleton
{
  SINGLETON(ImplementableSingleton)
protected:
  ImplementableSingleton(int value) { variable = value; }
  virtual ~ImplementableSingleton() {}
  int variable;
public:
  virtual int get_variable() const = 0;
};

class ImplementedSingleton : public ImplementableSingleton
{
  SINGLETON_IMPLEMENTATION(ImplementedSingleton, ImplementableSingleton)
protected:
  ImplementedSingleton(int value) : ImplementableSingleton(value + 1) { exists = true; }
  ~ImplementedSingleton() { exists = false; }
public:
  int get_variable() const override { return ImplementableSingleton::variable + 1; }
  static bool exists;
};

bool ImplementedSingleton::exists = false;

int main()
{
  // Constructs the singleton class
  SingletonTest::singleton::initialize();
  assert(SingletonTest::exists == true);

  // Free the singleton class
  SingletonTest::singleton::finalize();
  assert(SingletonTest::exists == false);

  // Calls constructor with params
  SingletonWithParams::singleton::initialize(42);
  assert(SingletonWithParams::singleton::get()->variable == 42);
  SingletonWithParams::singleton::finalize();

  // Access singleton implementation through the singleton interface
  ImplementedSingleton::singleton::initialize(23);
  assert(ImplementableSingleton::singleton::get()->get_variable() == 25);
  ImplementableSingleton::singleton::finalize();
  assert(ImplementedSingleton::exists == false);

  // SingletonInstantiator creates and delete singleton when going in and out of scope
  {
    SingletonInstantiator<SingletonTest> test;
    SingletonInstantiator<SingletonWithParams> test2(23);
    assert(SingletonTest::exists == true);
    assert(test2->variable == 23);
  }
  assert(SingletonTest::exists == false);
}
