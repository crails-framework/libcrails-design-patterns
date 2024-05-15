#include <crails/utils/flyweight.hpp>
#include <string>
#include <string_view>

#undef NDEBUG
#include <cassert>

class FlyweightTest : public Flyweight<std::string, std::string_view>
{
  std::shared_ptr<std::string> create_instance(std::string_view key) override
  {
    auto value = std::make_shared<std::string>(key);

    *value += "-instance";
    create_count++;
    return value;
  }
public:
  int create_count = 0;
};

int main()
{
  FlyweightTest flyweight;
  std::shared_ptr<std::string> tmp;

  assert(flyweight.contains("toto") == false);
  assert(*flyweight.require("toto") == "toto-instance");
  assert(flyweight.create_count == 1);
  tmp = flyweight.require("toto");
  assert(flyweight.create_count == 1);
  assert(flyweight.contains("toto") == true);
  flyweight.garbage_collect();
  assert(flyweight.contains("toto") == true);
  tmp = nullptr;
  flyweight.garbage_collect();
  assert(flyweight.contains("toto") == false);
  return 0;
}
