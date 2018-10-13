// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantTo.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicMemoryPool.hpp"
#include "Memory/StaticMemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TMemoryPool>
class JsonDocument : public Visitable {
 public:
  uint8_t nestingLimit;

  JsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getVariant().accept(visitor);
  }

  template <typename T>
  typename JsonVariantAs<T>::type as() {
    return getVariant().template as<T>();
  }

  template <typename T>
  typename JsonVariantConstAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  void clear() {
    _memoryPool.clear();
    _rootData.type = JSON_NULL;
  }

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  size_t memoryUsage() const {
    return _memoryPool.size();
  }

  TMemoryPool& memoryPool() {
    return _memoryPool;
  }

  template <typename T>
  typename JsonVariantTo<T>::type to() {
    _memoryPool.clear();
    return getVariant().template to<T>();
  }

 protected:
  template <typename T>
  void copy(const T& src) {
    to<JsonVariant>().set(src.as<JsonVariant>());
  }

 private:
  JsonVariant getVariant() {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  JsonVariantConst getVariant() const {
    return JsonVariantConst(&_rootData);
  }

  TMemoryPool _memoryPool;
  JsonVariantData _rootData;
};

class DynamicJsonDocument : public JsonDocument<DynamicMemoryPool> {
 public:
  DynamicJsonDocument() {}
  DynamicJsonDocument(size_t capacity) {
    memoryPool().reserve(capacity);
  }

  DynamicJsonDocument(const DynamicJsonDocument& src) {
    nestingLimit = src.nestingLimit;
    copy(src);
  }

  template <typename TMemoryPool>
  DynamicJsonDocument(const JsonDocument<TMemoryPool>& src) {
    nestingLimit = src.nestingLimit;
    copy(src);
  }

  DynamicJsonDocument operator=(const DynamicJsonDocument& src) {
    nestingLimit = src.nestingLimit;
    copy(src);
    return *this;
  }
};

template <size_t CAPACITY>
class StaticJsonDocument : public JsonDocument<StaticMemoryPool<CAPACITY> > {
 public:
  StaticMemoryPoolBase& memoryPool() {
    return JsonDocument<StaticMemoryPool<CAPACITY> >::memoryPool();
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
