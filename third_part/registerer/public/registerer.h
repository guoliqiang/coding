// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)
// Registerer is a factory register mechanism that make class registration at
// compile time and allow generating an object by giving the name. Example:
//
//     class BaseClass {  // base class
//       ...
//     };
//     REGISTER_REGISTERER(BaseClass);
//     #define REGISTER_BASECLASS(name) REGISTER_CLASS(BaseClass, name)
//
//     class Sub1 : public BaseClass {
//       ...
//     };
//     REGISTER_BASE(Sub1);
//     class Sub2 : public BaseClass {
//       ...
//     };
//     REGISTER_BASE(Sub2);
//
// Note that REGISTER_BASE(sub1) should be put in cc file instead of h file,
// to avoid multi-declaration error when compile.
//
// Then you could get a new object of the sub class by:
//    Base *obj = BaseClassRegisterer::GetInstanceByName("Sub1");
// or get a singleton object of the sub class by:
//    Base* obj = BaseClassRegisterer::GetSingletonInstanceByName("Sub1");
//
// This is convenient when you need decide the class at runtime or by flag:
//    string name = "Sub1";
//    if (...)
//      name = "Sub2";
//    Base *obj = BaseClassRegisterer::GetInstanceByName(name);
//
// If there should be only one instance in the program by desgin,
// GetUniqInstance could be used:
//    Base *obj = BaseClassRegisterer::GetUniqInstance();
// If multi sub classes are registered, this method will cause a CHECK fail.
//
// For more cases, see registerer_test.cc or contact dahaili@yunrang

#ifndef UTIL_REGISTERER_REGISTERER_H_
#define UTIL_REGISTERER_REGISTERER_H_

#include <map>
#include <string>
#include "../../../base/public/logging.h"
#include "../../../base/public/singleton.h"

namespace registerer {

// idea from boost any but make it more simple and don't use type_info.
class Any {
 public:
  Any() : content_(NULL) {}

  template<typename ValueType>
  Any(const ValueType &value)  // NOLINT
      : content_(new Holder<ValueType>(value)) {}

  Any(const Any &other)
      : content_(other.content_ ? other.content_->clone() : NULL) {
  }

  ~Any() {
    delete content_;
  }

  template<typename ValueType>
  ValueType *any_cast() {
    return content_ ?
      &static_cast<Holder<ValueType> *>(content_)->held_ : NULL;  // NOLINT
  }

 private:
  class PlaceHolder {
   public:
    virtual ~PlaceHolder() {}
    virtual PlaceHolder *clone() const = 0;
  };

  template<typename ValueType>
  class Holder : public PlaceHolder {
   public:
    explicit Holder(const ValueType &value) : held_(value) {}
    virtual PlaceHolder *clone() const {
      return new Holder(held_);
    }

    ValueType held_;
  };

  PlaceHolder *content_;
};

class ObjectFactory {
 public:
  ObjectFactory() {}
  virtual ~ObjectFactory() {}
  virtual Any NewInstance() {
    return Any();
  }
  virtual Any GetSingletonInstance() {
    return Any();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(ObjectFactory);
};

typedef std::map<std::string, ObjectFactory*> FactoryMap;
typedef std::map<std::string, FactoryMap> BaseClassMap;
BaseClassMap& global_factory_map();

}  // namespace registerer

#define REGISTER_REGISTERER(base_class) \
  class base_class ## Registerer { \
    typedef ::registerer::Any Any;\
    typedef ::registerer::FactoryMap FactoryMap; \
    public: \
      static base_class *GetInstanceByName(const ::std::string &name) { \
        FactoryMap &map = ::registerer::global_factory_map()[#base_class]; \
        FactoryMap::iterator iter = map.find(name); \
        if (iter == map.end()) { \
          LOG(ERROR) << "Get instance " << name << " failed."; \
          return NULL; \
        } \
        Any object = iter->second->NewInstance(); \
        return *(object.any_cast<base_class*>()); \
      } \
      static base_class* GetSingletonInstanceByName( \
          const ::std::string& name) { \
        FactoryMap& map = ::registerer::global_factory_map()[#base_class]; \
        FactoryMap::iterator iter = map.find(name); \
        if (iter == map.end()) { \
          LOG(ERROR) << "Get singleton instance " << name << " failed."; \
          return NULL; \
        }\
        Any object = iter->second->GetSingletonInstance(); \
        return *(object.any_cast<base_class*>()); \
      } \
      static const ::std::string GetUniqInstanceName() { \
        FactoryMap &map = ::registerer::global_factory_map()[#base_class]; \
        CHECK_EQ(map.size(), 1) << map.size(); \
        return map.begin()->first; \
      } \
      static base_class *GetUniqInstance() { \
        FactoryMap &map = ::registerer::global_factory_map()[#base_class]; \
        CHECK_EQ(map.size(), 1) << map.size(); \
        Any object = map.begin()->second->NewInstance(); \
        return *(object.any_cast<base_class*>()); \
      } \
      static bool IsValid(const ::std::string &name) { \
        FactoryMap &map = ::registerer::global_factory_map()[#base_class]; \
        return map.find(name) != map.end(); \
      } \
  }; \

#define REGISTER_CLASS(clazz, name) \
  namespace registerer { \
  class ObjectFactory##name : public ::registerer::ObjectFactory { \
    public: \
      ::registerer::Any NewInstance() { \
        return ::registerer::Any(new name()); \
      } \
      ::registerer::Any GetSingletonInstance() { \
        return ::registerer::Any(Singleton<name>::get()); \
      } \
  }; \
  void register_factory_##name() { \
    ::registerer::FactoryMap &map = \
      ::registerer::global_factory_map()[#clazz]; \
    if (map.find(#name) == map.end()) \
      map[#name] = new ObjectFactory##name(); \
  } \
  __attribute__((constructor)) void register_factory_##name(); \
  }

#endif  // UTIL_REGISTERER_REGISTERER_H_
