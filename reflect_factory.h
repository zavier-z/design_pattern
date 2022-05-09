#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <string>
#include <unordered_map>

template <typename T>
class Singleton {
 public:
  using value_type = T;

  static value_type& GetInstance() {
    static value_type kInstance;
    return kInstance;
  }
};

class Factory {
 public:
  template <typename Base, typename... Args>
  static Base* New(const char* class_name, Args... args) {
    auto table = GetTable();
    if (table.count(class_name)) {
      using newer_type = Base* (*)(Args...);
      return reinterpret_cast<newer_type>(table[class_name])(
          std::forward<Args>(args)...);
    }

    return nullptr;
  }

  static unsigned Register(const char* class_name, void* newer) {
    if (class_name && newer) {
      GetTable()[class_name] = newer;
    }
    return 0;
  }

 private:
  static std::unordered_map<std::string, void*>& GetTable() {
    static std::unordered_map<std::string, void*> kTable;
    return kTable;
  }
};

template <typename T, typename... Args>
void* Newer(Args... args) {
  return new T(std::forward<Args>(args)...);
}

#define CONCAT(A, B) A##B
#define REGISTER(name, ...)                \
  static unsigned CONCAT(name, __LINE__) = \
      Factory::Register(#name, (void*)(&Newer<name, ##__VA_ARGS__>));

#endif  // _FACTORY_H_

